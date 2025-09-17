// FILE: chapter-15-testing-tdd.cs
// Chapter 15 — Testing & TDD Basics — Beginner/Intermediate

using System;
using System.Collections.Generic;
using System.Linq;

namespace TestingTDDExample
{
    // Example 1: Simple Calculator (Target for Testing)
    public class Calculator
    {
        public double Add(double a, double b)
        {
            return a + b;
        }

        public double Subtract(double a, double b)
        {
            return a - b;
        }

        public double Multiply(double a, double b)
        {
            return a * b;
        }

        public double Divide(double a, double b)
        {
            if (b == 0)
                throw new DivideByZeroException("Cannot divide by zero");
            return a / b;
        }

        public double CalculateTotal(IEnumerable<double> numbers)
        {
            if (numbers == null)
                throw new ArgumentNullException(nameof(numbers));
            
            return numbers.Sum();
        }
    }

    // Example 2: TDD Shopping Cart Implementation
    public class ShoppingCartItem
    {
        public string Name { get; }
        public decimal Price { get; }
        public int Quantity { get; }

        public ShoppingCartItem(string name, decimal price, int quantity)
        {
            if (string.IsNullOrWhiteSpace(name))
                throw new ArgumentException("Name cannot be empty", nameof(name));
            if (price < 0)
                throw new ArgumentException("Price cannot be negative", nameof(price));
            if (quantity <= 0)
                throw new ArgumentException("Quantity must be positive", nameof(quantity));

            Name = name;
            Price = price;
            Quantity = quantity;
        }

        public decimal TotalPrice => Price * Quantity;
    }

    public class ShoppingCart
    {
        private readonly List<ShoppingCartItem> items = new List<ShoppingCartItem>();

        public IReadOnlyList<ShoppingCartItem> Items => items.AsReadOnly();

        public void AddItem(string name, decimal price, int quantity = 1)
        {
            var item = new ShoppingCartItem(name, price, quantity);
            items.Add(item);
        }

        public decimal GetTotal()
        {
            return items.Sum(item => item.TotalPrice);
        }

        public int GetItemCount()
        {
            return items.Sum(item => item.Quantity);
        }

        public void Clear()
        {
            items.Clear();
        }

        public bool RemoveItem(string name)
        {
            var item = items.FirstOrDefault(i => i.Name.Equals(name, StringComparison.OrdinalIgnoreCase));
            if (item != null)
            {
                items.Remove(item);
                return true;
            }
            return false;
        }

        public bool HasItem(string name)
        {
            return items.Any(i => i.Name.Equals(name, StringComparison.OrdinalIgnoreCase));
        }
    }

    // Example 3: TDD Password Validator
    public class PasswordValidator
    {
        private const int MINIMUM_LENGTH = 8;
        private static readonly string[] COMMON_PASSWORDS = {
            "password", "password123", "123456", "qwerty", "abc123", "letmein"
        };

        public ValidationResult ValidatePassword(string password)
        {
            var result = new ValidationResult();

            if (string.IsNullOrEmpty(password))
            {
                result.AddError("Password cannot be empty");
                return result;
            }

            if (!HasMinimumLength(password))
                result.AddError($"Password must be at least {MINIMUM_LENGTH} characters long");

            if (!HasUppercaseCharacter(password))
                result.AddError("Password must contain at least one uppercase letter");

            if (!HasLowercaseCharacter(password))
                result.AddError("Password must contain at least one lowercase letter");

            if (!HasDigit(password))
                result.AddError("Password must contain at least one digit");

            if (!HasSpecialCharacter(password))
                result.AddError("Password must contain at least one special character (!@#$%^&*)");

            if (IsCommonPassword(password))
                result.AddError("Password is too common. Please choose a more secure password");

            return result;
        }

        private bool HasMinimumLength(string password) => password.Length >= MINIMUM_LENGTH;
        private bool HasUppercaseCharacter(string password) => password.Any(char.IsUpper);
        private bool HasLowercaseCharacter(string password) => password.Any(char.IsLower);
        private bool HasDigit(string password) => password.Any(char.IsDigit);
        private bool HasSpecialCharacter(string password) => password.Any(c => "!@#$%^&*".Contains(c));
        private bool IsCommonPassword(string password) => 
            COMMON_PASSWORDS.Any(common => string.Equals(common, password, StringComparison.OrdinalIgnoreCase));
    }

    public class ValidationResult
    {
        private readonly List<string> errors = new List<string>();

        public bool IsValid => errors.Count == 0;
        public IReadOnlyList<string> Errors => errors.AsReadOnly();

        public void AddError(string error)
        {
            errors.Add(error);
        }

        public override string ToString()
        {
            return IsValid ? "Valid" : $"Invalid: {string.Join(", ", errors)}";
        }
    }

    // Example 4: Bank Account with TDD
    public class BankAccount
    {
        private decimal balance;
        private readonly List<Transaction> transactions = new List<Transaction>();

