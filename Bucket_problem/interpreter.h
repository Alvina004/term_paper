#pragma once
#include "problem_definition.h"
#include <vector>
using namespace std;

class Interpreter {
private:
    ProblemDefinition* problem;
    map<string, int> state;
    vector<map<string, int>> history;

public:
    Interpreter(ProblemDefinition* prob);

    bool executeCommand(const string& commandName);
    bool isSolved() const;
    map<string, int> getState() const;
    const vector<map<string, int>>& getHistory() const;
    void visualizeState() const;
};
