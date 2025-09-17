# Chapter 15: Testing & TDD Basics

## Child-Friendly Explanation 🧸
Imagine you're building a LEGO castle. Before you show it to your friends, you want to make sure all the walls are strong, the doors open properly, and everything fits together perfectly. Testing your code is like checking your LEGO castle! You write small tests to make sure each piece of your program works correctly before putting everything together.

Test-Driven Development (TDD) is like planning your LEGO castle by deciding what you want to test BEFORE you build it!

## Developer-Level Explanation 👨‍💻
Testing is the practice of writing code that verifies your application code works correctly under various conditions. Test-Driven Development (TDD) is a development methodology where you write tests before writing the implementation code, following a red-green-refactor cycle.

**Why Testing Matters:**
- **Catch bugs early**: Find problems before users do
- **Confidence in changes**: Refactor and add features safely
- **Documentation**: Tests show how code should be used
- **Better design**: Writing tests first leads to better APIs

## Unit Testing Fundamentals 🧪

**Principle**: Test individual components in isolation to ensure they work correctly.

### Basic Test Structure

```csharp
// Class to test - a simple calculator
public class Calculator
{
    public int Add(int a, int b)
    {
        return a + b;
    }
    
    public int Subtract(int a, int b)
    {
        return a - b;
    }
    
    public double Divide(int a, int b)
    {
        if (b == 0)
            throw new DivideByZeroException("Cannot divide by zero");
        return (double)a / b;
    }
    
    public bool IsEven(int number)
    {
        return number % 2 == 0;
    }
}

// Unit tests using basic assertions
public class CalculatorTests
{
    private Calculator calculator;
    
    public CalculatorTests()
    {
        calculator = new Calculator();
    }
    
    public void TestAddition()
    {
        // Arrange - set up test data
        int a = 5;
        int b = 3;
        int expected = 8;
        
        // Act - call the method being tested
        int result = calculator.Add(a, b);
        
        // Assert - verify the result
        if (result != expected)
            throw new Exception($"Expected {expected}, but got {result}");
        
        Console.WriteLine("✅ Addition test passed");
    }
    
    public void TestSubtraction()
    {
        // Arrange
        int a = 10;
        int b = 4;
        int expected = 6;
        
        // Act
        int result = calculator.Subtract(a, b);
        
        // Assert
        if (result != expected)
            throw new Exception($"Expected {expected}, but got {result}");
        
        Console.WriteLine("✅ Subtraction test passed");
    }
    
    public void TestDivisionByZero()
    {
        // Arrange
        int a = 10;
        int b = 0;
        
        // Act & Assert - expect an exception
        try
        {
            calculator.Divide(a, b);
            throw new Exception("Expected DivideByZeroException, but no exception was thrown");
        }
        catch (DivideByZeroException)
        {
            Console.WriteLine("✅ Division by zero test passed");
        }
    }
    
    public void TestIsEven()
    {
        // Test multiple cases
        var testCases = new[]
        {
            (number: 2, expected: true),
            (number: 3, expected: false),
            (number: 0, expected: true),
            (number: -4, expected: true),
            (number: -3, expected: false)
        };
        
        foreach (var (number, expected) in testCases)
        {
            bool result = calculator.IsEven(number);
            if (result != expected)
                throw new Exception($"For {number}, expected {expected}, but got {result}");
        }
        
        Console.WriteLine("✅ IsEven test passed for all cases");
    }
}
```

**Code Explanation:**
This demonstrates the basic structure of unit tests using the Arrange-Act-Assert pattern:
- **Arrange**: Set up test data and conditions
- **Act**: Call the method being tested
- **Assert**: Verify the result matches expectations

The tests cover different scenarios: normal cases, edge cases (like zero), and error conditions (division by zero).

## Test-Driven Development (TDD) 🔄

**Principle**: Write tests first, then implement code to make tests pass. Follow the red-green-refactor cycle.

