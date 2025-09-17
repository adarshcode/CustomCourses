# Chapter 11: Essential Design Patterns

## Child-Friendly Explanation 🧸
Think about how you organize your toys. You might have special boxes for different types - one for LEGOs, one for cars, one for dolls. Design patterns are like having special "toy organization systems" that work really well for specific situations. Just like you don't need to invent a new way to organize toys every time (you can use the systems that work!), programmers use design patterns - tried-and-true ways to solve common problems that come up again and again.

## Developer-Level Explanation 👨‍💻
Design patterns are reusable solutions to common problems in software design. They represent blueprints that you can customize to solve a recurring design problem in your code. Think of design patterns as tried-and-tested recipes for building software - they help you solve common problems while naturally following SOLID principles.

**Why Use Design Patterns?**
- **Proven Solutions**: Battle-tested approaches to common problems
- **Communication**: Shared vocabulary among developers
- **SOLID Compliance**: Patterns naturally follow good design principles
- **Maintainability**: Well-structured, predictable code

## Four Essential Patterns

### 1. Observer Pattern 👀
**Problem**: How do you notify multiple objects when something changes without tightly coupling them?

**Solution**: Create a subscription system where observers register interest in a subject's events.

```csharp
// Observer interface - defines how to receive notifications
public interface IObserver
{
    void Update(string message);
}

// Subject interface - defines subscription management
public interface ISubject
{
    void Subscribe(IObserver observer);
    void Unsubscribe(IObserver observer);
    void NotifyObservers(string message);
}

// Concrete observer - email notification
public class EmailNotifier : IObserver
{
    private string email;
    
    public EmailNotifier(string email) => this.email = email;
    
    public void Update(string message)
    {
        Console.WriteLine($"📧 Email to {email}: {message}");
    }
}

// Concrete observer - SMS notification
public class SmsNotifier : IObserver
{
    private string phone;
    
    public SmsNotifier(string phone) => this.phone = phone;
    
    public void Update(string message)
    {
        Console.WriteLine($"📱 SMS to {phone}: {message}");
    }
}

// Concrete subject - newsletter
public class Newsletter : ISubject
{
    private List<IObserver> subscribers = new List<IObserver>();
    
    public void Subscribe(IObserver observer) => subscribers.Add(observer);
    
    public void Unsubscribe(IObserver observer) => subscribers.Remove(observer);
    
    public void NotifyObservers(string message)
    {
        foreach (var subscriber in subscribers)
            subscriber.Update(message);
    }
    
    public void PublishArticle(string title)
    {
        Console.WriteLine($"📰 Published: {title}");
        NotifyObservers($"New article: {title}");
    }
}
```

**Code Explanation:**
This Observer pattern implementation demonstrates several key concepts:

1. **Loose Coupling**: The `Newsletter` class doesn't know the specific types of its subscribers - it only knows they implement `IObserver`. This means we can add new notification types without changing the Newsletter code.

2. **Open-Closed Principle**: We can extend the system with new observer types (like `WebNotifier` or `SlackNotifier`) without modifying existing code.

3. **Single Responsibility**: Each class has one job - `EmailNotifier` handles email notifications, `SmsNotifier` handles SMS, and `Newsletter` manages the publication process.

4. **How the pattern works**: When `PublishArticle()` is called, it automatically notifies all subscribers through the `NotifyObservers()` method, which loops through all registered observers and calls their `Update()` method.

### 2. Strategy Pattern 🎯
**Problem**: How do you switch between different algorithms or behaviors at runtime?

**Solution**: Encapsulate each algorithm in its own class and make them interchangeable.

