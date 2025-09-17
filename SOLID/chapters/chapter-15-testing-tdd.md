# Chapter 15: Testing & TDD Basics

> **"Testing is not about proving you're right, it's about finding out when you're wrong."** — Anonymous

## 🎯 Learning Objectives

By the end of this chapter, you will:
- Understand why testing is crucial for software quality
- Learn the basics of unit testing and Test-Driven Development (TDD)
- Know how to write effective test cases
- Practice writing tests in C#, Python, and C++
- Apply TDD red-green-refactor cycle

---

## 🧸 Kid-Friendly Explanation

Imagine you're building a LEGO castle. Before you show it to your friends, you want to make sure:
- All the walls are strong and won't fall down
- The doors open and close properly  
- The towers are straight and stable
- Everything fits together perfectly

Testing your code is like checking your LEGO castle! You write small tests to make sure each piece of your program works correctly before putting everything together.

```python
# Like testing if a LEGO door opens
def test_door_opens():
    door = LegoDoor()
    door.open()
    assert door.is_open == True  # Check that it worked!

# Like testing if walls are strong
def test_wall_is_stable():
    wall = LegoWall()
    wall.add_pressure(100)
    assert wall.is_standing == True  # Make sure it didn't fall!
```

**Test-Driven Development (TDD)** is like planning your LEGO castle by deciding what you want to test BEFORE you build it!

---

## 👨‍💻 Developer Explanation

**Testing** is the practice of writing code that verifies your application code works correctly under various conditions. **Test-Driven Development (TDD)** is a development methodology where you write tests before writing the implementation code.

### Why Testing Matters

1. **Catch bugs early** - Find problems before users do
2. **Confidence in changes** - Refactor without fear of breaking things
3. **Documentation** - Tests show how code should be used
4. **Better design** - Writing tests first leads to cleaner, more testable code

### The TDD Red-Green-Refactor Cycle

```
🔴 RED: Write a failing test
    ↓
🟢 GREEN: Write minimal code to make it pass  
    ↓
🔵 REFACTOR: Clean up the code while keeping tests passing
    ↓
Repeat...
```

### Types of Tests

#### 1. **Unit Tests**
Test individual functions or classes in isolation:

```csharp
[Test]
public void CalculateTotal_WithValidItems_ReturnsCorrectSum()
{
    // Arrange
    var calculator = new Calculator();
    var items = new[] { 10.0, 20.0, 30.0 };
    
    // Act
    var result = calculator.CalculateTotal(items);
    
    // Assert
    Assert.AreEqual(60.0, result);
}
```

#### 2. **Integration Tests**
Test how different parts work together:

```python
def test_user_registration_sends_email():
    # Arrange
    user_service = UserService(user_repo, email_service)
    
    # Act
    user_service.register_user("John", "john@test.com", 25)
    
    # Assert
    assert email_service.sent_emails_count == 1
    assert "john@test.com" in email_service.sent_to
```

### Test Structure: Arrange-Act-Assert (AAA)

```cpp
TEST(CalculatorTest, AddTwoNumbers) {
    // Arrange - Set up test data
    Calculator calc;
    int a = 5, b = 3;
    
    // Act - Execute the code under test
    int result = calc.add(a, b);
    
    // Assert - Verify the outcome
    EXPECT_EQ(8, result);
}
```

### TDD Example: Building a Password Validator

Let's walk through TDD by building a password validator:

#### Step 1: 🔴 RED - Write failing test
```csharp
[Test]
public void IsValid_WithShortPassword_ReturnsFalse()
{
    var validator = new PasswordValidator();
    bool result = validator.IsValid("123");
    Assert.IsFalse(result);
}
```

#### Step 2: 🟢 GREEN - Make it pass (minimal code)
```csharp
public class PasswordValidator
{
    public bool IsValid(string password)
    {
        return password.Length >= 8; // Simplest implementation
    }
}
```

#### Step 3: 🔴 RED - Add another failing test
```csharp
[Test]
public void IsValid_WithoutUppercase_ReturnsFalse()
{
    var validator = new PasswordValidator();
    bool result = validator.IsValid("password123");
    Assert.IsFalse(result);
}
```

#### Step 4: 🟢 GREEN - Update implementation
```csharp
public bool IsValid(string password)
{
    return password.Length >= 8 && 
           password.Any(char.IsUpper);
}
```

#### Step 5: 🔵 REFACTOR - Clean up
```csharp
public class PasswordValidator
{
    private const int MINIMUM_LENGTH = 8;
    
    public bool IsValid(string password)
    {
        if (string.IsNullOrEmpty(password))
            return false;
            
        return HasMinimumLength(password) && 
               HasUppercaseCharacter(password);
    }
    
    private bool HasMinimumLength(string password) 
        => password.Length >= MINIMUM_LENGTH;
    
    private bool HasUppercaseCharacter(string password) 
        => password.Any(char.IsUpper);
}
```

### Testing Best Practices

#### 1. **Good Test Names**
```python
# ❌ Bad: Unclear what's being tested
def test_calculate():
    pass

# ✅ Good: Clear intention
def test_calculate_total_with_empty_list_returns_zero():
    pass

def test_calculate_total_with_negative_numbers_raises_exception():
    pass
```

