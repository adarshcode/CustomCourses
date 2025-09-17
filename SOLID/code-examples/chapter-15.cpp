// FILE: chapter-15-testing-tdd.cpp
// Chapter 15 — Testing & TDD Basics — Beginner/Intermediate

#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <numeric>
#include <algorithm>
#include <memory>
#include <sstream>
#include <regex>
#include <ctime>
#include <iomanip>

// Example 1: Simple Calculator (Target for Testing)
class Calculator {
public:
    double add(double a, double b) {
        return a + b;
    }

    double subtract(double a, double b) {
        return a - b;
    }

    double multiply(double a, double b) {
        return a * b;
    }

    double divide(double a, double b) {
        if (b == 0) {
            throw std::runtime_error("Cannot divide by zero");
        }
        return a / b;
    }

    double calculateTotal(const std::vector<double>& numbers) {
        return std::accumulate(numbers.begin(), numbers.end(), 0.0);
    }
};

// Example 2: TDD Shopping Cart Implementation
class ShoppingCartItem {
private:
    std::string name;
    double price;
    int quantity;

public:
    ShoppingCartItem(const std::string& name, double price, int quantity)
        : name(name), price(price), quantity(quantity) {
        if (name.empty()) {
            throw std::invalid_argument("Name cannot be empty");
        }
        if (price < 0) {
            throw std::invalid_argument("Price cannot be negative");
        }
        if (quantity <= 0) {
            throw std::invalid_argument("Quantity must be positive");
        }
    }

    const std::string& getName() const { return name; }
    double getPrice() const { return price; }
    int getQuantity() const { return quantity; }
    double getTotalPrice() const { return price * quantity; }
};

class ShoppingCart {
private:
    std::vector<std::unique_ptr<ShoppingCartItem>> items;

public:
    void addItem(const std::string& name, double price, int quantity = 1) {
        items.push_back(std::make_unique<ShoppingCartItem>(name, price, quantity));
    }

    double getTotal() const {
        double total = 0.0;
        for (const auto& item : items) {
            total += item->getTotalPrice();
        }
        return total;
    }

    int getItemCount() const {
        int count = 0;
        for (const auto& item : items) {
            count += item->getQuantity();
        }
        return count;
    }

    size_t getItemTypes() const {
        return items.size();
    }

    void clear() {
        items.clear();
    }

    bool hasItem(const std::string& name) const {
        return std::any_of(items.begin(), items.end(),
            [&name](const auto& item) {
                return item->getName() == name;
            });
    }

    bool removeItem(const std::string& name) {
        auto it = std::find_if(items.begin(), items.end(),
            [&name](const auto& item) {
                return item->getName() == name;
            });
        
        if (it != items.end()) {
            items.erase(it);
            return true;
        }
        return false;
    }
};

// Example 3: TDD Password Validator
class ValidationResult {
private:
    std::vector<std::string> errors;

public:
    bool isValid() const {
        return errors.empty();
    }

    const std::vector<std::string>& getErrors() const {
        return errors;
    }

    void addError(const std::string& error) {
        errors.push_back(error);
    }

    std::string toString() const {
        if (isValid()) {
            return "Valid";
        }
        
        std::string result = "Invalid: ";
        for (size_t i = 0; i < errors.size(); ++i) {
            if (i > 0) result += ", ";
            result += errors[i];
        }
        return result;
    }
};

class PasswordValidator {
private:
    static const int MINIMUM_LENGTH = 8;
    static const std::vector<std::string> COMMON_PASSWORDS;

    bool hasMinimumLength(const std::string& password) const {
        return password.length() >= MINIMUM_LENGTH;
    }

    bool hasUppercaseCharacter(const std::string& password) const {
        return std::any_of(password.begin(), password.end(), [](char c) {
            return std::isupper(c);
        });
    }

    bool hasLowercaseCharacter(const std::string& password) const {
        return std::any_of(password.begin(), password.end(), [](char c) {
            return std::islower(c);
        });
    }

    bool hasDigit(const std::string& password) const {
        return std::any_of(password.begin(), password.end(), [](char c) {
            return std::isdigit(c);
        });
    }

    bool hasSpecialCharacter(const std::string& password) const {
        std::string specialChars = "!@#$%^&*";
        return std::any_of(password.begin(), password.end(), [&specialChars](char c) {
            return specialChars.find(c) != std::string::npos;
        });
    }