```csharp
// Strategy interface - defines the contract for payment methods
public interface IPaymentStrategy
{
    void ProcessPayment(decimal amount);
}

// Concrete strategies - different payment methods
public class CreditCardPayment : IPaymentStrategy
{
    private string cardNumber;
    
    public CreditCardPayment(string cardNumber) => this.cardNumber = cardNumber;
    
    public void ProcessPayment(decimal amount)
    {
        Console.WriteLine($"💳 Charged ${amount} to card ending in {cardNumber.Substring(cardNumber.Length - 4)}");
    }
}

public class PayPalPayment : IPaymentStrategy
{
    private string email;
    
    public PayPalPayment(string email) => this.email = email;
    
    public void ProcessPayment(decimal amount)
    {
        Console.WriteLine($"🅿️ Charged ${amount} via PayPal account {email}");
    }
}

// Context class - uses the strategy
public class ShoppingCart
{
    private IPaymentStrategy paymentStrategy;
    private decimal total;
    
    public void SetPaymentStrategy(IPaymentStrategy strategy) => paymentStrategy = strategy;
    
    public void AddItem(string item, decimal price)
    {
        total += price;
        Console.WriteLine($"🛒 Added {item}: ${price} (Total: ${total})");
    }
    
    public void Checkout()
    {
        if (paymentStrategy == null)
        {
            Console.WriteLine("❌ Please select a payment method");
            return;
        }
        
        paymentStrategy.ProcessPayment(total);
        Console.WriteLine("✅ Payment successful!");
        total = 0;
    }
}
```

**Code Explanation:**
The Strategy pattern demonstrates these concepts:

1. **Runtime Behavior Switching**: The `ShoppingCart` can switch payment methods during execution using `SetPaymentStrategy()`. This allows the same cart to process credit card payments for one customer and PayPal for another.

2. **Interface Segregation**: The `IPaymentStrategy` interface is focused and minimal - it only defines what's needed for payment processing.

3. **Dependency Inversion**: The `ShoppingCart` depends on the `IPaymentStrategy` abstraction, not concrete payment implementations. This means adding new payment methods (like Bitcoin or Apple Pay) won't require changing the cart code.

4. **How it works**: The cart delegates payment processing to whatever strategy is currently set. When `Checkout()` is called, it uses the current strategy's `ProcessPayment()` method, allowing different payment behaviors without changing the cart's logic.

### 3. Factory Pattern 🏭
**Problem**: How do you create objects without specifying their exact class?

**Solution**: Use a factory class to encapsulate object creation logic.

```csharp
// Product interface - what the factory produces
public interface ILogger
{
    void Log(string message);
}

// Concrete products - different types of loggers
public class FileLogger : ILogger
{
    public void Log(string message)
    {
        Console.WriteLine($"📄 File Log: {DateTime.Now} - {message}");
    }
}

public class DatabaseLogger : ILogger
{
    public void Log(string message)
    {
        Console.WriteLine($"🗄️ DB Log: {DateTime.Now} - {message}");
    }
}

public class ConsoleLogger : ILogger
{
    public void Log(string message)
    {
        Console.WriteLine($"🖥️ Console Log: {DateTime.Now} - {message}");
    }
}

// Factory class - creates appropriate logger
public static class LoggerFactory
{
    public static ILogger CreateLogger(string type)
    {
        return type.ToLower() switch
        {
            "file" => new FileLogger(),
            "database" => new DatabaseLogger(),
            "console" => new ConsoleLogger(),
            _ => throw new ArgumentException($"Unknown logger type: {type}")
        };
    }
}

// Usage
public class Application
{
    private ILogger logger;
    
    public Application(string loggerType)
    {
        logger = LoggerFactory.CreateLogger(loggerType);
    }
    
    public void DoSomething()
    {
        logger.Log("Application started");
        logger.Log("Performing some operation");
        logger.Log("Application finished");
    }
}
```

**Code Explanation:**
The Factory pattern shows these important concepts:

1. **Centralized Creation Logic**: The `LoggerFactory` centralizes all object creation logic. If we need to add configuration or change how loggers are created, we only modify the factory.

2. **Abstraction of Object Creation**: The `Application` class doesn't need to know how to create specific logger types - it just asks the factory for a logger of a certain type.

