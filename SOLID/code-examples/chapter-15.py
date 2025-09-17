# FILE: chapter-15-testing-tdd.py
# Chapter 15 — Testing & TDD Basics — Beginner/Intermediate

import unittest
from typing import List, Optional
from dataclasses import dataclass
from datetime import datetime
from enum import Enum
import re

# Example 1: Simple Calculator (Target for Testing)
class Calculator:
    def add(self, a: float, b: float) -> float:
        return a + b

    def subtract(self, a: float, b: float) -> float:
        return a - b

    def multiply(self, a: float, b: float) -> float:
        return a * b

    def divide(self, a: float, b: float) -> float:
        if b == 0:
            raise ZeroDivisionError("Cannot divide by zero")
        return a / b

    def calculate_total(self, numbers: List[float]) -> float:
        if numbers is None:
            raise ValueError("Numbers list cannot be None")
        return sum(numbers)

# Example 2: TDD Shopping Cart Implementation
@dataclass
class ShoppingCartItem:
    name: str
    price: float
    quantity: int

    def __post_init__(self):
        if not self.name or not self.name.strip():
            raise ValueError("Name cannot be empty")
        if self.price < 0:
            raise ValueError("Price cannot be negative")
        if self.quantity <= 0:
            raise ValueError("Quantity must be positive")

    @property
    def total_price(self) -> float:
        return self.price * self.quantity

class ShoppingCart:
    def __init__(self):
        self._items: List[ShoppingCartItem] = []

    @property
    def items(self) -> List[ShoppingCartItem]:
        return self._items.copy()

    def add_item(self, name: str, price: float, quantity: int = 1) -> None:
        item = ShoppingCartItem(name, price, quantity)
        self._items.append(item)

    def get_total(self) -> float:
        return sum(item.total_price for item in self._items)

    def get_item_count(self) -> int:
        return sum(item.quantity for item in self._items)

    def clear(self) -> None:
        self._items.clear()

    def remove_item(self, name: str) -> bool:
        for i, item in enumerate(self._items):
            if item.name.lower() == name.lower():
                del self._items[i]
                return True
        return False

    def has_item(self, name: str) -> bool:
        return any(item.name.lower() == name.lower() for item in self._items)

# Example 3: TDD Password Validator
class ValidationResult:
    def __init__(self):
        self._errors: List[str] = []

    @property
    def is_valid(self) -> bool:
        return len(self._errors) == 0

    @property
    def errors(self) -> List[str]:
        return self._errors.copy()

    def add_error(self, error: str) -> None:
        self._errors.append(error)

    def __str__(self) -> str:
        return "Valid" if self.is_valid else f"Invalid: {', '.join(self._errors)}"

class PasswordValidator:
    MINIMUM_LENGTH = 8
    COMMON_PASSWORDS = [
        "password", "password123", "123456", "qwerty", "abc123", "letmein"
    ]

    def validate_password(self, password: str) -> ValidationResult:
        result = ValidationResult()

        if not password:
            result.add_error("Password cannot be empty")
            return result

        if not self._has_minimum_length(password):
            result.add_error(f"Password must be at least {self.MINIMUM_LENGTH} characters long")

        if not self._has_uppercase_character(password):
            result.add_error("Password must contain at least one uppercase letter")

        if not self._has_lowercase_character(password):
            result.add_error("Password must contain at least one lowercase letter")

        if not self._has_digit(password):
            result.add_error("Password must contain at least one digit")

        if not self._has_special_character(password):
            result.add_error("Password must contain at least one special character (!@#$%^&*)")

        if self._is_common_password(password):
            result.add_error("Password is too common. Please choose a more secure password")

        return result

    def _has_minimum_length(self, password: str) -> bool:
        return len(password) >= self.MINIMUM_LENGTH

    def _has_uppercase_character(self, password: str) -> bool:
        return any(c.isupper() for c in password)

    def _has_lowercase_character(self, password: str) -> bool:
        return any(c.islower() for c in password)

    def _has_digit(self, password: str) -> bool:
        return any(c.isdigit() for c in password)

    def _has_special_character(self, password: str) -> bool:
        return any(c in "!@#$%^&*" for c in password)

    def _is_common_password(self, password: str) -> bool:
        return password.lower() in [common.lower() for common in self.COMMON_PASSWORDS]