### TDD Example: Building a Shopping Cart

Let's use TDD to build a shopping cart step by step:

```csharp
// Step 1: RED - Write a failing test first
public class ShoppingCartTests
{
    public void TestEmptyCartHasZeroItems()
    {
        // Arrange & Act
        var cart = new ShoppingCart();
        
        // Assert
        if (cart.ItemCount != 0)
            throw new Exception($"Empty cart should have 0 items, but has {cart.ItemCount}");
        
        Console.WriteLine("✅ Empty cart test ready");
    }
}

// Step 2: GREEN - Write minimal code to make test pass
public class ShoppingCart
{
    public int ItemCount { get; private set; } = 0;
}

// Step 3: Run test (should pass now)

// Step 4: RED - Add another failing test
public void TestAddItemIncreasesCount()
{
    // Arrange
    var cart = new ShoppingCart();
    
    // Act
    cart.AddItem("Apple", 1.50m);
    
    // Assert
    if (cart.ItemCount != 1)
        throw new Exception($"After adding item, count should be 1, but is {cart.ItemCount}");
    
    Console.WriteLine("✅ Add item test ready");
}

// Step 5: GREEN - Implement AddItem method
public class ShoppingCart
{
    private List<CartItem> items = new List<CartItem>();
    
    public int ItemCount => items.Count;
    
    public void AddItem(string name, decimal price)
    {
        items.Add(new CartItem { Name = name, Price = price });
    }
}

public class CartItem
{
    public string Name { get; set; }
    public decimal Price { get; set; }
}

// Step 6: RED - Test total calculation
public void TestCalculateTotal()
{
    // Arrange
    var cart = new ShoppingCart();
    cart.AddItem("Apple", 1.50m);
    cart.AddItem("Banana", 0.75m);
    
    // Act
    decimal total = cart.GetTotal();
    
    // Assert
    decimal expected = 2.25m;
    if (Math.Abs(total - expected) > 0.01m)
        throw new Exception($"Expected total {expected}, but got {total}");
    
    Console.WriteLine("✅ Calculate total test ready");
}

// Step 7: GREEN - Implement GetTotal method
public class ShoppingCart
{
    private List<CartItem> items = new List<CartItem>();
    
    public int ItemCount => items.Count;
    
    public void AddItem(string name, decimal price)
    {
        items.Add(new CartItem { Name = name, Price = price });
    }
    
    public decimal GetTotal()
    {
        return items.Sum(item => item.Price);
    }
}

// Step 8: REFACTOR - Improve the design
public class ShoppingCart
{
    private readonly List<CartItem> items = new List<CartItem>();
    
    public int ItemCount => items.Count;
    public decimal Total => items.Sum(item => item.Price);
    
    public void AddItem(string name, decimal price)
    {
        if (string.IsNullOrWhiteSpace(name))
            throw new ArgumentException("Item name cannot be empty");
        
        if (price < 0)
            throw new ArgumentException("Item price cannot be negative");
        
        items.Add(new CartItem(name, price));
    }
    
    public void RemoveItem(string name)
    {
        var item = items.FirstOrDefault(i => i.Name == name);
        if (item != null)
            items.Remove(item);
    }
    
    public List<CartItem> GetItems()
    {
        return items.ToList(); // Return copy to prevent external modification
    }
}

public class CartItem
{
    public string Name { get; }
    public decimal Price { get; }
    
    public CartItem(string name, decimal price)
    {
        Name = name;
        Price = price;
    }
}
```

**Code Explanation:**
This demonstrates the TDD cycle:
1. **RED**: Write a failing test for desired functionality
2. **GREEN**: Write minimal code to make the test pass
3. **REFACTOR**: Improve code quality while keeping tests green
4. **Repeat**: Add the next test and continue the cycle

Each step adds one small piece of functionality, guided by tests.

## Testing Best Practices 📋

### Good Test Characteristics

