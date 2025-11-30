#pragma once
#include <string>
#include <vector>
#include <map>
#include <functional>
using namespace std;

struct Variable {
    string name;
    string displayName;
    int initialValue;
    int minValue;
    int maxValue;
};

struct CommandDefinition {
    string name;
    string description;
    function<void(map<string, int>&)> action;
};

class ProblemDefinition {
public:
    string id;
    string title;
    string description;

    vector<Variable> variables;
    vector<CommandDefinition> commands;
    function<bool(const map<string, int>&)> goalChecker;

    ProblemDefinition(const string& id, const string& title);

    ProblemDefinition(const ProblemDefinition& other);

    void addVariable(const string& name, const string& display,
                     int initial, int min, int max);

    void addCommand(const string& name, const string& desc,
                    function<void(map<string, int>&)> action);

    void setGoal(function<bool(const map<string, int>&)> checker);
};