# Example 4: Bank Account with TDD
class TransactionType(Enum):
    DEPOSIT = "deposit"
    WITHDRAWAL = "withdrawal"

@dataclass
class Transaction:
    transaction_type: TransactionType
    amount: float
    description: str
    timestamp: datetime

    def __str__(self) -> str:
        return f"{self.timestamp.strftime('%Y-%m-%d %H:%M:%S')} - {self.transaction_type.value.title()}: ${self.amount:.2f} - {self.description}"

class BankAccount:
    def __init__(self, initial_balance: float = 0):
        if initial_balance < 0:
            raise ValueError("Initial balance cannot be negative")
        
        self._balance = initial_balance
        self._transactions: List[Transaction] = []
        
        if initial_balance > 0:
            self._transactions.append(Transaction(
                TransactionType.DEPOSIT, 
                initial_balance, 
                "Initial deposit",
                datetime.now()
            ))

    @property
    def balance(self) -> float:
        return self._balance

    @property
    def transaction_history(self) -> List[Transaction]:
        return self._transactions.copy()

    def deposit(self, amount: float, description: str = "Deposit") -> None:
        if amount <= 0:
            raise ValueError("Deposit amount must be positive")

        self._balance += amount
        self._transactions.append(Transaction(
            TransactionType.DEPOSIT,
            amount,
            description,
            datetime.now()
        ))

    def withdraw(self, amount: float, description: str = "Withdrawal") -> None:
        if amount <= 0:
            raise ValueError("Withdrawal amount must be positive")

        if amount > self._balance:
            raise ValueError("Insufficient funds")

        self._balance -= amount
        self._transactions.append(Transaction(
            TransactionType.WITHDRAWAL,
            amount,
            description,
            datetime.now()
        ))

    def get_total_deposits(self) -> float:
        return sum(t.amount for t in self._transactions if t.transaction_type == TransactionType.DEPOSIT)

    def get_total_withdrawals(self) -> float:
        return sum(t.amount for t in self._transactions if t.transaction_type == TransactionType.WITHDRAWAL)

# Example 5: String Calculator (TDD Kata)
class StringCalculator:
    def add(self, numbers: str) -> int:
        if not numbers:
            return 0

        # Split by comma and newline
        delimiters = [',', '\n']
        for delimiter in delimiters:
            numbers = numbers.replace(delimiter, ',')

        number_strings = numbers.split(',')
        parsed_numbers = []
        negatives = []

        for number_string in number_strings:
            if number_string.strip():
                try:
                    number = int(number_string.strip())
                    if number < 0:
                        negatives.append(number)
                    else:
                        parsed_numbers.append(number)
                except ValueError:
                    pass  # Ignore invalid numbers

        if negatives:
            raise ValueError(f"Negatives not allowed: {', '.join(map(str, negatives))}")

        return sum(parsed_numbers)

# Test Classes using unittest framework
class TestCalculator(unittest.TestCase):
    def setUp(self):
        self.calculator = Calculator()

    def test_add_two_positive_numbers(self):
        result = self.calculator.add(5, 3)
        self.assertEqual(8, result)

    def test_subtract_two_numbers(self):
        result = self.calculator.subtract(5, 3)
        self.assertEqual(2, result)

    def test_multiply_two_numbers(self):
        result = self.calculator.multiply(5, 3)
        self.assertEqual(15, result)

    def test_divide_two_numbers(self):
        result = self.calculator.divide(6, 3)
        self.assertEqual(2, result)

    def test_divide_by_zero_raises_exception(self):
        with self.assertRaises(ZeroDivisionError):
            self.calculator.divide(5, 0)

    def test_calculate_total_of_array(self):
        numbers = [1.0, 2.0, 3.0, 4.0]
        result = self.calculator.calculate_total(numbers)
        self.assertEqual(10.0, result)

    def test_calculate_total_with_none_raises_exception(self):
        with self.assertRaises(ValueError):
            self.calculator.calculate_total(None)

