#include "problem_registry.h"
#include <memory>
using namespace std;

void ProblemRegistry::registerProblem(const ProblemDefinition& problem) {
    problems[problem.id] = make_shared<ProblemDefinition>(problem);
}

ProblemDefinition* ProblemRegistry::getProblem(const string& id) {
    auto it = problems.find(id);
    return (it != problems.end()) ? it->second.get() : nullptr;
}

vector<string> ProblemRegistry::getAllProblemIds() const {
    vector<string> ids;
    for (const auto& [id, _] : problems) {
        ids.push_back(id);
    }
    return ids;
}
