#include "problem_definition.h"
#include <algorithm>

// Առաջին խնդիր: 2L և 7L դույլեր -> 3L
ProblemDefinition createBucketProblem_2_7_3() {
    ProblemDefinition problem("buckets_2_7_3", "Դույլերի խնդիր (2L + 7L -> 3L)");
    problem.description = "Ստացիր 3 լիտր ջուր օգտագործելով 2L և 7L դույլեր";

    problem.addVariable("bucketX", "X դույլ (2L)", 0, 0, 2);
    problem.addVariable("bucketY", "Y դույլ (7L)", 0, 0, 7);

    problem.addCommand("FILL X", "Լցնել X դույլը",
                       [](std::map<std::string, int>& state) {
                           state["bucketX"] = 2;
                       });

    problem.addCommand("FILL Y", "Լցնել Y դույլը",
                       [](std::map<std::string, int>& state) {
                           state["bucketY"] = 7;
                       });

    problem.addCommand("EMPTY X", "Դատարկել X դույլը",
                       [](std::map<std::string, int>& state) {
                           state["bucketX"] = 0;
                       });

    problem.addCommand("EMPTY Y", "Դատարկել Y դույլը",
                       [](std::map<std::string, int>& state) {
                           state["bucketY"] = 0;
                       });

    problem.addCommand("POUR X Y", "Լցնել X-ից Y",
                       [](std::map<std::string, int>& state) {
                           int x = state["bucketX"];
                           int y = state["bucketY"];
                           int space = 7 - y;
                           int amount = std::min(x, space);
                           state["bucketX"] -= amount;
                           state["bucketY"] += amount;
                       });

    problem.addCommand("POUR Y X", "Լցնել Y-ից X",
                       [](std::map<std::string, int>& state) {
                           int x = state["bucketX"];
                           int y = state["bucketY"];
                           int space = 2 - x;
                           int amount = std::min(y, space);
                           state["bucketY"] -= amount;
                           state["bucketX"] += amount;
                       });

    // 3L միայն Y դույլում է հնարավոր
    problem.setGoal([](const std::map<std::string, int>& state) {
        return state.at("bucketY") == 3;
    });

    return problem;
}

// Երկրորդ խնդիր: 3L և 5L դույլեր -> 4L
ProblemDefinition createBucketProblem_3_5_4() {
    ProblemDefinition problem("buckets_3_5_4", "Դույլերի խնդիր (3L + 5L -> 4L)");
    problem.description = "Ստացիր 4 լիտր ջուր օգտագործելով 3L և 5L դույլեր";

    problem.addVariable("bucketX", "X դույլ (3L)", 0, 0, 3);
    problem.addVariable("bucketY", "Y դույլ (5L)", 0, 0, 5);

    problem.addCommand("FILL X", "Լցնել X դույլը",
                       [](std::map<std::string, int>& state) {
                           state["bucketX"] = 3;
                       });

    problem.addCommand("FILL Y", "Լցնել Y դույլը",
                       [](std::map<std::string, int>& state) {
                           state["bucketY"] = 5;
                       });

    problem.addCommand("EMPTY X", "Դատարկել X դույլը",
                       [](std::map<std::string, int>& state) {
                           state["bucketX"] = 0;
                       });

    problem.addCommand("EMPTY Y", "Դատարկել Y դույլը",
                       [](std::map<std::string, int>& state) {
                           state["bucketY"] = 0;
                       });

    problem.addCommand("POUR X Y", "Լցնել X-ից Y",
                       [](std::map<std::string, int>& state) {
                           int x = state["bucketX"];
                           int y = state["bucketY"];
                           int space = 5 - y;
                           int amount = std::min(x, space);
                           state["bucketX"] -= amount;
                           state["bucketY"] += amount;
                       });

    problem.addCommand("POUR Y X", "Լցնել Y-ից X",
                       [](std::map<std::string, int>& state) {
                           int x = state["bucketX"];
                           int y = state["bucketY"];
                           int space = 3 - x;
                           int amount = std::min(y, space);
                           state["bucketY"] -= amount;
                           state["bucketX"] += amount;
                       });

    problem.setGoal([](const std::map<std::string, int>& state) {
        return state.at("bucketY") == 4;
    });

    return problem;
}

// Երրորդ խնդիր: 4L և 9L դույլեր -> 6L
ProblemDefinition createBucketProblem_4_9_6() {
    ProblemDefinition problem("buckets_4_9_6", "Դույլերի խնդիր (4L + 9L -> 6L)");
    problem.description = "Ստացիր 6 լիտր ջուր օգտագործելով 4L և 9L դույլեր";

    problem.addVariable("bucketX", "X դույլ (4L)", 0, 0, 4);
    problem.addVariable("bucketY", "Y դույլ (9L)", 0, 0, 9);

    problem.addCommand("FILL X", "Լցնել X դույլը",
                       [](std::map<std::string, int>& state) {
                           state["bucketX"] = 4;
                       });

    problem.addCommand("FILL Y", "Լցնել Y դույլը",
                       [](std::map<std::string, int>& state) {
                           state["bucketY"] = 9;
                       });

    problem.addCommand("EMPTY X", "Դատարկել X դույլը",
                       [](std::map<std::string, int>& state) {
                           state["bucketX"] = 0;
                       });

    problem.addCommand("EMPTY Y", "Դատարկել Y դույլը",
                       [](std::map<std::string, int>& state) {
                           state["bucketY"] = 0;
                       });

    problem.addCommand("POUR X Y", "Լցնել X-ից Y",
                       [](std::map<std::string, int>& state) {
                           int x = state["bucketX"];
                           int y = state["bucketY"];
                           int space = 9 - y;
                           int amount = std::min(x, space);
                           state["bucketX"] -= amount;
                           state["bucketY"] += amount;
                       });

    problem.addCommand("POUR Y X", "Լցնել Y-ից X",
                       [](std::map<std::string, int>& state) {
                           int x = state["bucketX"];
                           int y = state["bucketY"];
                           int space = 4 - x;
                           int amount = std::min(y, space);
                           state["bucketY"] -= amount;
                           state["bucketX"] += amount;
                       });

    problem.setGoal([](const std::map<std::string, int>& state) {
        return state.at("bucketY") == 6;
    });

    return problem;
}