class TestShoppingCart(unittest.TestCase):
    def setUp(self):
        self.cart = ShoppingCart()

    def test_new_cart_is_empty(self):
        self.assertEqual(0, len(self.cart.items))
        self.assertEqual(0, self.cart.get_total())

    def test_add_single_item(self):
        self.cart.add_item("Apple", 1.50)
        self.assertEqual(1, len(self.cart.items))
        self.assertEqual(1.50, self.cart.get_total())

    def test_add_multiple_items(self):
        self.cart.add_item("Apple", 1.50, 2)
        self.cart.add_item("Banana", 0.75, 3)
        self.assertEqual(5.25, self.cart.get_total())
        self.assertEqual(5, self.cart.get_item_count())

    def test_negative_price_raises_exception(self):
        with self.assertRaises(ValueError):
            self.cart.add_item("Invalid", -1.0)

    def test_zero_quantity_raises_exception(self):
        with self.assertRaises(ValueError):
            self.cart.add_item("Invalid", 1.0, 0)

    def test_cart_contains_added_item(self):
        self.cart.add_item("Apple", 1.50)
        self.assertTrue(self.cart.has_item("Apple"))
        self.assertFalse(self.cart.has_item("Orange"))

    def test_remove_item(self):
        self.cart.add_item("Apple", 1.50)
        removed = self.cart.remove_item("Apple")
        self.assertTrue(removed)
        self.assertFalse(self.cart.has_item("Apple"))

class TestPasswordValidator(unittest.TestCase):
    def setUp(self):
        self.validator = PasswordValidator()

    def test_short_password_is_invalid(self):
        result = self.validator.validate_password("short")
        self.assertFalse(result.is_valid)

    def test_password_without_uppercase_is_invalid(self):
        result = self.validator.validate_password("password123!")
        self.assertFalse(result.is_valid)

    def test_password_without_lowercase_is_invalid(self):
        result = self.validator.validate_password("PASSWORD123!")
        self.assertFalse(result.is_valid)

    def test_password_without_digit_is_invalid(self):
        result = self.validator.validate_password("Password!")
        self.assertFalse(result.is_valid)

    def test_password_without_special_character_is_invalid(self):
        result = self.validator.validate_password("Password123")
        self.assertFalse(result.is_valid)

    def test_valid_password_passes_validation(self):
        result = self.validator.validate_password("Password123!")
        self.assertTrue(result.is_valid)

    def test_common_password_is_invalid(self):
        result = self.validator.validate_password("password")
        self.assertFalse(result.is_valid)

class TestBankAccount(unittest.TestCase):
    def test_initial_balance_set_correctly(self):
        account = BankAccount(100)
        self.assertEqual(100, account.balance)

    def test_negative_initial_balance_raises_exception(self):
        with self.assertRaises(ValueError):
            BankAccount(-50)

    def test_deposit_increases_balance(self):
        account = BankAccount(100)
        account.deposit(50)
        self.assertEqual(150, account.balance)

    def test_withdrawal_decreases_balance(self):
        account = BankAccount(100)
        account.withdraw(25)
        self.assertEqual(75, account.balance)

    def test_overdraw_raises_exception(self):
        account = BankAccount(100)
        with self.assertRaises(ValueError):
            account.withdraw(200)

    def test_negative_deposit_raises_exception(self):
        account = BankAccount(100)
        with self.assertRaises(ValueError):
            account.deposit(-10)

    def test_negative_withdrawal_raises_exception(self):
        account = BankAccount(100)
        with self.assertRaises(ValueError):
            account.withdraw(-10)

    def test_transaction_history_tracks_operations(self):
        account = BankAccount(100)
        account.deposit(50)
        account.withdraw(25)
        
        history = account.transaction_history
        self.assertEqual(3, len(history))  # Initial + deposit + withdrawal