    bool isCommonPassword(const std::string& password) const {
        std::string lowerPassword = password;
        std::transform(lowerPassword.begin(), lowerPassword.end(), lowerPassword.begin(), ::tolower);
        
        return std::any_of(COMMON_PASSWORDS.begin(), COMMON_PASSWORDS.end(),
            [&lowerPassword](const std::string& common) {
                return lowerPassword == common;
            });
    }

public:
    ValidationResult validatePassword(const std::string& password) const {
        ValidationResult result;

        if (password.empty()) {
            result.addError("Password cannot be empty");
            return result;
        }

        if (!hasMinimumLength(password)) {
            result.addError("Password must be at least " + std::to_string(MINIMUM_LENGTH) + " characters long");
        }

        if (!hasUppercaseCharacter(password)) {
            result.addError("Password must contain at least one uppercase letter");
        }

        if (!hasLowercaseCharacter(password)) {
            result.addError("Password must contain at least one lowercase letter");
        }

        if (!hasDigit(password)) {
            result.addError("Password must contain at least one digit");
        }

        if (!hasSpecialCharacter(password)) {
            result.addError("Password must contain at least one special character (!@#$%^&*)");
        }

        if (isCommonPassword(password)) {
            result.addError("Password is too common. Please choose a more secure password");
        }

        return result;
    }
};

const std::vector<std::string> PasswordValidator::COMMON_PASSWORDS = {
    "password", "password123", "123456", "qwerty", "abc123", "letmein"
};

// Example 4: Bank Account with TDD
enum class TransactionType {
    DEPOSIT,
    WITHDRAWAL
};

struct Transaction {
    TransactionType type;
    double amount;
    std::string description;
    std::time_t timestamp;

    Transaction(TransactionType type, double amount, const std::string& description)
        : type(type), amount(amount), description(description) {
        timestamp = std::time(nullptr);
    }

    std::string toString() const {
        std::stringstream ss;
        ss << std::put_time(std::localtime(&timestamp), "%Y-%m-%d %H:%M:%S");
        ss << " - " << (type == TransactionType::DEPOSIT ? "Deposit" : "Withdrawal");
        ss << ": $" << std::fixed << std::setprecision(2) << amount;
        ss << " - " << description;
        return ss.str();
    }
};

class BankAccount {
private:
    double balance;
    std::vector<Transaction> transactions;

public:
    explicit BankAccount(double initialBalance = 0) : balance(initialBalance) {
        if (initialBalance < 0) {
            throw std::invalid_argument("Initial balance cannot be negative");
        }
        
        if (initialBalance > 0) {
            transactions.emplace_back(TransactionType::DEPOSIT, initialBalance, "Initial deposit");
        }
    }

    double getBalance() const {
        return balance;
    }

    const std::vector<Transaction>& getTransactionHistory() const {
        return transactions;
    }

    void deposit(double amount, const std::string& description = "Deposit") {
        if (amount <= 0) {
            throw std::invalid_argument("Deposit amount must be positive");
        }

        balance += amount;
        transactions.emplace_back(TransactionType::DEPOSIT, amount, description);
    }

    void withdraw(double amount, const std::string& description = "Withdrawal") {
        if (amount <= 0) {
            throw std::invalid_argument("Withdrawal amount must be positive");
        }

        if (amount > balance) {
            throw std::runtime_error("Insufficient funds");
        }

        balance -= amount;
        transactions.emplace_back(TransactionType::WITHDRAWAL, amount, description);
    }

    double getTotalDeposits() const {
        double total = 0;
        for (const auto& transaction : transactions) {
            if (transaction.type == TransactionType::DEPOSIT) {
                total += transaction.amount;
            }
        }
        return total;
    }

    double getTotalWithdrawals() const {
        double total = 0;
        for (const auto& transaction : transactions) {
            if (transaction.type == TransactionType::WITHDRAWAL) {
                total += transaction.amount;
            }
        }
        return total;
    }
};

// Example 5: String Calculator (TDD Kata)
class StringCalculator {
public:
    int add(const std::string& numbers) {
        if (numbers.empty()) {
            return 0;
        }

        std::vector<int> parsedNumbers;
        std::vector<int> negatives;

        // Replace newlines with commas for uniform parsing
        std::string normalizedNumbers = numbers;
        std::replace(normalizedNumbers.begin(), normalizedNumbers.end(), '\n', ',');

        std::stringstream ss(normalizedNumbers);
        std::string numberString;

        while (std::getline(ss, numberString, ',')) {
            if (!numberString.empty()) {
                try {
                    int number = std::stoi(numberString);
                    if (number < 0) {
                        negatives.push_back(number);
                    } else {
                        parsedNumbers.push_back(number);
                    }
                } catch (const std::invalid_argument&) {
                    // Ignore invalid numbers
                }
            }
        }

        if (!negatives.empty()) {
            std::string errorMessage = "Negatives not allowed: ";
            for (size_t i = 0; i < negatives.size(); ++i) {
                if (i > 0) errorMessage += ", ";
                errorMessage += std::to_string(negatives[i]);
            }
            throw std::runtime_error(errorMessage);
        }

        return std::accumulate(parsedNumbers.begin(), parsedNumbers.end(), 0);
    }
};

