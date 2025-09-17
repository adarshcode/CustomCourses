// FILE: chapter-16.cpp
// Chapter 16 — Refactoring — C++

#include <iostream>
#include <string>
#include <unordered_map>
#include <functional>
using namespace std;

// ❌ BEFORE Refactoring - Poor code structure
class BadCalculator {
public:
    double calc(const string& op, double a, double b) {
        if (op == "add" || op == "ADD" || op == "+") {
            return a + b;
        } else if (op == "sub" || op == "SUB" || op == "-") {
            return a - b;
        } else if (op == "mul" || op == "MUL" || op == "*") {
            return a * b;
        } else if (op == "div" || op == "DIV" || op == "/") {
            if (b == 0) {
                cout << "Error: Division by zero!" << endl;
                return 0;
            }
            return a / b;
        } else {
            cout << "Unknown operation: " << op << endl;
            return 0;
        }
    }
};

// ✅ AFTER Refactoring - Clean, maintainable code
enum class Operation {
    ADD, SUBTRACT, MULTIPLY, DIVIDE
};

struct CalculationResult {
    double value;
    bool isSuccess;
    string errorMessage;
    
    static CalculationResult success(double val) {
        return {val, true, ""};
    }
    
    static CalculationResult error(const string& msg) {
        return {0, false, msg};
    }
};

class Calculator {
private:
    unordered_map<Operation, function<CalculationResult(double, double)>> operations;
    
public:
    Calculator() {
        operations[Operation::ADD] = [](double a, double b) {
            return CalculationResult::success(a + b);
        };
        operations[Operation::SUBTRACT] = [](double a, double b) {
            return CalculationResult::success(a - b);
        };
        operations[Operation::MULTIPLY] = [](double a, double b) {
            return CalculationResult::success(a * b);
        };
        operations[Operation::DIVIDE] = [](double a, double b) {
            return b != 0 
                ? CalculationResult::success(a / b)
                : CalculationResult::error("Division by zero");
        };
    }
    
    CalculationResult calculate(Operation operation, double a, double b) {
        return operations[operation](a, b);
    }
};

int main() {
    cout << "🔧 Refactoring Example (C++)" << endl;
    cout << "============================" << endl << endl;
    
    // Before refactoring
    cout << "❌ Before refactoring:" << endl;
    BadCalculator badCalc;
    cout << "5 + 3 = " << badCalc.calc("add", 5, 3) << endl;
    cout << "10 / 0 = " << badCalc.calc("div", 10, 0) << endl;
    
    // After refactoring
    cout << endl << "✅ After refactoring:" << endl;
    Calculator goodCalc;
    
    auto result1 = goodCalc.calculate(Operation::ADD, 5, 3);
    cout << "5 + 3 = " << (result1.isSuccess ? to_string(result1.value) : result1.errorMessage) << endl;
    
    auto result2 = goodCalc.calculate(Operation::DIVIDE, 10, 0);
    cout << "10 / 0 = " << (result2.isSuccess ? to_string(result2.value) : result2.errorMessage) << endl;
    
    cout << endl << "💡 Refactoring Benefits:" << endl;
    cout << "   ✓ Better error handling" << endl;
    cout << "   ✓ Type-safe operations" << endl;
    cout << "   ✓ Easier to extend" << endl;
    cout << "   ✓ More testable" << endl;
    
    return 0;
}