3. **Open-Closed Principle**: Adding new logger types (like `EmailLogger` or `SlackLogger`) only requires adding them to the factory's switch statement - no changes to client code.

4. **How it works**: The factory uses the C# switch expression to determine which concrete logger to create based on the input string. The client receives an `ILogger` interface, so it can use any logger type the same way.

### 4. Decorator Pattern 🎨
**Problem**: How do you add new functionality to objects without changing their structure?

**Solution**: Wrap objects in decorator classes that add behavior while maintaining the same interface.

```csharp
// Component interface - basic coffee functionality
public interface ICoffee
{
    string GetDescription();
    decimal GetCost();
}

// Concrete component - basic coffee
public class SimpleCoffee : ICoffee
{
    public string GetDescription() => "Simple coffee";
    public decimal GetCost() => 2.00m;
}

// Base decorator
public abstract class CoffeeDecorator : ICoffee
{
    protected ICoffee coffee;
    
    public CoffeeDecorator(ICoffee coffee) => this.coffee = coffee;
    
    public virtual string GetDescription() => coffee.GetDescription();
    public virtual decimal GetCost() => coffee.GetCost();
}

// Concrete decorators - add-ons
public class MilkDecorator : CoffeeDecorator
{
    public MilkDecorator(ICoffee coffee) : base(coffee) { }
    
    public override string GetDescription() => coffee.GetDescription() + ", Milk";
    public override decimal GetCost() => coffee.GetCost() + 0.50m;
}

public class SugarDecorator : CoffeeDecorator
{
    public SugarDecorator(ICoffee coffee) : base(coffee) { }
    
    public override string GetDescription() => coffee.GetDescription() + ", Sugar";
    public override decimal GetCost() => coffee.GetCost() + 0.25m;
}

// Usage example
public class CoffeeShop
{
    public static void OrderCoffee()
    {
        ICoffee coffee = new SimpleCoffee();
        Console.WriteLine($"{coffee.GetDescription()}: ${coffee.GetCost()}");
        
        // Add milk
        coffee = new MilkDecorator(coffee);
        Console.WriteLine($"{coffee.GetDescription()}: ${coffee.GetCost()}");
        
        // Add sugar
        coffee = new SugarDecorator(coffee);
        Console.WriteLine($"{coffee.GetDescription()}: ${coffee.GetCost()}");
    }
}
```

**Code Explanation:**
The Decorator pattern demonstrates these concepts:

1. **Composition over Inheritance**: Instead of creating subclasses for every combination (MilkCoffee, SugarCoffee, MilkSugarCoffee), we compose decorators. This allows unlimited combinations without class explosion.

2. **Transparent Wrapping**: Each decorator implements the same `ICoffee` interface as the object it wraps. This means decorated objects can be used anywhere the original object could be used.

3. **Flexible Feature Addition**: We can add multiple decorators in any order: `new SugarDecorator(new MilkDecorator(new SimpleCoffee()))` creates coffee with both milk and sugar.

4. **How it works**: Each decorator holds a reference to an `ICoffee` object and adds its own behavior to the methods. When `GetDescription()` is called, each decorator adds its part to the description and passes the call down the chain.

## Pattern Selection Guide

| When you need to... | Use Pattern | Key Benefit |
|---------------------|-------------|-------------|
| Notify multiple objects of changes | Observer | Loose coupling between subjects and observers |
| Switch algorithms at runtime | Strategy | Interchangeable behaviors |
| Create objects without knowing exact types | Factory | Centralized object creation |
| Add features without changing structure | Decorator | Flexible feature composition |

## Key Takeaways

✅ **Design patterns solve common problems with proven solutions**  
✅ **Patterns naturally follow SOLID principles**  
✅ **Use patterns to make code more maintainable and extensible**  
✅ **Don't force patterns - use them when they solve real problems**

---

*These four patterns form the foundation for most design solutions. Master them, and you'll recognize opportunities to apply SOLID principles naturally in your code.*