#### 2. **One Assert Per Test**
```csharp
// ❌ Bad: Multiple things being tested
[Test]
public void UserCreation_Test()
{
    var user = new User("John", "john@test.com", 25);
    Assert.AreEqual("John", user.Name);
    Assert.AreEqual("john@test.com", user.Email);
    Assert.AreEqual(25, user.Age);
}

// ✅ Good: Focused tests
[Test]
public void User_WithValidName_SetsNameCorrectly()
{
    var user = new User("John", "john@test.com", 25);
    Assert.AreEqual("John", user.Name);
}

[Test]
public void User_WithValidEmail_SetsEmailCorrectly()
{
    var user = new User("John", "john@test.com", 25);
    Assert.AreEqual("john@test.com", user.Email);
}
```

#### 3. **Test Edge Cases**
```python
def test_divide_by_zero_raises_exception():
    calculator = Calculator()
    with pytest.raises(ZeroDivisionError):
        calculator.divide(10, 0)

def test_large_numbers_handled_correctly():
    calculator = Calculator()
    result = calculator.add(999999999, 1)
    assert result == 1000000000

def test_negative_numbers_handled_correctly():
    calculator = Calculator()
    result = calculator.add(-5, -3)
    assert result == -8
```

### Mocking and Test Doubles

When testing, you often need to isolate the code under test from its dependencies:

```csharp
// Interface for dependency
public interface IEmailService 
{
    void SendEmail(string to, string subject, string body);
}

// Class under test
public class UserService
{
    private readonly IEmailService emailService;
    
    public UserService(IEmailService emailService)
    {
        this.emailService = emailService;
    }
    
    public void RegisterUser(string email)
    {
        // Registration logic...
        emailService.SendEmail(email, "Welcome", "Welcome to our service!");
    }
}

// Test with mock
[Test]
public void RegisterUser_SendsWelcomeEmail()
{
    // Arrange
    var mockEmailService = new Mock<IEmailService>();
    var userService = new UserService(mockEmailService.Object);
    
    // Act
    userService.RegisterUser("test@example.com");
    
    // Assert
    mockEmailService.Verify(x => x.SendEmail(
        "test@example.com", 
        "Welcome", 
        "Welcome to our service!"), Times.Once);
}
```

---

## 🔧 Practical Examples

### Example 1: Shopping Cart with TDD

Let's build a shopping cart using TDD:

```python
import unittest

class ShoppingCart:
    def __init__(self):
        self.items = []
    
    def add_item(self, name, price, quantity=1):
        if price < 0:
            raise ValueError("Price cannot be negative")
        if quantity <= 0:
            raise ValueError("Quantity must be positive")
            
        self.items.append({
            'name': name,
            'price': price,
            'quantity': quantity
        })
    
    def get_total(self):
        return sum(item['price'] * item['quantity'] for item in self.items)
    
    def get_item_count(self):
        return sum(item['quantity'] for item in self.items)

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
        self.assertEqual(5.25, self.cart.get_total())  # (1.50*2) + (0.75*3)
        self.assertEqual(5, self.cart.get_item_count())
    
    def test_negative_price_raises_exception(self):
        with self.assertRaises(ValueError):
            self.cart.add_item("Invalid", -1.0)
    
    def test_zero_quantity_raises_exception(self):
        with self.assertRaises(ValueError):
            self.cart.add_item("Invalid", 1.0, 0)
```

---

## 🎯 Exercises

### Exercise 1: String Calculator (TDD Kata)

Create a String Calculator using TDD that:
1. Takes a string of numbers separated by commas and returns their sum
2. Handles empty strings (returns 0)
3. Handles single numbers
4. Handles two numbers
5. Handles any number of numbers
6. Handles new lines between numbers
7. Throws exception for negative numbers

Example:
```
"" → 0
"1" → 1  
"1,2" → 3
"1\n2,3" → 6
"1,-2" → Exception: "negatives not allowed: -2"
```

### Exercise 2: Bank Account

Using TDD, create a BankAccount class with:
- Initial balance
- Deposit money (positive amounts only)
- Withdraw money (can't overdraw)
- Get current balance
- Transaction history

### Exercise 3: Password Validator

Extend the password validator to check:
- Minimum 8 characters
- At least one uppercase letter
- At least one lowercase letter  
- At least one digit
- At least one special character
- No common passwords (like "password123")

---

## ✅ Checklist

- [ ] **Write Tests First**: Do you write the test before the implementation?
- [ ] **Red-Green-Refactor**: Are you following the TDD cycle?
- [ ] **Clear Test Names**: Do test names clearly describe what's being tested?
- [ ] **Single Responsibility**: Does each test verify one specific behavior?
- [ ] **Arrange-Act-Assert**: Are your tests well-structured?
- [ ] **Edge Cases**: Do you test boundary conditions and error cases?
- [ ] **Fast Tests**: Do your tests run quickly?
- [ ] **Independent Tests**: Can tests run in any order?
- [ ] **Repeatable**: Do tests give the same result every time?

---

## 🎯 Key Takeaways

1. **Tests are safety nets** - They catch bugs before they reach users
2. **TDD improves design** - Writing tests first leads to better code structure
3. **Tests are documentation** - They show how your code should be used
4. **Refactor with confidence** - Good tests let you improve code safely
5. **Test the behavior, not the implementation** - Focus on what the code does, not how
6. **Start simple** - Write the simplest test that fails, then make it pass
7. **Mock dependencies** - Isolate the code under test from external dependencies

Testing and TDD are skills that take practice to master, but they're essential for building reliable, maintainable software. Start with simple tests and gradually incorporate more advanced techniques as you become more comfortable.

---

**Next Chapter:** [Chapter 16: Refactoring Techniques](./chapter-16-refactoring.md)