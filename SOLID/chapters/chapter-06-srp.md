# Chapter 6: Single Responsibility Principle (SRP)

## Child-Friendly Explanation
Imagine if your teacher had to teach math, cook lunch, drive the school bus, AND clean the classrooms all by themselves! That would be way too much work for one person, and they probably wouldn't do any of those jobs very well. The Single Responsibility Principle is like saying "each person should have just one main job to do really well" - in programming, each class should do just one thing really well too!

```python
# BAD - One class doing too many jobs (like a super-teacher!)
class SuperTeacher:
    def teach_math(self, students):
        print("Teaching math...")
    
    def cook_lunch(self, meal_plan):
        print("Cooking lunch...")
    
    def drive_bus(self, route):
        print("Driving school bus...")
    
    def clean_classroom(self, room_number):
        print("Cleaning classroom...")
    # This class has too many responsibilities!

# GOOD - Each class has one clear job
class MathTeacher:
    def teach_math(self, students):
        print("Teaching math really well!")

class Chef:
    def cook_lunch(self, meal_plan):
        print("Cooking delicious lunch!")

class BusDriver:
    def drive_bus(self, route):
        print("Safely driving school bus!")

class Janitor:
    def clean_classroom(self, room_number):
        print("Keeping classroom spotless!")

# Now each person can focus on doing their one job excellently!
```

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

**How to run:**
- C#: `csc chapter-06-basic.cs && .\chapter-06-basic.exe`
- C++: `g++ -std=c++17 chapter-06-basic.cpp -o chapter-06-basic && .\chapter-06-basic.exe`
- Python: `python chapter-06-basic.py`

**Expected Output:** Shows before/after comparison of SRP violation and correction.

### Intermediate Example

**How to run:**
- C#: `csc chapter-06-intermediate.cs && .\chapter-06-intermediate.exe`
- C++: `g++ -std=c++17 chapter-06-intermediate.cpp -o chapter-06-intermediate && .\chapter-06-intermediate.exe`
- Python: `python chapter-06-intermediate.py`

**Expected Output:** Demonstrates proper separation of concerns in an employee management system.

### Advanced Example

**How to run:**
- C#: `csc chapter-06-advanced.cs && .\chapter-06-advanced.exe`
- C++: `g++ -std=c++17 chapter-06-advanced.cpp -o chapter-06-advanced && .\chapter-06-advanced.exe`
- Python: `python chapter-06-advanced.py`

**Why this advanced solution follows SRP:**
- **Clear Responsibilities**: Each class has one well-defined purpose
- **Separation of Concerns**: Business logic, data access, and presentation are separated
- **Single Reason to Change**: Each class would only change for one specific reason
- **High Cohesion**: All methods in each class work together toward the same goal

### Real-World SRP Example: E-commerce Order System

**Violating SRP (Bad):**
```python
class Order:
    def __init__(self, customer_id, items):
        self.customer_id = customer_id
        self.items = items
        self.total = 0
        self.status = "pending"
    
    # Responsibility 1: Calculate order totals
    def calculate_total(self):
        self.total = sum(item.price * item.quantity for item in self.items)
        # Apply taxes
        self.total *= 1.08
        return self.total
    
    # Responsibility 2: Database operations
    def save_to_database(self):
        print(f"INSERT INTO orders VALUES ({self.customer_id}, {self.total})")
        for item in self.items:
            print(f"INSERT INTO order_items VALUES ({item.id}, {item.quantity})")
    
    # Responsibility 3: Email notifications
    def send_confirmation_email(self):
        print(f"Sending email to customer {self.customer_id}")
        print("Subject: Order Confirmation")
        print(f"Your order total: ${self.total}")
    
    # Responsibility 4: Inventory management
    def update_inventory(self):
        for item in self.items:
            print(f"Reducing inventory for {item.name} by {item.quantity}")
    
    # Responsibility 5: Payment processing
    def process_payment(self, credit_card):
        print(f"Processing payment of ${self.total} with card {credit_card}")
        self.status = "paid"
    
    # This class changes for 5 different reasons!
```

**Following SRP (Good):**
```python
# Each class has ONE responsibility

class Order:  # Only manages order data
    def __init__(self, customer_id, items):
        self.customer_id = customer_id
        self.items = items
        self.total = 0
        self.status = "pending"

class OrderCalculator:  # Only calculates order totals
    def __init__(self, tax_rate=0.08):
        self.tax_rate = tax_rate
    
    def calculate_total(self, order):
        subtotal = sum(item.price * item.quantity for item in order.items)
        order.total = subtotal * (1 + self.tax_rate)
        return order.total

class OrderRepository:  # Only handles database operations
    def save(self, order):
        print(f"Saving order for customer {order.customer_id} - Total: ${order.total}")
        # Database logic here
    
    def find_by_id(self, order_id):
        print(f"Loading order {order_id} from database")
        # Database retrieval logic

class EmailNotificationService:  # Only handles email notifications
    def send_order_confirmation(self, order, customer_email):
        print(f"📧 Sending confirmation to {customer_email}")
        print(f"Order total: ${order.total}")
    
    def send_shipping_notification(self, order, tracking_number):
        print(f"📧 Sending shipping notification - Tracking: {tracking_number}")

class InventoryService:  # Only manages inventory
    def update_stock(self, order):
        for item in order.items:
            print(f"📦 Reducing {item.name} stock by {item.quantity}")
    
    def check_availability(self, items):
        print("Checking item availability...")
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

```cpp
#include <gtest/gtest.h>  // Example with Google Test

// SRP makes unit testing straightforward
class TaxCalculator {
public:
    double calculateTax(double amount, double rate) {
        return amount * rate;
    }
};

class PriceFormatter {
public:
    string formatPrice(double price) {
        return "$" + to_string(static_cast<int>(price * 100) / 100.0);
    }
};

// Easy to test - each class has one clear purpose
TEST(TaxCalculatorTest, CalculatesTaxCorrectly) {
    TaxCalculator calculator;
    EXPECT_DOUBLE_EQ(10.0, calculator.calculateTax(100.0, 0.1));
}

TEST(PriceFormatterTest, FormatsCorrectly) {
    PriceFormatter formatter;
    EXPECT_EQ("$99.99", formatter.formatPrice(99.99));
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