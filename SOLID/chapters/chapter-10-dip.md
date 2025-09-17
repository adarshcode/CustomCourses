# Chapter 10: Dependency Inversion Principle (DIP)

> **"High-level modules should not depend on low-level modules. Both should depend on abstractions."** — Robert C. Martin

## 🎯 Learning Objectives

By the end of this chapter, you will:
- Understand what Dependency Inversion Principle means
- Recognize code that violates DIP
- Learn how to apply DIP to create flexible, testable code
- Practice writing code that depends on abstractions, not concretions

---

## 🧸 Kid-Friendly Explanation

Imagine you have a toy car that only works with one specific type of battery brand. If that brand stops being made, your car becomes useless! That's like having dependencies on specific things.

The Dependency Inversion Principle is like making your toy car work with ANY type of battery that fits. Instead of depending on one specific brand, you depend on the idea that "it's a battery that gives power."

In programming, instead of your code depending on specific classes, it should depend on "contracts" (interfaces) that describe what something can do.

```csharp
// Bad: Toy car only works with SpecificBattery
public class ToyCar
{
    private SpecificBattery battery;
    
    public ToyCar()
    {
        battery = new SpecificBattery();  // Too specific!
    }
}

// Good: Toy car works with any battery that provides power
public class ToyCar
{
    private IPowerSource battery;
    
    public ToyCar(IPowerSource battery)
    {
        this.battery = battery;  // Any power source works!
    }
}
```

---

## 👨‍💻 Developer Explanation

The **Dependency Inversion Principle (DIP)** is the "D" in SOLID. It states:

1. **High-level modules should not depend on low-level modules.** Both should depend on abstractions.
2. **Abstractions should not depend on details.** Details should depend on abstractions.

### What Does This Mean?

- **High-level modules**: Business logic, orchestration, complex operations
- **Low-level modules**: Database access, file I/O, external APIs, utility functions
- **Abstractions**: Interfaces, abstract classes that define contracts
- **Details**: Concrete implementations of those abstractions

### Why DIP Matters

**Without DIP:**
```csharp
// High-level module depends directly on low-level module
public class OrderService  // High-level
{
    private SqlDatabase db = new SqlDatabase();  // Low-level dependency!
    
    public void ProcessOrder(Order order)
    {
        // Business logic mixed with specific database technology
        db.SaveOrder(order);
    }
}
```

**Problems:**
- Hard to test (requires real database)
- Hard to change (switching to MongoDB requires rewriting OrderService)
- Tight coupling between business logic and implementation details

**With DIP:**
```csharp
// Both depend on abstraction
public interface IOrderRepository  // Abstraction
{
    void SaveOrder(Order order);
}

public class OrderService  // High-level
{
    private readonly IOrderRepository repository;
    
    public OrderService(IOrderRepository repository)  // Depends on abstraction
    {
        this.repository = repository;
    }
    
    public void ProcessOrder(Order order)
    {
        // Pure business logic
        repository.SaveOrder(order);
    }
}

public class SqlOrderRepository : IOrderRepository  // Low-level, depends on abstraction
{
    public void SaveOrder(Order order) { /* SQL implementation */ }
}
```

**Benefits:**
- Easy to test (can inject mock repository)
- Easy to change (can switch to NoSQL without touching OrderService)
- Business logic is pure and focused

---

## 🔧 Code Examples

Let's see DIP in action with a notification system:

### ❌ DIP Violation Example

```csharp
// Low-level modules (concrete implementations)
public class EmailSender
{
    public void SendEmail(string message) { /* Send email */ }
}

public class SmsSender
{
    public void SendSms(string message) { /* Send SMS */ }
}

// High-level module depends directly on low-level modules
public class NotificationService  // Violates DIP!
{
    private EmailSender emailSender = new EmailSender();  // Direct dependency
    private SmsSender smsSender = new SmsSender();        // Direct dependency
    
    public void SendUrgentNotification(string message)
    {
        // Business logic tied to specific implementations
        emailSender.SendEmail(message);
        smsSender.SendSms(message);
    }
}
```

**Problems with this approach:**
- `NotificationService` is tightly coupled to specific sender implementations
- Hard to add new notification methods (push notifications, Slack, etc.)
- Difficult to test without actually sending emails/SMS
- Violates Open/Closed Principle too

### ✅ DIP Compliant Example

