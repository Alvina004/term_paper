#include "mainwindow.h"
#include "problem_registry.h"
#include <QPainter>
#include <QMessageBox>
#include <QGroupBox>
#include <sstream>
using namespace std;

BucketWidget::BucketWidget(const QString& name, int capacity, QWidget* parent)
    : QWidget(parent), bucketName(name), maxCapacity(capacity), currentValue(0) {
    setMinimumSize(150, 300);
}

void BucketWidget::setValue(int value) {
    currentValue = qBound(0, value, maxCapacity);
    update();
}

void BucketWidget::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int w = width();
    int h = height();


    int bucketWidth = 100;
    int bucketHeight = 200;
    int x = (w - bucketWidth) / 2;
    int y = h - bucketHeight - 40;

    // դույլի սև գծանշումն ենք անում
    painter.setPen(QPen(Qt::black, 3));
    painter.setBrush(Qt::white);
    painter.drawRect(x, y, bucketWidth, bucketHeight);

    // հասկանում ենք, ըստ տրված արժեքի դույլի որ հատվածն է լցվելու
    if (currentValue > 0) {
        int waterHeight = (bucketHeight * currentValue) / maxCapacity;
        QLinearGradient gradient(x, y + bucketHeight - waterHeight, x, y + bucketHeight);
        gradient.setColorAt(0, QColor(100, 180, 255));
        gradient.setColorAt(1, QColor(50, 120, 200));
        painter.setBrush(gradient);
        painter.setPen(Qt::NoPen); // Քանի որ ջրով լցված հատվածը գծանշում չի ունենալու, հանում ենք pen-ի ռեժիմը
        painter.drawRect(x, y + bucketHeight - waterHeight, bucketWidth, waterHeight);
    }

    // Labels
    painter.setPen(Qt::black);
    QFont font = painter.font();
    font.setPointSize(12);
    font.setBold(true);
    painter.setFont(font);
    painter.drawText(rect(), Qt::AlignTop | Qt::AlignHCenter, bucketName);
    QString label = QString("%1L / %2L").arg(currentValue).arg(maxCapacity);
    painter.drawText(QRect(0, h - 30, w, 30), Qt::AlignCenter, label);
}

//  MainWindow Implementation
ProblemDefinition createBucketProblem();

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), problem(nullptr), interpreter(nullptr),
    currentStep(0), isRunning(false) {

    ProblemRegistry::instance().registerProblem(createBucketProblem());
    problem = ProblemRegistry::instance().getProblem("buckets_2_7_3");

    if (!problem) {
        QMessageBox::critical(this, "Error", "Failed to load problem!");
        return;
    }

    interpreter = new Interpreter(problem);
    stepTimer = new QTimer(this);
    stepTimer->setInterval(1000);

    setupUI();
    updateBucketDisplay();
    connect(stepTimer, &QTimer::timeout, this, &MainWindow::executeNextStep);
}

MainWindow::~MainWindow() {
    delete interpreter;
}

