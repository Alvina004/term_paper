#pragma once
#include "problem_definition.h"
#include <map>
#include <memory>
using namespace std;

class ProblemRegistry {
private:
    map<string, shared_ptr<ProblemDefinition>> problems;

    ProblemRegistry() = default;

public:
    static ProblemRegistry& instance() {
        static ProblemRegistry registry;
        return registry;
    }

    void registerProblem(const ProblemDefinition& problem);
    ProblemDefinition* getProblem(const string& id);
    vector<string> getAllProblemIds() const;
};