        public decimal Balance => balance;
        public IReadOnlyList<Transaction> TransactionHistory => transactions.AsReadOnly();

        public BankAccount(decimal initialBalance = 0)
        {
            if (initialBalance < 0)
                throw new ArgumentException("Initial balance cannot be negative", nameof(initialBalance));
            
            balance = initialBalance;
            if (initialBalance > 0)
            {
                transactions.Add(new Transaction(TransactionType.Deposit, initialBalance, "Initial deposit"));
            }
        }

        public void Deposit(decimal amount, string description = "Deposit")
        {
            if (amount <= 0)
                throw new ArgumentException("Deposit amount must be positive", nameof(amount));

            balance += amount;
            transactions.Add(new Transaction(TransactionType.Deposit, amount, description));
        }

        public void Withdraw(decimal amount, string description = "Withdrawal")
        {
            if (amount <= 0)
                throw new ArgumentException("Withdrawal amount must be positive", nameof(amount));

            if (amount > balance)
                throw new InvalidOperationException("Insufficient funds");

            balance -= amount;
            transactions.Add(new Transaction(TransactionType.Withdrawal, amount, description));
        }

        public decimal GetTotalDeposits()
        {
            return transactions
                .Where(t => t.Type == TransactionType.Deposit)
                .Sum(t => t.Amount);
        }

        public decimal GetTotalWithdrawals()
        {
            return transactions
                .Where(t => t.Type == TransactionType.Withdrawal)
                .Sum(t => t.Amount);
        }
    }

    public enum TransactionType
    {
        Deposit,
        Withdrawal
    }

    public class Transaction
    {
        public TransactionType Type { get; }
        public decimal Amount { get; }
        public string Description { get; }
        public DateTime Timestamp { get; }

        public Transaction(TransactionType type, decimal amount, string description)
        {
            Type = type;
            Amount = amount;
            Description = description ?? string.Empty;
            Timestamp = DateTime.Now;
        }

        public override string ToString()
        {
            return $"{Timestamp:yyyy-MM-dd HH:mm:ss} - {Type}: ${Amount:F2} - {Description}";
        }
    }

    // Example 5: String Calculator (TDD Kata)
    public class StringCalculator
    {
        public int Add(string numbers)
        {
            if (string.IsNullOrEmpty(numbers))
                return 0;

            var delimiters = new[] { ',', '\n' };
            var numberStrings = numbers.Split(delimiters, StringSplitOptions.RemoveEmptyEntries);

            var parsedNumbers = new List<int>();
            var negatives = new List<int>();

            foreach (var numberString in numberStrings)
            {
                if (int.TryParse(numberString.Trim(), out int number))
                {
                    if (number < 0)
                        negatives.Add(number);
                    else
                        parsedNumbers.Add(number);
                }
            }

            if (negatives.Any())
            {
                throw new ArgumentException($"Negatives not allowed: {string.Join(", ", negatives)}");
            }

            return parsedNumbers.Sum();
        }
    }

    // Simple Test Runner (for demonstration - normally you'd use a testing framework)
    public class SimpleTestRunner
    {
        private int testsPassed = 0;
        private int testsFailed = 0;

        public void Assert(bool condition, string testName)
        {
            if (condition)
            {
                Console.WriteLine($"✅ {testName}");
                testsPassed++;
            }
            else
            {
                Console.WriteLine($"❌ {testName}");
                testsFailed++;
            }
        }

        public void AssertEqual<T>(T expected, T actual, string testName)
        {
            bool isEqual = EqualityComparer<T>.Default.Equals(expected, actual);
            if (isEqual)
            {
                Console.WriteLine($"✅ {testName}");
                testsPassed++;
            }
            else
            {
                Console.WriteLine($"❌ {testName} - Expected: {expected}, Actual: {actual}");
                testsFailed++;
            }
        }

        public void AssertThrows<TException>(Action action, string testName) where TException : Exception
        {
            try
            {
                action();
                Console.WriteLine($"❌ {testName} - Expected exception was not thrown");
                testsFailed++;
            }
            catch (TException)
            {
                Console.WriteLine($"✅ {testName}");
                testsPassed++;
            }
            catch (Exception ex)
            {
                Console.WriteLine($"❌ {testName} - Wrong exception type: {ex.GetType().Name}");
                testsFailed++;
            }
        }

        public void PrintSummary()
        {
            Console.WriteLine($"\n=== Test Summary ===");
            Console.WriteLine($"Passed: {testsPassed}");
            Console.WriteLine($"Failed: {testsFailed}");
            Console.WriteLine($"Total: {testsPassed + testsFailed}");
            Console.WriteLine($"Success Rate: {(testsPassed * 100.0 / (testsPassed + testsFailed)):F1}%");
        }
    }

