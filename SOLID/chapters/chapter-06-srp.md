# Chapter 6: Single Responsibility Principle (SRP)

## Child-Friendly Explanation
Imagine if your teacher had to teach math, cook lunch, drive the school bus, AND clean the classrooms all by themselves! That would be way too much work for one person, and they probably wouldn't do any of those jobs very well. The Single Responsibility Principle is like saying "each person should have just one main job to do really well" - in programming, each class should do just one thing really well too!

```csharp
// BAD - One class doing too many jobs (like a super-teacher!)
public class SuperTeacher
{
    public void TeachMath(List<string> students)
    {
        Console.WriteLine("Teaching math...");
    }
    
    public void CookLunch(string mealPlan)
    {
        Console.WriteLine("Cooking lunch...");
    }
    
    public void DriveBus(string route)
    {
        Console.WriteLine("Driving school bus...");
    }
    
    public void CleanClassroom(int roomNumber)
    {
        Console.WriteLine("Cleaning classroom...");
    }
    // This class has too many responsibilities!
}

// GOOD - Each class has one clear job
public class MathTeacher
{
    public void TeachMath(List<string> students)
    {
        Console.WriteLine("Teaching math really well!");
    }
}

public class Chef
{
    public void CookLunch(string mealPlan)
    {
        Console.WriteLine("Cooking delicious lunch!");
    }
}

public class BusDriver
{
    public void DriveBus(string route)
    {
        Console.WriteLine("Safely driving school bus!");
    }
}

public class Janitor
{
    public void CleanClassroom(int roomNumber)
    {
        Console.WriteLine("Keeping classroom spotless!");
    }
}

// Now each person can focus on doing their one job excellently!
```

**Code Explanation:**
This example demonstrates **Single Responsibility Principle** by showing how the problematic `SuperTeacher` class (which handles teaching, cooking, driving, and cleaning) violates SRP because it has **multiple reasons to change**. The improved design splits these into separate classes (`MathTeacher`, `Chef`, `BusDriver`, `Janitor`), where each class has **one clear responsibility** and would only need to change for reasons related to its specific job.

## Developer-Level Explanation
The Single Responsibility Principle states that a class should have only one reason to change, meaning it should have only one job or responsibility. This is the first and arguably most important principle of SOLID design.

**Why SRP matters:**
- **Maintainability**: Classes with single responsibilities are easier to understand and modify
- **Testability**: Focused classes are easier to test thoroughly
- **Reusability**: Single-purpose classes can be reused in different contexts
- **Reduced Coupling**: Classes with clear responsibilities have fewer dependencies

### Identifying SRP Violations

**Bad Example - Multiple Responsibilities:**
```csharp
// This class violates SRP - it does too many things!
public class UserManager 
{
    // Responsibility 1: User data management
    public void CreateUser(string username, string email) { }
    public void UpdateUser(int id, string email) { }
    public void DeleteUser(int id) { }
    
    // Responsibility 2: Data persistence  
    public void SaveToDatabase(User user) { }
    public User LoadFromDatabase(int id) { }
    
    // Responsibility 3: Email notifications
    public void SendWelcomeEmail(User user) { }
    public void SendPasswordResetEmail(User user) { }
    
    // Responsibility 4: User validation
    public bool ValidateEmail(string email) { }
    public bool ValidatePassword(string password) { }
    
    // This class will change for multiple reasons:
    // - Database schema changes
    // - Email template changes  
    // - Validation rule changes
    // - User management logic changes
}
```

**Good Example - Single Responsibilities:**
```csharp
// Each class has ONE clear responsibility
public class User 
{
    public int Id { get; set; }
    public string Username { get; set; }
    public string Email { get; set; }
    // Just represents user data
}

public class UserRepository 
{
    public void Save(User user) { /* Database operations */ }
    public User FindById(int id) { /* Database operations */ }
    public void Delete(int id) { /* Database operations */ }
    // Only changes if database access needs change
}

public class UserValidator 
{
    public bool IsValidEmail(string email) { /* Validation logic */ }
    public bool IsValidPassword(string password) { /* Validation logic */ }
    // Only changes if validation rules change
}

public class EmailService 
{
    public void SendWelcomeEmail(User user) { /* Email logic */ }
    public void SendPasswordResetEmail(User user) { /* Email logic */ }
    // Only changes if email requirements change
}

public class UserService 
{
    private UserRepository repository;
    private UserValidator validator;
    private EmailService emailService;
    
    public void CreateUser(string username, string email) 
    {
        if (validator.IsValidEmail(email)) 
        {
            User user = new User { Username = username, Email = email };
            repository.Save(user);
            emailService.SendWelcomeEmail(user);
        }
    }
    // Only changes if user management workflow changes
}
```