class TestStringCalculator(unittest.TestCase):
    def setUp(self):
        self.calculator = StringCalculator()

    def test_empty_string_returns_zero(self):
        result = self.calculator.add("")
        self.assertEqual(0, result)

    def test_single_number(self):
        result = self.calculator.add("1")
        self.assertEqual(1, result)

    def test_two_numbers(self):
        result = self.calculator.add("1,2")
        self.assertEqual(3, result)

    def test_three_numbers(self):
        result = self.calculator.add("1,2,3")
        self.assertEqual(6, result)

    def test_newline_delimiter(self):
        result = self.calculator.add("1\n2,3")
        self.assertEqual(6, result)

    def test_negative_numbers_throw_exception(self):
        with self.assertRaises(ValueError) as context:
            self.calculator.add("1,-2")
        self.assertIn("Negatives not allowed", str(context.exception))

# Simple Test Runner for demo purposes
class SimpleTestRunner:
    def __init__(self):
        self.tests_passed = 0
        self.tests_failed = 0

    def run_all_tests(self):
        print("=== Running Unit Tests ===\n")
        
        test_classes = [
            TestCalculator,
            TestShoppingCart,
            TestPasswordValidator,
            TestBankAccount,
            TestStringCalculator
        ]

        for test_class in test_classes:
            print(f"--- {test_class.__name__} ---")
            suite = unittest.TestLoader().loadTestsFromTestCase(test_class)
            runner = unittest.TextTestRunner(verbosity=0, stream=open('/dev/null', 'w'))
            result = runner.run(suite)
            
            self.tests_passed += result.testsRun - len(result.failures) - len(result.errors)
            self.tests_failed += len(result.failures) + len(result.errors)
            
            for test, _ in result.failures:
                print(f"❌ {test._testMethodName}")
            for test, _ in result.errors:
                print(f"❌ {test._testMethodName}")
                
            successful_tests = result.testsRun - len(result.failures) - len(result.errors)
            for i in range(successful_tests):
                print(f"✅ Test passed")
            print()

    def print_summary(self):
        total = self.tests_passed + self.tests_failed
        success_rate = (self.tests_passed * 100.0 / total) if total > 0 else 0
        
        print("=== Test Summary ===")
        print(f"Passed: {self.tests_passed}")
        print(f"Failed: {self.tests_failed}")
        print(f"Total: {total}")
        print(f"Success Rate: {success_rate:.1f}%")

def demonstrate_tdd_process():
    """Demonstrate the TDD Red-Green-Refactor cycle"""
    print("=== TDD Red-Green-Refactor Demo ===\n")
    
    # Example: Building a simple grade calculator
    print("🔴 RED: Write failing test first")
    print("def test_calculate_grade_average():")
    print("    calculator = GradeCalculator()")
    print("    result = calculator.calculate_average([90, 80, 70])")
    print("    assert result == 80.0")
    print()
    
    print("🟢 GREEN: Write minimal code to pass")
    print("class GradeCalculator:")
    print("    def calculate_average(self, grades):")
    print("        return sum(grades) / len(grades)")
    print()
    
    print("🔵 REFACTOR: Improve the code while keeping tests green")
    print("class GradeCalculator:")
    print("    def calculate_average(self, grades):")
    print("        if not grades:")
    print("            raise ValueError('Grades list cannot be empty')")
    print("        return sum(grades) / len(grades)")
    print()

def main():
    print("=== Testing & TDD Demo ===\n")
    
    # Demonstrate TDD process
    demonstrate_tdd_process()
    
    # Run actual tests
    runner = SimpleTestRunner()
    runner.run_all_tests()
    runner.print_summary()

    print("\n=== TDD Benefits ===")
    print("✓ Catches bugs early in development")
    print("✓ Provides safety net for refactoring")
    print("✓ Documents expected behavior")
    print("✓ Leads to better code design")
    print("✓ Builds confidence in code quality")
    print("✓ Encourages writing testable code")
    print("✓ Helps clarify requirements")

if __name__ == "__main__":
    main()