void MainWindow::setupUI() {
    setWindowTitle("Դույլերի խնդիր - Water Bucket Problem");
    resize(1000, 700);

    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);

    QVBoxLayout* leftPanel = new QVBoxLayout();

    QGroupBox* descGroup = new QGroupBox(" Խնդրի նկարագրություն");
    QVBoxLayout* descLayout = new QVBoxLayout(descGroup);

    descriptionLabel = new QLabel(
        "<h3>Դույլերի խնդիր (2L + 7L → 3L)</h3>"
        "<p>Տրված են երկու դույլ՝ <b>2 լիտր</b> և <b>7 լիտր</b> տարողությամբ։<br>"
        "Նպատակ՝ ստանալ ճշգրիտ <b style='color: green;'>3 լիտր</b> ջուր։</p>"
        "<p><b>Թույլատրելի հրամաններ:</b><br>"
        "• FILL X - Լցնել X դույլը<br>"
        "• FILL Y - Լցնել Y դույլը<br>"
        "• EMPTY X - Դատարկել X դույլը<br>"
        "• EMPTY Y - Դատարկել Y դույլը<br>"
        "• POUR X Y - Լցնել X-ից Y<br>"
        "• POUR Y X - Լցնել Y-ից X</p>"
        );
    descriptionLabel->setWordWrap(true);
    descLayout->addWidget(descriptionLabel);
    leftPanel->addWidget(descGroup);

    QGroupBox* codeGroup = new QGroupBox("📥Ձեր լուծումը (մեկ հրաման յուրաքանչյուր տողում)");
    QVBoxLayout* codeLayout = new QVBoxLayout(codeGroup);

    codeEditor = new QTextEdit();
    codeEditor->setPlaceholderText("Օրինակ:\nFILL Y\nPOUR Y X\nEMPTY X\nPOUR Y X");
    QFont monoFont("Courier");
    monoFont.setPointSize(11);
    codeEditor->setFont(monoFont);
    codeLayout->addWidget(codeEditor);

    leftPanel->addWidget(codeGroup);

    // Buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    runButton = new QPushButton("▶️ Կատարել");
    runButton->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; font-size: 14px; padding: 8px; }");

    stepButton = new QPushButton("⏭ Քայլ առ Քայլ");
    stepButton->setStyleSheet("QPushButton { background-color: #2196F3; color: white; font-size: 14px; padding: 8px; }");

    resetButton = new QPushButton("⬅️ Վերսկսել");
    resetButton->setStyleSheet("QPushButton { background-color: #FF9800; color: white; font-size: 14px; padding: 8px; }");

    buttonLayout->addWidget(runButton);
    buttonLayout->addWidget(stepButton);
    buttonLayout->addWidget(resetButton);
    leftPanel->addLayout(buttonLayout);

    // Output console
    QGroupBox* outputGroup = new QGroupBox("📤 Ելք");
    QVBoxLayout* outputLayout = new QVBoxLayout(outputGroup);

    outputConsole = new QTextEdit();
    outputConsole->setReadOnly(true);
    outputConsole->setMaximumHeight(150);
    outputConsole->setFont(monoFont);
    outputLayout->addWidget(outputConsole);

    leftPanel->addWidget(outputGroup);

    mainLayout->addLayout(leftPanel, 2);

    //Right Panel:
    QVBoxLayout* rightPanel = new QVBoxLayout();

    QGroupBox* visualGroup = new QGroupBox("🪣  Տեսողական ցուցադրում");
    QVBoxLayout* visualLayout = new QVBoxLayout(visualGroup);

    // label
    statusLabel = new QLabel("Սկզբնական վիճակ");
    statusLabel->setAlignment(Qt::AlignCenter);
    QFont statusFont = statusLabel->font();
    statusFont.setPointSize(14);
    statusFont.setBold(true);
    statusLabel->setFont(statusFont);
    statusLabel->setStyleSheet("QLabel { background-color: #E3F2FD; padding: 10px; border-radius: 5px; }");
    visualLayout->addWidget(statusLabel);

    // Buckets
    QHBoxLayout* bucketsLayout = new QHBoxLayout();
    bucketX = new BucketWidget("X դույլ (2L)", 2);
    bucketY = new BucketWidget("Y դույլ (7L)", 7);

    bucketsLayout->addWidget(bucketX);
    bucketsLayout->addStretch();
    bucketsLayout->addWidget(bucketY);

    visualLayout->addLayout(bucketsLayout);
    rightPanel->addWidget(visualGroup);

    mainLayout->addLayout(rightPanel, 1);

    // Connect signals
    connect(runButton, &QPushButton::clicked, this, &MainWindow::onRunClicked);
    connect(resetButton, &QPushButton::clicked, this, &MainWindow::onResetClicked);
    connect(stepButton, &QPushButton::clicked, this, &MainWindow::onStepClicked);
}

void MainWindow::updateBucketDisplay() {
    auto state = interpreter->getState();
    bucketX->setValue(state["bucketX"]);
    bucketY->setValue(state["bucketY"]);
}

void MainWindow::showMessage(const QString& msg, const QString& color) {
    outputConsole->append(QString("<span style='color: %1;'>%2</span>").arg(color, msg));
}

