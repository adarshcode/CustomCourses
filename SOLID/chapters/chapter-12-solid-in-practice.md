# Chapter 12: SOLID in Practice - Complete System

## Child-Friendly Explanation 🧸
Remember learning to ride a bike? First you learned about balance, then pedaling, then steering, then braking - each skill separately. But to actually ride a bike, you need to use ALL these skills together at the same time! SOLID principles are like this - we've learned each principle separately, but now we're going to build a complete program that uses ALL the principles working together to create something amazing!

## Developer-Level Explanation 👨‍💻
This chapter demonstrates how all five SOLID principles work together in a realistic application. We'll build a complete order processing system that showcases how SRP, OCP, LSP, ISP, and DIP combine to create maintainable, extensible, and robust software.

## Building an Order Processing System

Let's create an e-commerce order system that uses all SOLID principles together:

### 1. Domain Models (SRP in Action)

Each class has a single, clear responsibility:

```csharp
// SRP: Order only manages order data and business rules
public class Order
{
    public string Id { get; private set; }
    public string CustomerId { get; private set; }
    public List<OrderItem> Items { get; private set; }
    public OrderStatus Status { get; private set; }
    public decimal Total => Items.Sum(item => item.Price * item.Quantity);
    
    public Order(string customerId)
    {
        Id = Guid.NewGuid().ToString();
        CustomerId = customerId;
        Items = new List<OrderItem>();
        Status = OrderStatus.Pending;
    }
    
    public void AddItem(string productId, decimal price, int quantity)
    {
        Items.Add(new OrderItem(productId, price, quantity));
    }
    
    public void UpdateStatus(OrderStatus newStatus)
    {
        Status = newStatus;
    }
}

// SRP: OrderItem only manages item data
public class OrderItem
{
    public string ProductId { get; }
    public decimal Price { get; }
    public int Quantity { get; }
    
    public OrderItem(string productId, decimal price, int quantity)
    {
        ProductId = productId;
        Price = price;
        Quantity = quantity;
    }
}

public enum OrderStatus { Pending, Paid, Shipped, Delivered, Cancelled }
```

**Code Explanation:**
The `Order` class demonstrates SRP by having only one responsibility: managing order data and basic business rules. It doesn't handle payment processing, notifications, or persistence - those are separate concerns handled by other classes.

### 2. Payment Processing (OCP + Strategy Pattern)

Open for extension, closed for modification:

```csharp
// Interface for payment processing (OCP foundation)
public interface IPaymentProcessor
{
    PaymentResult ProcessPayment(decimal amount, string customerId);
}

// Concrete payment implementations (OCP extensions)
public class CreditCardProcessor : IPaymentProcessor
{
    public PaymentResult ProcessPayment(decimal amount, string customerId)
    {
        Console.WriteLine($"💳 Processing ${amount} credit card payment for customer {customerId}");
        // Simulate credit card processing
        return new PaymentResult { Success = true, TransactionId = "CC-" + Guid.NewGuid().ToString()[..8] };
    }
}

public class PayPalProcessor : IPaymentProcessor
{
    public PaymentResult ProcessPayment(decimal amount, string customerId)
    {
        Console.WriteLine($"🅿️ Processing ${amount} PayPal payment for customer {customerId}");
        // Simulate PayPal processing
        return new PaymentResult { Success = true, TransactionId = "PP-" + Guid.NewGuid().ToString()[..8] };
    }
}

public class CryptoProcessor : IPaymentProcessor
{
    public PaymentResult ProcessPayment(decimal amount, string customerId)
    {
        Console.WriteLine($"₿ Processing ${amount} cryptocurrency payment for customer {customerId}");
        // Simulate crypto processing
        return new PaymentResult { Success = true, TransactionId = "BTC-" + Guid.NewGuid().ToString()[..8] };
    }
}

public class PaymentResult
{
    public bool Success { get; set; }
    public string TransactionId { get; set; }
    public string ErrorMessage { get; set; }
}
```

**Code Explanation:**
This demonstrates OCP perfectly - we can add new payment methods (like Apple Pay or Google Pay) by creating new classes that implement `IPaymentProcessor`, without modifying any existing code. The system is open for extension but closed for modification.

