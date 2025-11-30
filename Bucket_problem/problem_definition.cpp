#include "problem_definition.h"
using namespace std;

ProblemDefinition::ProblemDefinition(const string& id, const string& title)
    : id(id), title(title) {}

ProblemDefinition::ProblemDefinition(const ProblemDefinition& other)
    : id(other.id), title(other.title), description(other.description),
    variables(other.variables), commands(other.commands),
    goalChecker(other.goalChecker) {}

void ProblemDefinition::addVariable(const string& name, const string& display,
                                    int initial, int min, int max) {
    variables.push_back({ name, display, initial, min, max });
}

void ProblemDefinition::addCommand(const string& name, const string& desc,
                                   function<void(map<string, int>&)> action) {
    commands.push_back({ name, desc, action });
}

void ProblemDefinition::setGoal(function<bool(const map<string, int>&)> checker) {
    goalChecker = checker;
}