// Simple Test Framework for C++
class SimpleTestRunner {
private:
    int testsPassed = 0;
    int testsFailed = 0;

public:
    void assertTrue(bool condition, const std::string& testName) {
        if (condition) {
            std::cout << "✅ " << testName << std::endl;
            testsPassed++;
        } else {
            std::cout << "❌ " << testName << std::endl;
            testsFailed++;
        }
    }

    template<typename T>
    void assertEqual(const T& expected, const T& actual, const std::string& testName) {
        if (expected == actual) {
            std::cout << "✅ " << testName << std::endl;
            testsPassed++;
        } else {
            std::cout << "❌ " << testName << " - Expected: " << expected << ", Actual: " << actual << std::endl;
            testsFailed++;
        }
    }

    template<typename ExceptionType, typename Func>
    void assertThrows(Func&& func, const std::string& testName) {
        try {
            func();
            std::cout << "❌ " << testName << " - Expected exception was not thrown" << std::endl;
            testsFailed++;
        } catch (const ExceptionType&) {
            std::cout << "✅ " << testName << std::endl;
            testsPassed++;
        } catch (const std::exception& ex) {
            std::cout << "❌ " << testName << " - Wrong exception type: " << ex.what() << std::endl;
            testsFailed++;
        }
    }

    void printSummary() {
        int total = testsPassed + testsFailed;
        double successRate = total > 0 ? (testsPassed * 100.0 / total) : 0.0;

        std::cout << "\n=== Test Summary ===" << std::endl;
        std::cout << "Passed: " << testsPassed << std::endl;
        std::cout << "Failed: " << testsFailed << std::endl;
        std::cout << "Total: " << total << std::endl;
        std::cout << "Success Rate: " << std::fixed << std::setprecision(1) << successRate << "%" << std::endl;
    }
};

// Test Functions
void testCalculator(SimpleTestRunner& runner) {
    Calculator calculator;

    runner.assertEqual(8.0, calculator.add(5, 3), "Add two positive numbers");
    runner.assertEqual(2.0, calculator.subtract(5, 3), "Subtract two numbers");
    runner.assertEqual(15.0, calculator.multiply(5, 3), "Multiply two numbers");
    runner.assertEqual(2.0, calculator.divide(6, 3), "Divide two numbers");

    runner.assertThrows<std::runtime_error>([&]() {
        calculator.divide(5, 0);
    }, "Divide by zero throws exception");

    std::vector<double> numbers = {1.0, 2.0, 3.0, 4.0};
    runner.assertEqual(10.0, calculator.calculateTotal(numbers), "Calculate total of array");
}

void testShoppingCart(SimpleTestRunner& runner) {
    ShoppingCart cart;

    runner.assertEqual(static_cast<size_t>(0), cart.getItemTypes(), "New cart is empty");
    runner.assertEqual(0.0, cart.getTotal(), "Empty cart total is zero");

    cart.addItem("Apple", 1.50, 2);
    runner.assertEqual(static_cast<size_t>(1), cart.getItemTypes(), "Cart has one item type");
    runner.assertEqual(3.00, cart.getTotal(), "Cart total with one item");
    runner.assertEqual(2, cart.getItemCount(), "Cart item count");

    cart.addItem("Banana", 0.75, 3);
    runner.assertEqual(5.25, cart.getTotal(), "Cart total with multiple items");

    runner.assertTrue(cart.hasItem("Apple"), "Cart contains added item");
    runner.assertTrue(!cart.hasItem("Orange"), "Cart doesn't contain non-added item");

    runner.assertThrows<std::invalid_argument>([&]() {
        cart.addItem("Invalid", -1.0);
    }, "Negative price throws exception");
}

void testPasswordValidator(SimpleTestRunner& runner) {
    PasswordValidator validator;

    auto result = validator.validatePassword("short");
    runner.assertTrue(!result.isValid(), "Short password is invalid");

    result = validator.validatePassword("password123");
    runner.assertTrue(!result.isValid(), "Password without uppercase is invalid");

    result = validator.validatePassword("Password123!");
    runner.assertTrue(result.isValid(), "Valid password passes validation");

    result = validator.validatePassword("password");
    runner.assertTrue(!result.isValid(), "Common password is invalid");
}