```csharp
// ✅ Good test - follows FIRST principles
public class OrderProcessorTests
{
    public void TestProcessOrder_WithValidOrder_ReturnsSuccess()
    {
        // FAST: Quick to run
        // INDEPENDENT: Doesn't depend on other tests
        // REPEATABLE: Same result every time
        // SELF-VALIDATING: Clear pass/fail
        // TIMELY: Written just before implementation
        
        // Arrange
        var orderProcessor = new OrderProcessor();
        var order = new Order
        {
            Id = "ORDER-123",
            Items = new List<OrderItem>
            {
                new OrderItem("Product1", 10.00m, 2)
            }
        };
        
        // Act
        var result = orderProcessor.ProcessOrder(order);
        
        // Assert
        if (!result.Success)
            throw new Exception($"Expected success, but got: {result.ErrorMessage}");
        
        if (result.OrderId != order.Id)
            throw new Exception($"Expected order ID {order.Id}, but got {result.OrderId}");
        
        Console.WriteLine("✅ Process order test passed");
    }
    
    public void TestProcessOrder_WithEmptyOrder_ReturnsFailure()
    {
        // Arrange
        var orderProcessor = new OrderProcessor();
        var emptyOrder = new Order { Id = "EMPTY-001", Items = new List<OrderItem>() };
        
        // Act
        var result = orderProcessor.ProcessOrder(emptyOrder);
        
        // Assert
        if (result.Success)
            throw new Exception("Expected failure for empty order, but got success");
        
        if (!result.ErrorMessage.Contains("empty"))
            throw new Exception($"Expected error about empty order, but got: {result.ErrorMessage}");
        
        Console.WriteLine("✅ Empty order test passed");
    }
}

// Supporting classes for the test
public class OrderProcessor
{
    public ProcessResult ProcessOrder(Order order)
    {
        if (order == null)
            return new ProcessResult { Success = false, ErrorMessage = "Order cannot be null" };
        
        if (order.Items == null || order.Items.Count == 0)
            return new ProcessResult { Success = false, ErrorMessage = "Order cannot be empty" };
        
        // Process the order
        Console.WriteLine($"Processing order {order.Id} with {order.Items.Count} items");
        
        return new ProcessResult { Success = true, OrderId = order.Id };
    }
}

public class ProcessResult
{
    public bool Success { get; set; }
    public string OrderId { get; set; }
    public string ErrorMessage { get; set; }
}

public class Order
{
    public string Id { get; set; }
    public List<OrderItem> Items { get; set; } = new List<OrderItem>();
}

public class OrderItem
{
    public string ProductName { get; }
    public decimal Price { get; }
    public int Quantity { get; }
    
    public OrderItem(string productName, decimal price, int quantity)
    {
        ProductName = productName;
        Price = price;
        Quantity = quantity;
    }
}
```

**Code Explanation:**
Good tests follow the FIRST principles:
- **Fast**: Run quickly to encourage frequent execution
- **Independent**: Each test can run alone without depending on others
- **Repeatable**: Same results in any environment
- **Self-Validating**: Clear pass/fail without manual inspection
- **Timely**: Written just before or with the implementation code

## TDD Benefits and Challenges

| Benefits | Challenges |
|----------|------------|
| ✅ Better design through testing first | ⚠️ Initial learning curve |
| ✅ High test coverage by default | ⚠️ Requires discipline to maintain |
| ✅ Confidence in refactoring | ⚠️ Can slow initial development |
| ✅ Tests serve as documentation | ⚠️ Tests need maintenance too |

## Key Takeaways

✅ **Tests give you confidence to make changes**  
✅ **TDD leads to better design and fewer bugs**  
✅ **Write tests for normal cases, edge cases, and error conditions**  
✅ **Good tests are fast, independent, and self-validating**  
✅ **The red-green-refactor cycle guides incremental development**

---

*Remember: Tests are not just about finding bugs - they're about designing better software and giving you confidence to evolve your code.*