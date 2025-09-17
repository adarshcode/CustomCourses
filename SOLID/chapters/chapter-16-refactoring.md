# Chapter 16: Refactoring - From Bad Code to SOLID Code

## Child-Friendly Explanation 🧸
Imagine your bedroom gets really messy over time - clothes everywhere, toys scattered around, books in piles. It still works as a bedroom, but it's hard to find anything and it's not fun to be in. Refactoring is like cleaning and organizing your room without throwing anything important away. When you're done, everything still works the same way, but now it's much nicer, easier to use, and you can find things quickly!

## Developer-Level Explanation 👨‍💻
Refactoring is the process of improving code structure and design without changing its external behavior. It's about making code more readable, maintainable, and extensible while preserving functionality. This chapter shows how to systematically transform messy code into clean, SOLID-compliant code.

**Why Refactoring Matters:**
- **Technical Debt Reduction**: Prevents code from becoming unmaintainable
- **Improved Readability**: Makes code easier to understand and modify
- **Better Design**: Applies SOLID principles to existing code
- **Risk Mitigation**: Reduces the chance of bugs in future changes

## The Refactoring Process 🔄

**Safe Refactoring Steps:**
1. **Identify code smells** and SOLID violations
2. **Write tests** to preserve existing behavior
3. **Apply refactoring techniques** in small steps
4. **Verify tests still pass** after each change
5. **Clean up** and remove obsolete code

## Refactoring Example: Order Processing System

Let's refactor a messy order processing system step by step:

### Step 1: The Messy Code (Before Refactoring)

```csharp
// ❌ This class violates all SOLID principles!
public class OrderManager
{
    public void ProcessOrder(string customerName, string customerEmail, 
                           List<string> productNames, List<decimal> prices, 
                           List<int> quantities, string paymentMethod)
    {
        // Validation logic mixed with business logic
        if (string.IsNullOrEmpty(customerName))
        {
            Console.WriteLine("Error: Customer name is required");
            return;
        }
        
        if (string.IsNullOrEmpty(customerEmail) || !customerEmail.Contains("@"))
        {
            Console.WriteLine("Error: Valid email is required");
            return;
        }
        
        if (productNames.Count != prices.Count || productNames.Count != quantities.Count)
        {
            Console.WriteLine("Error: Product data mismatch");
            return;
        }
        
        // Calculate total - business logic
        decimal total = 0;
        for (int i = 0; i < productNames.Count; i++)
        {
            total += prices[i] * quantities[i];
        }
        
        // Apply discount logic - more business rules
        if (total > 100)
        {
            total *= 0.9m; // 10% discount
            Console.WriteLine("Applied 10% discount for orders over $100");
        }
        
        // Payment processing - external dependency
        if (paymentMethod == "creditcard")
        {
            Console.WriteLine($"Processing credit card payment of ${total}");
            // Simulate credit card processing
            if (total > 1000)
            {
                Console.WriteLine("Error: Credit card limit exceeded");
                return;
            }
        }
        else if (paymentMethod == "paypal")
        {
            Console.WriteLine($"Processing PayPal payment of ${total}");
            // Simulate PayPal processing
        }
        else
        {
            Console.WriteLine("Error: Unsupported payment method");
            return;
        }
        
        // Database operations - another external dependency
        Console.WriteLine("Saving order to database...");
        Console.WriteLine($"Order saved: {customerName}, ${total}");
        
        // Email notifications - yet another external dependency
        Console.WriteLine($"Sending confirmation email to {customerEmail}");
        Console.WriteLine($"Email sent: Your order for ${total} has been processed");
        
        // Inventory management - more mixed concerns
        for (int i = 0; i < productNames.Count; i++)
        {
            Console.WriteLine($"Reducing inventory for {productNames[i]} by {quantities[i]}");
        }
        
        Console.WriteLine("Order processing completed");
    }
}
```

**Code Problems:**
- **SRP Violation**: Handles validation, calculation, payment, database, email, and inventory
- **OCP Violation**: Adding new payment methods requires modifying the class
- **LSP Violation**: Not applicable, but design doesn't support polymorphism
- **ISP Violation**: Any client using this class depends on all functionality
- **DIP Violation**: Directly depends on concrete implementations and external systems

### Step 2: Extract Domain Models (SRP)