### 3. Notification System (ISP + Observer Pattern)

Interface segregation with focused interfaces:

```csharp
// ISP: Separate interfaces for different notification capabilities
public interface IOrderNotifier
{
    void NotifyOrderPlaced(Order order);
}

public interface IPaymentNotifier
{
    void NotifyPaymentProcessed(string orderId, PaymentResult result);
}

public interface IShippingNotifier
{
    void NotifyOrderShipped(string orderId, string trackingNumber);
}

// Concrete notifiers implementing only what they need
public class EmailNotifier : IOrderNotifier, IPaymentNotifier, IShippingNotifier
{
    public void NotifyOrderPlaced(Order order)
    {
        Console.WriteLine($"📧 Email: Order {order.Id} placed for ${order.Total}");
    }
    
    public void NotifyPaymentProcessed(string orderId, PaymentResult result)
    {
        Console.WriteLine($"📧 Email: Payment {result.TransactionId} processed for order {orderId}");
    }
    
    public void NotifyOrderShipped(string orderId, string trackingNumber)
    {
        Console.WriteLine($"📧 Email: Order {orderId} shipped. Tracking: {trackingNumber}");
    }
}

public class SmsNotifier : IOrderNotifier, IShippingNotifier
{
    // SMS doesn't need payment notifications (ISP in action)
    public void NotifyOrderPlaced(Order order)
    {
        Console.WriteLine($"📱 SMS: Order {order.Id} placed");
    }
    
    public void NotifyOrderShipped(string orderId, string trackingNumber)
    {
        Console.WriteLine($"📱 SMS: Order {orderId} shipped. Track: {trackingNumber}");
    }
}
```

**Code Explanation:**
ISP is demonstrated here by having separate, focused interfaces instead of one large notification interface. `SmsNotifier` only implements the interfaces it needs (order and shipping notifications) and doesn't have to implement payment notifications, which follows ISP perfectly.

### 4. Order Service (DIP + Dependency Injection)

High-level modules depend on abstractions:

```csharp
// DIP: OrderService depends on abstractions, not concretions
public class OrderService
{
    private readonly IPaymentProcessor paymentProcessor;
    private readonly IOrderNotifier orderNotifier;
    private readonly IPaymentNotifier paymentNotifier;
    private readonly IOrderRepository orderRepository;
    
    // DIP: Dependencies injected through constructor
    public OrderService(
        IPaymentProcessor paymentProcessor,
        IOrderNotifier orderNotifier,
        IPaymentNotifier paymentNotifier,
        IOrderRepository orderRepository)
    {
        this.paymentProcessor = paymentProcessor;
        this.orderNotifier = orderNotifier;
        this.paymentNotifier = paymentNotifier;
        this.orderRepository = orderRepository;
    }
    
    public async Task<string> ProcessOrder(string customerId, List<(string productId, decimal price, int quantity)> items)
    {
        // Create order
        var order = new Order(customerId);
        foreach (var (productId, price, quantity) in items)
        {
            order.AddItem(productId, price, quantity);
        }
        
        // Notify order placed
        orderNotifier.NotifyOrderPlaced(order);
        
        // Process payment
        var paymentResult = paymentProcessor.ProcessPayment(order.Total, customerId);
        
        if (paymentResult.Success)
        {
            order.UpdateStatus(OrderStatus.Paid);
            paymentNotifier.NotifyPaymentProcessed(order.Id, paymentResult);
            
            // Save to repository
            await orderRepository.SaveAsync(order);
            
            return order.Id;
        }
        else
        {
            order.UpdateStatus(OrderStatus.Cancelled);
            Console.WriteLine($"❌ Payment failed: {paymentResult.ErrorMessage}");
            return null;
        }
    }
}

// Repository interface (DIP)
public interface IOrderRepository
{
    Task SaveAsync(Order order);
    Task<Order> GetByIdAsync(string orderId);
}

// Concrete repository implementation
public class InMemoryOrderRepository : IOrderRepository
{
    private readonly Dictionary<string, Order> orders = new();
    
    public Task SaveAsync(Order order)
    {
        orders[order.Id] = order;
        Console.WriteLine($"💾 Order {order.Id} saved to repository");
        return Task.CompletedTask;
    }
    
    public Task<Order> GetByIdAsync(string orderId)
    {
        orders.TryGetValue(orderId, out var order);
        return Task.FromResult(order);
    }
}
```

