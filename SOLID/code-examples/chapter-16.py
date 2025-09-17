# FILE: chapter-16.py
# Chapter 16 — Refactoring — Python

from enum import Enum
from typing import NamedTuple

# ❌ BEFORE Refactoring - Poor code structure
class BadCalculator:
    def calc(self, op, a, b):
        if op in ["add", "ADD", "+"]:
            return a + b
        elif op in ["sub", "SUB", "-"]:
            return a - b
        elif op in ["mul", "MUL", "*"]:
            return a * b
        elif op in ["div", "DIV", "/"]:
            if b == 0:
                print("Error: Division by zero!")
                return 0
            return a / b
        else:
            print(f"Unknown operation: {op}")
            return 0

# ✅ AFTER Refactoring - Clean, maintainable code
class Operation(Enum):
    ADD = "add"
    SUBTRACT = "subtract"
    MULTIPLY = "multiply"
    DIVIDE = "divide"

class CalculationResult(NamedTuple):
    value: float
    is_success: bool
    error_message: str = ""
    
    @classmethod
    def success(cls, value):
        return cls(value, True, "")
    
    @classmethod
    def error(cls, message):
        return cls(0, False, message)

class Calculator:
    def __init__(self):
        self._operations = {
            Operation.ADD: lambda a, b: CalculationResult.success(a + b),
            Operation.SUBTRACT: lambda a, b: CalculationResult.success(a - b),
            Operation.MULTIPLY: lambda a, b: CalculationResult.success(a * b),
            Operation.DIVIDE: lambda a, b: (
                CalculationResult.success(a / b) if b != 0 
                else CalculationResult.error("Division by zero")
            )
        }
    
    def calculate(self, operation, a, b):
        return self._operations[operation](a, b)

def main():
    print("🔧 Refactoring Example (Python)")
    print("===============================\n")
    
    # Before refactoring
    print("❌ Before refactoring:")
    bad_calc = BadCalculator()
    print(f"5 + 3 = {bad_calc.calc('add', 5, 3)}")
    print(f"10 / 0 = {bad_calc.calc('div', 10, 0)}")
    
    # After refactoring
    print("\n✅ After refactoring:")
    good_calc = Calculator()
    
    result1 = good_calc.calculate(Operation.ADD, 5, 3)
    print(f"5 + 3 = {result1.value if result1.is_success else result1.error_message}")
    
    result2 = good_calc.calculate(Operation.DIVIDE, 10, 0)
    print(f"10 / 0 = {result2.value if result2.is_success else result2.error_message}")
    
    print("\n💡 Refactoring Benefits:")
    print("   ✓ Better error handling")
    print("   ✓ Type-safe operations")
    print("   ✓ Easier to extend")
    print("   ✓ More testable")

if __name__ == "__main__":
    main()