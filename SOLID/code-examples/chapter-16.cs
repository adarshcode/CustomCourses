// FILE: chapter-16.cs
// Chapter 16 — Refactoring — C#

using System;
using System.Collections.Generic;

// ❌ BEFORE Refactoring - Poor code structure
class BadCalculator
{
    public double calc(string op, double a, double b)
    {
        if (op == "add" || op == "ADD" || op == "+")
        {
            return a + b;
        }
        else if (op == "sub" || op == "SUB" || op == "-")
        {
            return a - b;
        }
        else if (op == "mul" || op == "MUL" || op == "*")
        {
            return a * b;
        }
        else if (op == "div" || op == "DIV" || op == "/")
        {
            if (b == 0) 
            {
                Console.WriteLine("Error: Division by zero!");
                return 0;
            }
            return a / b;
        }
        else
        {
            Console.WriteLine("Unknown operation: " + op);
            return 0;
        }
    }
}

// ✅ AFTER Refactoring - Clean, maintainable code
public enum Operation
{
    Add, Subtract, Multiply, Divide
}

public class CalculationResult
{
    public double Value { get; }
    public bool IsSuccess { get; }
    public string ErrorMessage { get; }
    
    private CalculationResult(double value, bool isSuccess, string errorMessage)
    {
        Value = value;
        IsSuccess = isSuccess;
        ErrorMessage = errorMessage;
    }
    
    public static CalculationResult Success(double value) => new CalculationResult(value, true, null);
    public static CalculationResult Error(string message) => new CalculationResult(0, false, message);
}

public class Calculator
{
    private readonly Dictionary<Operation, Func<double, double, CalculationResult>> operations;
    
    public Calculator()
    {
        operations = new Dictionary<Operation, Func<double, double, CalculationResult>>
        {
            [Operation.Add] = (a, b) => CalculationResult.Success(a + b),
            [Operation.Subtract] = (a, b) => CalculationResult.Success(a - b),
            [Operation.Multiply] = (a, b) => CalculationResult.Success(a * b),
            [Operation.Divide] = (a, b) => b != 0 
                ? CalculationResult.Success(a / b) 
                : CalculationResult.Error("Division by zero")
        };
    }
    
    public CalculationResult Calculate(Operation operation, double a, double b)
    {
        return operations[operation](a, b);
    }
}

class Program
{
    static void Main()
    {
        Console.WriteLine("🔧 Refactoring Example (C#)");
        Console.WriteLine("==========================\n");
        
        // Before refactoring
        Console.WriteLine("❌ Before refactoring:");
        var badCalc = new BadCalculator();
        Console.WriteLine($"5 + 3 = {badCalc.calc("add", 5, 3)}");
        Console.WriteLine($"10 / 0 = {badCalc.calc("div", 10, 0)}");
        
        // After refactoring
        Console.WriteLine("\n✅ After refactoring:");
        var goodCalc = new Calculator();
        
        var result1 = goodCalc.Calculate(Operation.Add, 5, 3);
        Console.WriteLine($"5 + 3 = {(result1.IsSuccess ? result1.Value.ToString() : result1.ErrorMessage)}");
        
        var result2 = goodCalc.Calculate(Operation.Divide, 10, 0);
        Console.WriteLine($"10 / 0 = {(result2.IsSuccess ? result2.Value.ToString() : result2.ErrorMessage)}");
        
        Console.WriteLine("\n💡 Refactoring Benefits:");
        Console.WriteLine("   ✓ Better error handling");
        Console.WriteLine("   ✓ Type-safe operations");
        Console.WriteLine("   ✓ Easier to extend");
        Console.WriteLine("   ✓ More testable");
    }
}