**Code Explanation:**
DIP is demonstrated through dependency injection - `OrderService` doesn't create its dependencies directly. Instead, it receives them through its constructor, depending only on interfaces (abstractions) rather than concrete implementations. This makes the service easily testable and allows different implementations to be swapped without changing the service code.

### 5. Putting It All Together (LSP Compliance)

Let's create different order processors that can be substituted for each other:

```csharp
// Base order processor (LSP foundation)
public abstract class OrderProcessorBase
{
    public virtual async Task<bool> ProcessOrder(Order order)
    {
        Console.WriteLine($"Processing order {order.Id}");
        return await ProcessOrderInternal(order);
    }
    
    protected abstract Task<bool> ProcessOrderInternal(Order order);
}

// Concrete processors that follow LSP
public class StandardOrderProcessor : OrderProcessorBase
{
    protected override Task<bool> ProcessOrderInternal(Order order)
    {
        Console.WriteLine("📦 Standard processing - 3-5 business days");
        return Task.FromResult(true);
    }
}

public class ExpressOrderProcessor : OrderProcessorBase
{
    protected override Task<bool> ProcessOrderInternal(Order order)
    {
        Console.WriteLine("⚡ Express processing - next day delivery");
        return Task.FromResult(true);
    }
}

// Usage example showing LSP in action
public class OrderManager
{
    public async Task ProcessWithAnyProcessor(Order order, OrderProcessorBase processor)
    {
        // LSP: Any processor can be substituted here
        var result = await processor.ProcessOrder(order);
        Console.WriteLine($"Order processing result: {result}");
    }
}
```

**Code Explanation:**
LSP is demonstrated by the ability to substitute any `OrderProcessorBase` implementation without breaking the system. Both `StandardOrderProcessor` and `ExpressOrderProcessor` can be used interchangeably in the `ProcessWithAnyProcessor` method.

## Complete System Demo

Here's how all the pieces work together:

```csharp
public class ECommerceDemo
{
    public static async Task RunDemo()
    {
        // Set up dependencies (DIP)
        var paymentProcessor = new CreditCardProcessor();
        var orderNotifier = new EmailNotifier();
        var paymentNotifier = new EmailNotifier();
        var orderRepository = new InMemoryOrderRepository();
        
        // Create order service with injected dependencies
        var orderService = new OrderService(
            paymentProcessor, 
            orderNotifier, 
            paymentNotifier, 
            orderRepository);
        
        // Process an order
        var items = new List<(string, decimal, int)>
        {
            ("laptop", 999.99m, 1),
            ("mouse", 29.99m, 2)
        };
        
        var orderId = await orderService.ProcessOrder("customer123", items);
        Console.WriteLine($"✅ Order {orderId} completed successfully!");
    }
}
```

**Code Explanation:**
This demo shows all SOLID principles working together:
- **SRP**: Each class has one responsibility
- **OCP**: New payment methods can be added without changing existing code
- **LSP**: Different processors can be substituted
- **ISP**: Notifiers implement only the interfaces they need
- **DIP**: High-level OrderService depends on abstractions

## SOLID Principles Summary

| Principle | How It's Applied | Benefit |
|-----------|------------------|---------|
| **SRP** | Each class has one responsibility | Easy to understand and maintain |
| **OCP** | New features added without changing existing code | Safe to extend |
| **LSP** | Derived classes can substitute base classes | Predictable behavior |
| **ISP** | Focused, specific interfaces | Flexible implementations |
| **DIP** | Depend on abstractions, use dependency injection | Testable and flexible |

## Key Takeaways

✅ **SOLID principles work best when used together**  
✅ **Real-world systems naturally benefit from SOLID design**  
✅ **Dependency injection makes systems flexible and testable**  
✅ **Good design makes adding features easier and safer**

---

*This complete system demonstrates that SOLID principles aren't academic concepts - they're practical tools for building maintainable, extensible software that stands the test of time.*