```csharp
// ✅ Step 2: Create focused domain models
public class Customer
{
    public string Name { get; }
    public string Email { get; }
    
    public Customer(string name, string email)
    {
        if (string.IsNullOrWhiteSpace(name))
            throw new ArgumentException("Customer name is required");
        
        if (string.IsNullOrWhiteSpace(email) || !email.Contains("@"))
            throw new ArgumentException("Valid email is required");
        
        Name = name;
        Email = email;
    }
}

public class OrderItem
{
    public string ProductName { get; }
    public decimal Price { get; }
    public int Quantity { get; }
    public decimal LineTotal => Price * Quantity;
    
    public OrderItem(string productName, decimal price, int quantity)
    {
        if (string.IsNullOrWhiteSpace(productName))
            throw new ArgumentException("Product name is required");
        
        if (price < 0)
            throw new ArgumentException("Price cannot be negative");
        
        if (quantity <= 0)
            throw new ArgumentException("Quantity must be positive");
        
        ProductName = productName;
        Price = price;
        Quantity = quantity;
    }
}

public class Order
{
    public Customer Customer { get; }
    public List<OrderItem> Items { get; }
    public decimal Subtotal => Items.Sum(item => item.LineTotal);
    public decimal Total { get; private set; }
    
    public Order(Customer customer, List<OrderItem> items)
    {
        Customer = customer ?? throw new ArgumentNullException(nameof(customer));
        Items = items ?? throw new ArgumentNullException(nameof(items));
        
        if (!items.Any())
            throw new ArgumentException("Order must contain at least one item");
        
        Total = Subtotal;
    }
    
    public void ApplyDiscount(decimal discountAmount)
    {
        Total = Math.Max(0, Total - discountAmount);
    }
}
```

**Code Explanation:**
We've extracted domain models that follow SRP. Each class has a single responsibility:
- `Customer` manages customer data and validation
- `OrderItem` manages individual item data and calculations
- `Order` manages the overall order and applies business rules

### Step 3: Create Abstractions (DIP + OCP)

```csharp
// ✅ Step 3: Define interfaces to support DIP and OCP
public interface IPaymentProcessor
{
    PaymentResult ProcessPayment(decimal amount);
}

public interface IOrderRepository
{
    void SaveOrder(Order order);
}

public interface IEmailService
{
    void SendOrderConfirmation(Customer customer, Order order);
}

public interface IInventoryService
{
    void UpdateInventory(List<OrderItem> items);
}

public interface IDiscountCalculator
{
    decimal CalculateDiscount(Order order);
}

// Supporting classes
public class PaymentResult
{
    public bool Success { get; set; }
    public string ErrorMessage { get; set; }
}
```

**Code Explanation:**
These interfaces define abstractions that allow us to:
- **Follow DIP**: High-level order processing won't depend on low-level details
- **Follow OCP**: New implementations can be added without modifying existing code
- **Support testing**: Interfaces can be easily mocked for unit tests

### Step 4: Implement Concrete Services (LSP + ISP)

```csharp
// ✅ Step 4: Concrete implementations that follow LSP
public class CreditCardProcessor : IPaymentProcessor
{
    public PaymentResult ProcessPayment(decimal amount)
    {
        Console.WriteLine($"Processing credit card payment of ${amount}");
        
        if (amount > 1000)
        {
            return new PaymentResult 
            { 
                Success = false, 
                ErrorMessage = "Credit card limit exceeded" 
            };
        }
        
        return new PaymentResult { Success = true };
    }
}

public class PayPalProcessor : IPaymentProcessor
{
    public PaymentResult ProcessPayment(decimal amount)
    {
        Console.WriteLine($"Processing PayPal payment of ${amount}");
        return new PaymentResult { Success = true };
    }
}

public class DatabaseOrderRepository : IOrderRepository
{
    public void SaveOrder(Order order)
    {
        Console.WriteLine($"Saving order to database: {order.Customer.Name}, ${order.Total}");
    }
}

public class EmailService : IEmailService
{
    public void SendOrderConfirmation(Customer customer, Order order)
    {
        Console.WriteLine($"Sending confirmation email to {customer.Email}");
        Console.WriteLine($"Email sent: Your order for ${order.Total} has been processed");
    }
}

public class InventoryService : IInventoryService
{
    public void UpdateInventory(List<OrderItem> items)
    {
        foreach (var item in items)
        {
            Console.WriteLine($"Reducing inventory for {item.ProductName} by {item.Quantity}");
        }
    }
}

public class VolumeDiscountCalculator : IDiscountCalculator
{
    public decimal CalculateDiscount(Order order)
    {
        if (order.Subtotal > 100)
        {
            var discount = order.Subtotal * 0.1m; // 10% discount
            Console.WriteLine($"Applied 10% volume discount: ${discount}");
            return discount;
        }
        return 0;
    }
}
```