## Code Examples

### Basic Example

**Expected Output:** Shows before/after comparison of SRP violation and correction.

### Intermediate Example

**Expected Output:** Demonstrates proper separation of concerns in an employee management system.

### Advanced Example

**Why this advanced solution follows SRP:**
- **Clear Responsibilities**: Each class has one well-defined purpose
- **Separation of Concerns**: Business logic, data access, and presentation are separated
- **Single Reason to Change**: Each class would only change for one specific reason
- **High Cohesion**: All methods in each class work together toward the same goal

### Real-World SRP Example: E-commerce Order System

**Violating SRP (Bad):**
```csharp
public class Order
{
    public int CustomerId { get; set; }
    public List<OrderItem> Items { get; set; }
    public decimal Total { get; set; }
    public string Status { get; set; }
    
    public Order(int customerId, List<OrderItem> items)
    {
        CustomerId = customerId;
        Items = items;
        Total = 0;
        Status = "pending";
    }
    
    // Responsibility 1: Calculate order totals
    public decimal CalculateTotal()
    {
        Total = Items.Sum(item => item.Price * item.Quantity);
        // Apply taxes
        Total *= 1.08m;
        return Total;
    }
    
    // Responsibility 2: Database operations
    public void SaveToDatabase()
    {
        Console.WriteLine($"INSERT INTO orders VALUES ({CustomerId}, {Total})");
        foreach (var item in Items)
        {
            Console.WriteLine($"INSERT INTO order_items VALUES ({item.Id}, {item.Quantity})");
        }
    }
    
    // Responsibility 3: Email notifications
    public void SendConfirmationEmail()
    {
        Console.WriteLine($"Sending email to customer {CustomerId}");
        Console.WriteLine("Subject: Order Confirmation");
        Console.WriteLine($"Your order total: ${Total}");
    }
    
    // Responsibility 4: Inventory management
    public void UpdateInventory()
    {
        foreach (var item in Items)
        {
            Console.WriteLine($"Reducing inventory for {item.Name} by {item.Quantity}");
        }
    }
    
    // Responsibility 5: Payment processing
    public void ProcessPayment(string creditCard)
    {
        Console.WriteLine($"Processing payment of ${Total} with card {creditCard}");
        Status = "paid";
    }
    
    // This class changes for 5 different reasons!
}
```