```csharp
// Abstraction (interface)
public interface INotificationSender
{
    void Send(string message);
}

// Low-level modules depend on abstraction
public class EmailSender : INotificationSender
{
    public void Send(string message)
    {
        Console.WriteLine($"Sending email: {message}");
        // Actual email sending logic
    }
}

public class SmsSender : INotificationSender
{
    public void Send(string message)
    {
        Console.WriteLine($"Sending SMS: {message}");
        // Actual SMS sending logic
    }
}

public class PushNotificationSender : INotificationSender
{
    public void Send(string message)
    {
        Console.WriteLine($"Sending push notification: {message}");
        // Push notification logic
    }
}

// High-level module depends on abstraction
public class NotificationService
{
    private readonly List<INotificationSender> senders;
    
    public NotificationService(List<INotificationSender> senders)
    {
        this.senders = senders;  // Dependency injected through constructor
    }
    
    public void SendUrgentNotification(string message)
    {
        // Pure business logic - doesn't know about specific implementations
        foreach (var sender in senders)
        {
            sender.Send($"URGENT: {message}");
        }
    }
}

// Usage with Dependency Injection
var notificationSenders = new List<INotificationSender>
{
    new EmailSender(),
    new SmsSender(),
    new PushNotificationSender()
};

var notificationService = new NotificationService(notificationSenders);
notificationService.SendUrgentNotification("Server is down!");
```

### Common DIP Patterns

#### 1. Constructor Injection
```csharp
public class OrderService
{
    private readonly IPaymentProcessor processor;
    
    public OrderService(IPaymentProcessor processor)  // Dependency injected
    {
        this.processor = processor;
    }
}
```

#### 2. Factory Pattern with DIP
```csharp
public interface IPaymentProcessorFactory
{
    IPaymentProcessor CreateProcessor(PaymentType type);
}

public class OrderService
{
    private readonly IPaymentProcessorFactory factory;
    
    public OrderService(IPaymentProcessorFactory factory)
    {
        this.factory = factory;
    }
    
    public void ProcessPayment(Order order)
    {
        var processor = factory.CreateProcessor(order.PaymentType);
        processor.ProcessPayment(order.Amount);
    }
}
```

---

## 🎯 Exercises

### Exercise 1: Fix the Logger Violation

```csharp
// This violates DIP - fix it!
public class UserService
{
    private FileLogger logger = new FileLogger();  // Direct dependency
    
    public void CreateUser(User user)
    {
        // Validation logic
        if (string.IsNullOrEmpty(user.Email))
        {
            logger.LogError("Email is required");
            return;
        }
        
        // Save user logic
        logger.LogInfo($"User {user.Email} created successfully");
    }
}

public class FileLogger
{
    public void LogInfo(string message) { /* Write to file */ }
    public void LogError(string message) { /* Write to file */ }
}
```

**Your task:** Refactor this to follow DIP. Create an abstraction and use dependency injection.

### Exercise 2: Payment System

Create a payment processing system that follows DIP:
- Support multiple payment methods (Credit Card, PayPal, Bank Transfer)
- Payment processor should not depend on specific payment implementations
- Should be easy to add new payment methods without changing existing code

### Exercise 3: Data Access Layer

Design a data access layer that follows DIP:
- Support multiple databases (SQL Server, MySQL, MongoDB)
- Business logic should not know about specific database implementations
- Should be easy to switch databases for testing

---

## ✅ Checklist

- [ ] **Identify Dependencies**: Can you spot high-level modules depending directly on low-level modules?
- [ ] **Create Abstractions**: Have you defined interfaces for dependencies?
- [ ] **Use Dependency Injection**: Are dependencies passed in rather than created internally?
- [ ] **Test Isolation**: Can you easily test components in isolation?
- [ ] **Flexible Design**: Can you swap implementations without changing high-level code?
- [ ] **Consistent Abstractions**: Do your interfaces represent stable contracts?

---

## 🎯 Key Takeaways

1. **Depend on abstractions, not concretions** - Use interfaces instead of concrete classes
2. **Inject dependencies** - Don't create them inside your classes
3. **Keep business logic pure** - High-level modules should focus on business rules
4. **Make testing easy** - DIP enables easy mocking and testing
5. **Enable flexibility** - New implementations can be added without changing existing code

The Dependency Inversion Principle is the foundation of testable, maintainable, and flexible software architecture. It enables loose coupling and makes your code more adaptable to change.

---

**Next Chapter:** [Chapter 11: Essential Design Patterns](./chapter-11-design-patterns.md)