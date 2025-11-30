#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTimer>
#include "interpreter.h"
#include "problem_definition.h"

class BucketWidget : public QWidget {
    Q_OBJECT
public:
    explicit BucketWidget(const QString& name, int capacity, QWidget* parent = nullptr);
    void setValue(int value);
    int getValue() const { return currentValue; }

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QString bucketName;
    int maxCapacity;
    int currentValue;
};

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onRunClicked();
    void onResetClicked();
    void onStepClicked();
    void executeNextStep();

private:
    void setupUI();
    void updateBucketDisplay();
    void showMessage(const QString& msg, const QString& color);

    // UI Components
    QTextEdit* codeEditor;
    QTextEdit* outputConsole;
    BucketWidget* bucketX;
    BucketWidget* bucketY;
    QPushButton* runButton;
    QPushButton* resetButton;
    QPushButton* stepButton;
    QLabel* statusLabel;
    QLabel* descriptionLabel;

    // Problem solving
    ProblemDefinition* problem;
    Interpreter* interpreter;
    std::vector<std::string> commands;
    int currentStep;
    QTimer* stepTimer;
    bool isRunning;
};

#endif // MAINWINDOW_H