**Following SRP (Good):**
```csharp
// Each class has ONE responsibility

public class Order  // Only manages order data
{
    public int CustomerId { get; set; }
    public List<OrderItem> Items { get; set; }
    public decimal Total { get; set; }
    public string Status { get; set; }
    
    public Order(int customerId, List<OrderItem> items)
    {
        CustomerId = customerId;
        Items = items;
        Total = 0;
        Status = "pending";
    }
}

public class OrderCalculator  // Only calculates order totals
{
    private readonly decimal taxRate;
    
    public OrderCalculator(decimal taxRate = 0.08m)
    {
        this.taxRate = taxRate;
    }
    
    public decimal CalculateTotal(Order order)
    {
        var subtotal = order.Items.Sum(item => item.Price * item.Quantity);
        order.Total = subtotal * (1 + taxRate);
        return order.Total;
    }
}

public class OrderRepository  // Only handles database operations
{
    public void Save(Order order)
    {
        Console.WriteLine($"Saving order for customer {order.CustomerId} - Total: ${order.Total}");
        // Database logic here
    }
    
    public Order FindById(int orderId)
    {
        Console.WriteLine($"Loading order {orderId} from database");
        // Database retrieval logic
        return null; // Placeholder
    }
}

public class EmailNotificationService  // Only handles email notifications
{
    public void SendOrderConfirmation(Order order, string customerEmail)
    {
        Console.WriteLine($"📧 Sending confirmation to {customerEmail}");
        Console.WriteLine($"Order total: ${order.Total}");
    }
    
    public void SendShippingNotification(Order order, string trackingNumber)
    {
        Console.WriteLine($"📧 Sending shipping notification - Tracking: {trackingNumber}");
    }
}

public class InventoryService  // Only manages inventory
{
    public void UpdateStock(Order order)
    {
        foreach (var item in order.Items)
        {
            Console.WriteLine($"📦 Reducing {item.Name} stock by {item.Quantity}");
        }
    }
    
    public bool CheckAvailability(List<OrderItem> items)
    {
        Console.WriteLine("Checking item availability...");
        return true; // Placeholder
    }
}
        return True  # Simplified

class PaymentProcessor:  # Only processes payments
    def process_credit_card(self, amount, card_details):
        print(f"💳 Processing ${amount} payment")
        return {"success": True, "transaction_id": "TXN123"}

# Orchestrating class that coordinates the single-responsibility classes
class OrderService:
    def __init__(self, calculator, repository, email_service, inventory_service, payment_processor):
        self.calculator = calculator
        self.repository = repository
        self.email_service = email_service
        self.inventory_service = inventory_service
        self.payment_processor = payment_processor
    
    def process_order(self, order, customer_email, payment_details):
        # Each step delegates to a specialized class
        if not self.inventory_service.check_availability(order.items):
            return False
        
        self.calculator.calculate_total(order)
        
        payment_result = self.payment_processor.process_credit_card(
            order.total, payment_details
        )
        
        if payment_result["success"]:
            order.status = "paid"
            self.repository.save(order)
            self.inventory_service.update_stock(order)
            self.email_service.send_order_confirmation(order, customer_email)
            return True
        
        return False

# Usage - clean separation of concerns
from dataclasses import dataclass

@dataclass
class OrderItem:
    id: int
    name: str
    price: float
    quantity: int

# Create services
calculator = OrderCalculator(tax_rate=0.08)
repository = OrderRepository()
email_service = EmailNotificationService()
inventory_service = InventoryService()
payment_processor = PaymentProcessor()

order_service = OrderService(
    calculator, repository, email_service, 
    inventory_service, payment_processor
)

# Process an order
items = [
    OrderItem(1, "Laptop", 999.99, 1),
    OrderItem(2, "Mouse", 29.99, 2)
]
order = Order(customer_id=123, items=items)

success = order_service.process_order(
    order, 
    "customer@example.com", 
    {"card_number": "1234-5678-9012-3456"}
)

print(f"Order processed: {success}")
```

### SRP Benefits in Testing

With SRP, each class is easy to test in isolation:

```csharp
// SRP makes unit testing straightforward
public class TaxCalculator 
{
    public double CalculateTax(double amount, double rate) 
    {
        return amount * rate;
    }
}

public class PriceFormatter 
{
    public string FormatPrice(double price) 
    {
        return $"${price:F2}";
    }
}

// Easy to test - each class has one clear purpose
[Test]
public void TaxCalculator_CalculatesTaxCorrectly() 
{
    var calculator = new TaxCalculator();
    Assert.AreEqual(10.0, calculator.CalculateTax(100.0, 0.1), 0.01);
}

[Test]
public void PriceFormatter_FormatsCorrectly() 
{
    var formatter = new PriceFormatter();
    Assert.AreEqual("$99.99", formatter.FormatPrice(99.99));
}

// If we had one class doing both jobs, testing would be harder:
// - More complex setup
// - Tests would be coupled
// - Harder to isolate failures
```

## Exercises

1. **Easy**: Identify what's wrong with a `Calculator` class that can add numbers AND save results to a file.
   - *Hint: How many different reasons might this class need to change?*

2. **Medium**: Refactor a `Student` class that handles student data, calculates grades, AND prints report cards.
   - *Hint: Think about what distinct responsibilities exist here*

3. **Hard**: Design a library system that needs to manage books, handle user authentication, and generate reports - ensure each class follows SRP.
   - *Hint: Identify all the different "jobs" that need to be done*

## Chapter Checklist

After completing this chapter, you should be able to:

- [ ] Define the Single Responsibility Principle in your own words
- [ ] Identify classes that violate SRP
- [ ] Recognize symptoms of SRP violations (god classes, multiple concerns)
- [ ] Refactor code to follow SRP
- [ ] Design new classes with single, clear responsibilities
- [ ] Understand the relationship between SRP and maintainability
- [ ] Explain why "one reason to change" matters
- [ ] Balance between over-splitting and under-splitting responsibilities