void MainWindow::onRunClicked() {
    if (isRunning) return; //եթե արդեն run  ա լինում, սաղ մաքրում ենք սկզբնական վիճակի, որ ապահովռնք որ նոր run ը մաքուր լինի

    onResetClicked();

    QString code = codeEditor->toPlainText();
    istringstream stream(code.toStdString());
    string line;
    commands.clear();

    while (getline(stream, line)) {
        size_t start = line.find_first_not_of(" \t\r\n");
        if (start != string::npos) {
            line = line.substr(start);
            size_t end = line.find_last_not_of(" \t\r\n");
            line = line.substr(0, end + 1);
            if (!line.empty() && line[0] != '#') {
                commands.push_back(line);
            }
        }
    }

    if (commands.empty()) {
        QMessageBox::warning(this, "Սխալ", "Խնդրում ենք մուտքագրել հրամաններ!");
        return;
    }

    outputConsole->clear();
    showMessage("=== Սկսում ենք կատարումը ===", "blue");

    currentStep = 0;
    isRunning = true;
    runButton->setEnabled(false);
    stepButton->setEnabled(false);
    codeEditor->setEnabled(false);

    stepTimer->start();
}

void MainWindow::executeNextStep() {
    if (currentStep >= commands.size()) {
        stepTimer->stop();
        isRunning = false;
        runButton->setEnabled(true);
        stepButton->setEnabled(true);
        codeEditor->setEnabled(true);

        if (!interpreter->isSolved()) {
            showMessage("\n❌ Խնդիրը չի լուծվել", "red");
            statusLabel->setText("❌ Ձախողում");
            statusLabel->setStyleSheet("QLabel { background-color: #FFEBEE; padding: 10px; border-radius: 5px; color: red; }");
        }
        return;
    }

    string cmd = commands[currentStep];
    showMessage(QString("\nՔայլ %1: %2").arg(currentStep + 1).arg(QString::fromStdString(cmd)), "black");

    if (interpreter->executeCommand(cmd)) {
        updateBucketDisplay();

        auto state = interpreter->getState();
        showMessage(QString("→ X=%1L, Y=%2L").arg(state["bucketX"]).arg(state["bucketY"]), "gray");

        if (interpreter->isSolved()) {
            stepTimer->stop();
            isRunning = false;
            runButton->setEnabled(true);
            stepButton->setEnabled(true);
            codeEditor->setEnabled(true);

            showMessage("\n✅ Շնորհավորում ենք! Խնդիրը լուծվեց!", "green");
            statusLabel->setText("✅ Լուծված!");
            statusLabel->setStyleSheet("QLabel { background-color: #C8E6C9; padding: 10px; border-radius: 5px; color: green; }");

            QMessageBox::information(this, "Հաջողություն", "🎉 Շնորհավորում ենք!\nԽնդիրը լուծված է!");
        }
    } else {
        stepTimer->stop();
        isRunning = false;
        runButton->setEnabled(true);
        stepButton->setEnabled(true);
        codeEditor->setEnabled(true);

        showMessage("❌ Սխալ հրաման", "red");
        statusLabel->setText("❌ Սխալ");
        statusLabel->setStyleSheet("QLabel { background-color: #FFEBEE; padding: 10px; border-radius: 5px; color: red; }");
    }

    currentStep++;
}

void MainWindow::onStepClicked() {
    if (isRunning) return;

    if (currentStep == 0) {
        onResetClicked();

        QString code = codeEditor->toPlainText();
        istringstream stream(code.toStdString());
        string line;
        commands.clear();

        while (getline(stream, line)) {
            size_t start = line.find_first_not_of(" \t\r\n");
            if (start != string::npos) {
                line = line.substr(start);
                size_t end = line.find_last_not_of(" \t\r\n");
                line = line.substr(0, end + 1);
                if (!line.empty() && line[0] != '#') {
                    commands.push_back(line);
                }
            }
        }

        if (commands.empty()) {
            QMessageBox::warning(this, "Սխալ", "Խնդրում ենք մուտքագրել հրամաններ!");
            return;
        }

        outputConsole->clear();
        showMessage("=== Քայլ առ քայլ կատարում ===", "blue");
    }

    if (currentStep < commands.size() && !interpreter->isSolved()) {
        executeNextStep();
    }
}

void MainWindow::onResetClicked() {
    stepTimer->stop();
    isRunning = false;
    currentStep = 0;
    commands.clear();

    delete interpreter;
    interpreter = new Interpreter(problem);

    updateBucketDisplay();
    outputConsole->clear();

    statusLabel->setText("Սկզբնական վիճակ");
    statusLabel->setStyleSheet("QLabel { background-color: #E3F2FD; padding: 10px; border-radius: 5px; }");

    runButton->setEnabled(true);
    stepButton->setEnabled(true);
    codeEditor->setEnabled(true);
}
