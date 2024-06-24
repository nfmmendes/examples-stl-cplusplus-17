#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <cassert>
#include <limits>
using std::cout;
using std::endl;
using std::is_base_of;
using std::is_same;
using std::map;
using std::numeric_limits;
using std::string;
using std::vector;

enum VariableType
{
    CONTINUOUS,
    INTEGER,
    BINARY
};

class Variable
{
public:
    Variable(const string &varName, VariableType varType, double varLowerBound, double varUpperBound) : name(varName), type(varType), lowerBound(varLowerBound), upperBound(varUpperBound)
    {
    }

    Variable(const string &varName, const VariableType varType) : name(varName), type(varType)
    {
        if (varType == VariableType::INTEGER)
        {
            lowerBound = numeric_limits<int>::min();
            upperBound = numeric_limits<int>::max();
        }
        else if (varType == VariableType::CONTINUOUS)
        {
            lowerBound = numeric_limits<double>::min();
            upperBound = numeric_limits<double>::max();
        }
        else if (varType == VariableType::BINARY)
        {
            lowerBound = 0;
            lowerBound = 1;
        }
    }

    double getValue() const { return value; }
    void setValue(double varValue) { value = varValue; }

private:
    double lowerBound;
    double upperBound;
    string name;
    VariableType type;
    double value;
};

class IntegerVariable : public Variable
{
public:
    IntegerVariable(const string &name, double lowerBound, double upperBound) : Variable(name, VariableType::INTEGER, lowerBound, upperBound)
    {
    }

    IntegerVariable(const string &name) : Variable(name, VariableType::INTEGER)
    {
    }
};

class ContinuousVariable : public Variable
{
public:
    ContinuousVariable(const string &name, double lowerBound, double upperBound) : Variable(name, VariableType::CONTINUOUS, lowerBound, upperBound)
    {
    }

    ContinuousVariable(const string &name) : Variable(name, VariableType::CONTINUOUS)
    {
    }
};

class BinaryVariable : public Variable
{
public:
    BinaryVariable(const string &name) : Variable(name, VariableType::BINARY)
    {
    }
};

class SemiIntegerVariable : public Variable
{
public:
    SemiIntegerVariable(const string &name, double varLowerBound, double varUpperBound) : Variable(name, VariableType::INTEGER, varLowerBound, varUpperBound) {}
};

class AssignmentVariable : public BinaryVariable
{
public:
    AssignmentVariable(const string &name) : BinaryVariable(name) {}
};

class FlowVariable : public ContinuousVariable
{
public:
    FlowVariable(const string &name, double lowerBound, double upperBound) : ContinuousVariable(name, lowerBound, upperBound) {}
};

class DemandVariable : public IntegerVariable
{
public:
    DemandVariable(const string &name, int lowerBound, int upperBound) : IntegerVariable(name, lowerBound, upperBound) {}
};

template <class T = ContinuousVariable> // Template initialization was not allowed before C++17
T *variableFactory(const string &name, double lowerBound = 0, double upperBound = 0)
{
    // Defining a if as constexpress is a new feature of C++17
    if constexpr (is_same<T, ContinuousVariable>::value || is_base_of<ContinuousVariable, T>::value)
    {
        return new T(name, lowerBound, upperBound);
    }
    else if constexpr (is_same<T, IntegerVariable>::value || is_base_of<IntegerVariable, T>::value)
    {
        return new T(name, static_cast<int>(lowerBound), static_cast<int>(upperBound));
    }
    else if constexpr (is_same<T, BinaryVariable>::value || is_base_of<BinaryVariable, T>::value)
    {
        return new T(name);
    }
    else if constexpr (is_base_of<Variable, T>::value)
    {
        return new T(name, lowerBound, upperBound);
    }

    return nullptr;
}

int main()
{
    // Insert delete's
    auto integerVariable = variableFactory<IntegerVariable>("integer", 3, 6);
    auto continuousVariable = variableFactory<ContinuousVariable>("continuous", 1.5, 12);
    auto binaryVariable = variableFactory<BinaryVariable>("Binary");
    auto assignmentVariable = variableFactory<AssignmentVariable>("Assignment");
    auto flowVariable = variableFactory<FlowVariable>("Flow", -5.0, 3.5);
    auto demandVariable = variableFactory<DemandVariable>("Demand", 1, 10);
    auto semiIntegerVariable = variableFactory<SemiIntegerVariable>("Semi-integer", 1.0, 9);
    auto nonValidTypeVariable = variableFactory<int>("Wrong");

    assert(nonValidTypeVariable == nullptr);

    delete integerVariable;
    delete continuousVariable;
    delete binaryVariable;
    delete assignmentVariable;
    delete flowVariable;
    delete demandVariable;
    delete semiIntegerVariable;
    delete nonValidTypeVariable;

    return 0;
}