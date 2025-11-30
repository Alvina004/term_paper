#include "problem_definition.h"
#include <algorithm>

ProblemDefinition createBucketProblem() {
    ProblemDefinition problem("buckets_2_7_3", "Դույլերի խնդիր");
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

    problem.setGoal([](const std::map<std::string, int>& state) {
        return state.at("bucketX") == 3 || state.at("bucketY") == 3;
    });

    return problem;
}
