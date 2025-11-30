#include "interpreter.h"
#include <iostream>
#include <algorithm>
using namespace std;

Interpreter::Interpreter(ProblemDefinition* prob) : problem(prob) {
    for (const auto& var : problem->variables) {
        state[var.name] = var.initialValue;
    }
    history.push_back(state);
}

bool Interpreter::executeCommand(const string& commandName) {
    auto it = find_if(problem->commands.begin(), problem->commands.end(),
                      [&commandName](const CommandDefinition& cmd) {
                          return cmd.name == commandName;
                      });

    if (it != problem->commands.end()) {
        it->action(state);
        history.push_back(state);
        return true;
    }

    cerr << "Unknown command: " << commandName << endl;
    return false;
}

bool Interpreter::isSolved() const {
    return problem->goalChecker(state);
}

map<string, int> Interpreter::getState() const {
    return state;
}

const vector<map<string, int>>& Interpreter::getHistory() const {
    return history;
}

void Interpreter::visualizeState() const {
    for (const auto& var : problem->variables) {
        int value = state.at(var.name);
        cout << var.displayName << " [" << value << "/" << var.maxValue << "]: ";
        for (int i = 0; i < value; i++) {
            cout << "#";
        }
        cout << endl;
    }
}