    // Demo Program
    class Program
    {
        static void Main()
        {
            Console.WriteLine("=== Testing & TDD Demo ===\n");

            var testRunner = new SimpleTestRunner();

            // Test Calculator
            Console.WriteLine("--- Calculator Tests ---");
            TestCalculator(testRunner);

            // Test Shopping Cart  
            Console.WriteLine("\n--- Shopping Cart Tests ---");
            TestShoppingCart(testRunner);

            // Test Password Validator
            Console.WriteLine("\n--- Password Validator Tests ---");
            TestPasswordValidator(testRunner);

            // Test Bank Account
            Console.WriteLine("\n--- Bank Account Tests ---");
            TestBankAccount(testRunner);

            // Test String Calculator
            Console.WriteLine("\n--- String Calculator Tests ---");
            TestStringCalculator(testRunner);

            testRunner.PrintSummary();

            Console.WriteLine("\n=== TDD Benefits ===");
            Console.WriteLine("✓ Catches bugs early in development");
            Console.WriteLine("✓ Provides safety net for refactoring");
            Console.WriteLine("✓ Documents expected behavior");
            Console.WriteLine("✓ Leads to better code design");
            Console.WriteLine("✓ Builds confidence in code quality");
        }

        static void TestCalculator(SimpleTestRunner testRunner)
        {
            var calculator = new Calculator();

            testRunner.AssertEqual(8.0, calculator.Add(5, 3), "Add two positive numbers");
            testRunner.AssertEqual(2.0, calculator.Subtract(5, 3), "Subtract two numbers");
            testRunner.AssertEqual(15.0, calculator.Multiply(5, 3), "Multiply two numbers");
            testRunner.AssertEqual(2.0, calculator.Divide(6, 3), "Divide two numbers");
            
            testRunner.AssertThrows<DivideByZeroException>(() => calculator.Divide(5, 0), "Divide by zero throws exception");
            
            var numbers = new[] { 1.0, 2.0, 3.0, 4.0 };
            testRunner.AssertEqual(10.0, calculator.CalculateTotal(numbers), "Calculate total of array");
        }

        static void TestShoppingCart(SimpleTestRunner testRunner)
        {
            var cart = new ShoppingCart();

            testRunner.AssertEqual(0, cart.Items.Count, "New cart is empty");
            testRunner.AssertEqual(0m, cart.GetTotal(), "Empty cart total is zero");

            cart.AddItem("Apple", 1.50m, 2);
            testRunner.AssertEqual(1, cart.Items.Count, "Cart has one item type");
            testRunner.AssertEqual(3.00m, cart.GetTotal(), "Cart total with one item");
            testRunner.AssertEqual(2, cart.GetItemCount(), "Cart item count");

            cart.AddItem("Banana", 0.75m, 3);
            testRunner.AssertEqual(5.25m, cart.GetTotal(), "Cart total with multiple items");

            testRunner.Assert(cart.HasItem("Apple"), "Cart contains added item");
            testRunner.Assert(!cart.HasItem("Orange"), "Cart doesn't contain non-added item");
        }

        static void TestPasswordValidator(SimpleTestRunner testRunner)
        {
            var validator = new PasswordValidator();

            var result = validator.ValidatePassword("short");
            testRunner.Assert(!result.IsValid, "Short password is invalid");

            result = validator.ValidatePassword("password123");
            testRunner.Assert(!result.IsValid, "Password without uppercase is invalid");

            result = validator.ValidatePassword("Password123!");
            testRunner.Assert(result.IsValid, "Valid password passes validation");

            result = validator.ValidatePassword("password");
            testRunner.Assert(!result.IsValid, "Common password is invalid");
        }

        static void TestBankAccount(SimpleTestRunner testRunner)
        {
            var account = new BankAccount(100m);
            testRunner.AssertEqual(100m, account.Balance, "Initial balance set correctly");

            account.Deposit(50m);
            testRunner.AssertEqual(150m, account.Balance, "Balance after deposit");

            account.Withdraw(25m);
            testRunner.AssertEqual(125m, account.Balance, "Balance after withdrawal");

            testRunner.AssertThrows<InvalidOperationException>(() => account.Withdraw(200m), "Overdraw throws exception");
            testRunner.AssertThrows<ArgumentException>(() => account.Deposit(-10m), "Negative deposit throws exception");
        }

        static void TestStringCalculator(SimpleTestRunner testRunner)
        {
            var calculator = new StringCalculator();

            testRunner.AssertEqual(0, calculator.Add(""), "Empty string returns zero");
            testRunner.AssertEqual(1, calculator.Add("1"), "Single number");
            testRunner.AssertEqual(3, calculator.Add("1,2"), "Two numbers");
            testRunner.AssertEqual(6, calculator.Add("1,2,3"), "Three numbers");
            testRunner.AssertEqual(6, calculator.Add("1\n2,3"), "New line delimiter");

            testRunner.AssertThrows<ArgumentException>(() => calculator.Add("1,-2"), "Negative numbers throw exception");
        }
    }
}