**Code Explanation:**
Each concrete implementation follows LSP by being fully substitutable for its interface. The interfaces are focused (ISP) - each service only exposes what's needed for its specific responsibility.

### Step 5: Refactored Order Service (All SOLID Principles)

```csharp
// ✅ Step 5: Clean OrderService following all SOLID principles
public class OrderService
{
    private readonly IPaymentProcessor paymentProcessor;
    private readonly IOrderRepository orderRepository;
    private readonly IEmailService emailService;
    private readonly IInventoryService inventoryService;
    private readonly IDiscountCalculator discountCalculator;
    
    // DIP: Depend on abstractions, inject dependencies
    public OrderService(
        IPaymentProcessor paymentProcessor,
        IOrderRepository orderRepository,
        IEmailService emailService,
        IInventoryService inventoryService,
        IDiscountCalculator discountCalculator)
    {
        this.paymentProcessor = paymentProcessor;
        this.orderRepository = orderRepository;
        this.emailService = emailService;
        this.inventoryService = inventoryService;
        this.discountCalculator = discountCalculator;
    }
    
    // SRP: Single responsibility - coordinate order processing
    public bool ProcessOrder(Customer customer, List<OrderItem> items)
    {
        try
        {
            // Create order
            var order = new Order(customer, items);
            
            // Apply discounts
            var discount = discountCalculator.CalculateDiscount(order);
            if (discount > 0)
            {
                order.ApplyDiscount(discount);
            }
            
            // Process payment
            var paymentResult = paymentProcessor.ProcessPayment(order.Total);
            if (!paymentResult.Success)
            {
                Console.WriteLine($"Payment failed: {paymentResult.ErrorMessage}");
                return false;
            }
            
            // Save order
            orderRepository.SaveOrder(order);
            
            // Send confirmation
            emailService.SendOrderConfirmation(customer, order);
            
            // Update inventory
            inventoryService.UpdateInventory(order.Items);
            
            Console.WriteLine("Order processing completed successfully");
            return true;
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Order processing failed: {ex.Message}");
            return false;
        }
    }
}
```

**Code Explanation:**
The refactored `OrderService` now follows all SOLID principles:
- **SRP**: Only coordinates order processing, delegates specific tasks
- **OCP**: New payment methods or discount strategies can be added without changes
- **LSP**: Any implementation of the interfaces can be substituted
- **ISP**: Uses focused interfaces, no forced dependencies
- **DIP**: Depends on abstractions, not concrete implementations

### Step 6: Usage Example

```csharp
// ✅ Step 6: Clean usage with dependency injection
public class OrderProcessingDemo
{
    public static void RunDemo()
    {
        // Set up dependencies (in real app, this would be done by DI container)
        var paymentProcessor = new CreditCardProcessor();
        var orderRepository = new DatabaseOrderRepository();
        var emailService = new EmailService();
        var inventoryService = new InventoryService();
        var discountCalculator = new VolumeDiscountCalculator();
        
        // Create order service with injected dependencies
        var orderService = new OrderService(
            paymentProcessor,
            orderRepository,
            emailService,
            inventoryService,
            discountCalculator);
        
        // Process an order
        var customer = new Customer("John Doe", "john@email.com");
        var items = new List<OrderItem>
        {
            new OrderItem("Laptop", 999.99m, 1),
            new OrderItem("Mouse", 29.99m, 2)
        };
        
        bool success = orderService.ProcessOrder(customer, items);
        Console.WriteLine($"Order processing result: {success}");
    }
}
```

**Code Explanation:**
The usage is now clean and follows dependency injection principles. Different implementations can be easily swapped for testing or different environments.

## Refactoring Techniques Summary

| Technique | Purpose | When to Use |
|-----------|---------|-------------|
| **Extract Method** | Break down large functions | Function doing multiple things |
| **Extract Class** | Separate responsibilities | Class violating SRP |
| **Introduce Interface** | Create abstractions | Tight coupling, hard to test |
| **Move Method** | Place method in right class | Method using more data from another class |
| **Replace Conditional with Polymorphism** | Eliminate type checking | Long if/switch statements |

## Key Takeaways

✅ **Refactoring improves code without changing behavior**  
✅ **Always have tests before refactoring to ensure safety**  
✅ **Apply SOLID principles systematically during refactoring**  
✅ **Small, incremental changes are safer than large rewrites**  
✅ **Dependency injection makes code more testable and flexible**

---

*Remember: Good code is not written, it's rewritten. Refactoring is an essential skill for maintaining healthy codebases over time.*