void testBankAccount(SimpleTestRunner& runner) {
    BankAccount account(100.0);
    runner.assertEqual(100.0, account.getBalance(), "Initial balance set correctly");

    account.deposit(50.0);
    runner.assertEqual(150.0, account.getBalance(), "Balance after deposit");

    account.withdraw(25.0);
    runner.assertEqual(125.0, account.getBalance(), "Balance after withdrawal");

    runner.assertThrows<std::runtime_error>([&]() {
        account.withdraw(200.0);
    }, "Overdraw throws exception");

    runner.assertThrows<std::invalid_argument>([&]() {
        account.deposit(-10.0);
    }, "Negative deposit throws exception");

    runner.assertThrows<std::invalid_argument>([&]() {
        BankAccount(-50.0);
    }, "Negative initial balance throws exception");
}

void testStringCalculator(SimpleTestRunner& runner) {
    StringCalculator calculator;

    runner.assertEqual(0, calculator.add(""), "Empty string returns zero");
    runner.assertEqual(1, calculator.add("1"), "Single number");
    runner.assertEqual(3, calculator.add("1,2"), "Two numbers");
    runner.assertEqual(6, calculator.add("1,2,3"), "Three numbers");
    runner.assertEqual(6, calculator.add("1\n2,3"), "New line delimiter");

    runner.assertThrows<std::runtime_error>([&]() {
        calculator.add("1,-2");
    }, "Negative numbers throw exception");
}

void demonstrateTDDProcess() {
    std::cout << "=== TDD Red-Green-Refactor Demo ===" << std::endl;
    std::cout << std::endl;
    
    std::cout << "🔴 RED: Write failing test first" << std::endl;
    std::cout << "TEST(GradeCalculatorTest, CalculateAverage) {" << std::endl;
    std::cout << "    GradeCalculator calculator;" << std::endl;
    std::cout << "    std::vector<int> grades = {90, 80, 70};" << std::endl;
    std::cout << "    double result = calculator.calculateAverage(grades);" << std::endl;
    std::cout << "    EXPECT_EQ(80.0, result);" << std::endl;
    std::cout << "}" << std::endl;
    std::cout << std::endl;
    
    std::cout << "🟢 GREEN: Write minimal code to pass" << std::endl;
    std::cout << "class GradeCalculator {" << std::endl;
    std::cout << "public:" << std::endl;
    std::cout << "    double calculateAverage(const std::vector<int>& grades) {" << std::endl;
    std::cout << "        return std::accumulate(grades.begin(), grades.end(), 0.0) / grades.size();" << std::endl;
    std::cout << "    }" << std::endl;
    std::cout << "};" << std::endl;
    std::cout << std::endl;
    
    std::cout << "🔵 REFACTOR: Improve the code while keeping tests green" << std::endl;
    std::cout << "class GradeCalculator {" << std::endl;
    std::cout << "public:" << std::endl;
    std::cout << "    double calculateAverage(const std::vector<int>& grades) {" << std::endl;
    std::cout << "        if (grades.empty()) {" << std::endl;
    std::cout << "            throw std::invalid_argument(\"Grades list cannot be empty\");" << std::endl;
    std::cout << "        }" << std::endl;
    std::cout << "        return std::accumulate(grades.begin(), grades.end(), 0.0) / grades.size();" << std::endl;
    std::cout << "    }" << std::endl;
    std::cout << "};" << std::endl;
    std::cout << std::endl;
}

int main() {
    std::cout << "=== Testing & TDD Demo ===" << std::endl << std::endl;

    // Demonstrate TDD process
    demonstrateTDDProcess();

    // Run tests
    SimpleTestRunner runner;

    std::cout << "--- Calculator Tests ---" << std::endl;
    testCalculator(runner);

    std::cout << "\n--- Shopping Cart Tests ---" << std::endl;
    testShoppingCart(runner);

    std::cout << "\n--- Password Validator Tests ---" << std::endl;
    testPasswordValidator(runner);

    std::cout << "\n--- Bank Account Tests ---" << std::endl;
    testBankAccount(runner);

    std::cout << "\n--- String Calculator Tests ---" << std::endl;
    testStringCalculator(runner);

    runner.printSummary();

    std::cout << "\n=== TDD Benefits ===" << std::endl;
    std::cout << "✓ Catches bugs early in development" << std::endl;
    std::cout << "✓ Provides safety net for refactoring" << std::endl;
    std::cout << "✓ Documents expected behavior" << std::endl;
    std::cout << "✓ Leads to better code design" << std::endl;
    std::cout << "✓ Builds confidence in code quality" << std::endl;
    std::cout << "✓ Encourages RAII and proper resource management" << std::endl;
    std::cout << "✓ Helps identify interface design issues early" << std::endl;

    return 0;
}