# Chapter 2: Encapsulation - Protecting Your Data

## Child-Friendly Explanation 🧸
Imagine your bedroom has a treasure box with a special lock. You can put things in and take things out, but only you have the key! Other people can't just grab your treasures directly - they have to ask you nicely. Encapsulation in programming is like that treasure box - it keeps the important parts of your code safe and lets you control how other parts of the program can use them.

```csharp
public class TreasureBox
{
    private List<string> treasures = new List<string>();  // Private - protected data!
    
    public void AddTreasure(string treasure)
    {
        if (!string.IsNullOrEmpty(treasure))  // Validation - only real treasures!
        {
            treasures.Add(treasure);
            Console.WriteLine($"Added {treasure} to treasure box");
        }
        else
        {
            Console.WriteLine("That's not a real treasure!");
        }
    }
    
    public string PeekInside()
    {
        return $"You have {treasures.Count} treasures";
    }
}

// You can't access treasures directly - the box protects them!
var myBox = new TreasureBox();
myBox.AddTreasure("Golden coin");
Console.WriteLine(myBox.PeekInside());  // You have 1 treasures
// myBox.treasures;  // This would cause an error - protected!
```

**Code Explanation:**
This example demonstrates **data hiding** through the `private` keyword. The `treasures` list is protected from direct access, and all interactions must go through controlled methods (`AddTreasure`, `PeekInside`) that can validate inputs and maintain data integrity.

## Developer-Level Explanation
Encapsulation is the principle of bundling data (attributes) and methods (functions) that operate on that data into a single unit (class), while restricting direct access to some of the object's components. This is achieved through access modifiers (private, protected, public) and provides controlled access via methods (getters/setters).

**Why encapsulation matters:**
- **Data Protection**: Prevents external code from corrupting object state
- **Controlled Access**: Enables validation and business logic in data access
- **Maintainability**: Internal implementation can change without affecting external code
- **Debugging**: Easier to track where and how data is modified

**Common pitfalls:**
- Making everything public "for convenience"
- Creating getters/setters for every private field without thinking about necessity
- Forgetting to validate input in setter methods
- Breaking encapsulation by returning mutable references to internal collections

### Access Modifiers: The Security Guards

Think of access modifiers as different levels of security:

```csharp
public class BankAccount 
{
    private double balance;           // Private - only THIS class can access
    protected string accountType;     // Protected - this class and children
    public string accountNumber;      // Public - anyone can access
    
    public BankAccount(string accNum, double initialBalance) 
    {
        accountNumber = accNum;       // Public - direct access OK
        accountType = "Standard";     // Protected - direct access OK in class
        SetBalance(initialBalance);   // Private - must use method
    }
    
    private void SetBalance(double newBalance) 
    {
        if (newBalance >= 0) {        // Validation protects data integrity
            balance = newBalance;
        }
    }
    
    public double GetBalance() 
    {
        return balance;               // Controlled read access
    }
}
```

### The Problem Encapsulation Solves

**Without Encapsulation (Bad):**
```csharp
// ❌ Anyone can break our temperature sensor!
public class TemperatureSensor 
{
    public double Temperature;  // Public - no protection!
}

var sensor = new TemperatureSensor();
sensor.Temperature = -500;  // Impossible temperature - no validation!
```

**With Encapsulation (Good):**
```csharp
// ✅ Protected data with controlled access
public class TemperatureSensor 
{
    private double temperature;  // Private - protected data
    
    public void SetTemperature(double temp) 
    {
        if (temp >= -273.15)  // Absolute zero validation
        {
            temperature = temp;
            Console.WriteLine($"Temperature set to {temp}°C");
        } 
        else 
        {
            Console.WriteLine("Error: Invalid temperature!");
        }
    }
    
    public double GetTemperature() 
    {
        return temperature;
    }
}

var sensor = new TemperatureSensor();
sensor.SetTemperature(25);    // Temperature set to 25°C
sensor.SetTemperature(-500);  // Error: Invalid temperature!
```

**Code Explanation:**
The first example shows how direct field access can lead to invalid states. The second example uses **encapsulation** to protect the temperature field and ensure only valid values are stored through validation in the `SetTemperature` method.

### Properties vs Direct Field Access

Modern C# provides elegant ways to implement encapsulation:

```csharp
public class Person 
{
    private int age;
    
    // Property with validation - looks like a field, acts like methods
    public int Age 
    {
        get { return age; }
        set 
        {
            if (value >= 0 && value <= 150) 
    
    // Auto-property for simple cases (no validation needed)
    public string Name { get; set; }
}

var person = new Person();
person.Age = 25;        // Uses setter validation
person.Age = -5;        // Throws exception
```

**Code Explanation:**
C# properties provide a clean syntax for encapsulation. The `Age` property looks like a field to users but internally uses get/set methods with validation. Auto-properties like `Name` provide encapsulation without extra code when validation isn't needed.

### Advanced Property Examples

```csharp
public class Circle
{
    private double radius;
    
    public double Radius
    {
        get { return radius; }
        set 
        {
            if (value <= 0)
                throw new ArgumentException("Radius must be positive");
            radius = value;
        }
    }
    
    // Calculated properties - computed from other data
    public double Area => Math.PI * radius * radius;
    public double Diameter => 2 * radius;
    
    public Circle(double radius)
    {
        Radius = radius;  // Uses property validation
    }
}

var circle = new Circle(5);
Console.WriteLine($"Area: {circle.Area:F2}");  // Calculated property
circle.Radius = 10;                             // Validated assignment
// circle.Radius = -1;                          // Would throw exception
```

**Code Explanation:**
This demonstrates **calculated properties** (`Area`, `Diameter`) that are computed from other data rather than stored. The `=>` syntax creates read-only properties that automatically recalculate when the underlying data changes.

### Encapsulation in Collections

Protecting internal collections is crucial for maintaining object integrity:

```csharp
public class Playlist 
{
    private List<string> songs = new List<string>();
    
    // ❌ DON'T DO THIS - breaks encapsulation!
    // public List<string> Songs { get { return songs; } }
    
    // ✅ DO THIS - controlled access
    public void AddSong(string song) 
    {
        if (!string.IsNullOrWhiteSpace(song)) 
        {
            songs.Add(song);
            Console.WriteLine($"Added: {song}");
        }
    }
    
    public void RemoveSong(string song) 
    {
        if (songs.Remove(song)) 
        {
            Console.WriteLine($"Removed: {song}");
        }
    }
    
    // Safe way to provide read access - returns copy
    public List<string> GetSongsCopy() 
    {
        return new List<string>(songs);  // Returns a copy, not reference
    }
    
    // Better: provide specific queries instead of raw data
    public int SongCount => songs.Count;
    public bool HasSong(string song) => songs.Contains(song);
    public string GetSong(int index) => index >= 0 && index < songs.Count ? songs[index] : null;
}
```

**Code Explanation:**
This example shows how to properly encapsulate collections. Direct access to the internal list would allow external code to modify it without validation. Instead, we provide controlled methods (`AddSong`, `RemoveSong`) and safe read access through copies or specific queries.

## Practical Code Examples

The following examples demonstrate encapsulation with practical validation and controlled access patterns.

### Real-World Encapsulation Benefits

**1. Data Integrity Protection**
```csharp
public class BankAccount
{
    private decimal balance;
    private List<string> transactions = new List<string>();
    
    public BankAccount(decimal initialBalance = 0)
    {
        balance = Math.Max(0, initialBalance);  // No negative starting balance
    }
    
    public bool Deposit(decimal amount)
    {
        if (amount > 0)
        {
            balance += amount;
            transactions.Add($"Deposited ${amount}");
            return true;
        }
        return false;
    }
    
    public bool Withdraw(decimal amount)
    {
        if (amount > 0 && amount <= balance)
        {
            balance -= amount;
            transactions.Add($"Withdrew ${amount}");
            return true;
        }
        return false;  // Insufficient funds or invalid amount
    }
    
    public decimal GetBalance()
    {
        return balance;  // Read-only access
    }
    
    public List<string> GetStatement()
    {
        return new List<string>(transactions);  // Safe copy, not original
    }
}

// Usage - the class protects itself from invalid operations
var account = new BankAccount(100);
account.Deposit(50);      // ✓ Valid
account.Withdraw(200);    // ✗ Invalid - insufficient funds
account.Withdraw(-10);    // ✗ Invalid - negative amount
```

**Code Explanation:**
This example shows how encapsulation protects **business rules**. The private `balance` field can only be modified through validated methods (`Deposit`, `Withdraw`) that enforce constraints like "no negative amounts" and "no overdrafts". External code cannot bypass these protections.

**2. Interface Stability**
```csharp
// Internal implementation can change without breaking external code
public class EmailService 
{
    private IEmailProvider provider;  // Implementation detail
    
    public EmailService() 
    {
        // Can switch between different email providers
        provider = new SMTPProvider();  // or GMailProvider(), etc.
    }
    
    public bool SendEmail(string to, string subject, string body) 
```csharp
public class EmailService 
{
    private IEmailProvider provider;  // Hidden implementation detail
    
    public EmailService(IEmailProvider emailProvider)
    {
        provider = emailProvider;
    }
    
    public bool SendEmail(string to, string subject, string body) 
    {
        // Public interface stays the same regardless of internal changes
        return provider.Send(to, subject, body);
    }
}
```

**Code Explanation:**
This demonstrates **interface stability** - the public `SendEmail` method provides a consistent interface while hiding the internal email provider implementation. The internal provider can be changed without affecting code that uses this class.

## Practice Exercises

1. **Easy**: Create a `Counter` class that can only count up (no negative numbers allowed).
   ```csharp
   public class Counter
   {
       private int count = 0;  // Private field
       
       public void Increment() 
       {
           // Your code here - only allow positive changes
       }
       
       public int GetValue() 
       {
           // Your code here - safe read access
       }
   }
   ```

2. **Medium**: Build a `Password` class that validates password strength and hides the actual password.
   ```csharp
   public class Password 
   {
       private string hashedPassword;
       
       public bool SetPassword(string newPassword) 
       {
           // Validate strength, then hash and store
           // Return true if valid, false otherwise
       }
       
       public bool VerifyPassword(string candidatePassword) 
       {
           // Check if provided password matches stored hash
       }
   }
   ```

3. **Hard**: Create a `BankVault` class that requires multiple authentication steps before allowing access to contents.
   ```csharp
   public class BankVault 
   {
       private List<string> contents = new List<string>();
       private bool biometricAuthenticated = false;
       private bool pinVerified = false;
       private bool keyInserted = false;
       
       public bool AuthenticateBiometric(string fingerprint) { /* Your code */ }
       public bool VerifyPin(int pin) { /* Your code */ }
       public bool InsertKey(string keyId) { /* Your code */ }
       public List<string> GetContents() { /* Only works if all auth steps passed */ }
   }
   ```

## Key Takeaways

✅ **Private fields protect data from invalid modifications**  
✅ **Properties provide controlled access with validation**  
✅ **Encapsulation makes code more maintainable and debuggable**  
✅ **Internal implementation can change without affecting external code**  
✅ **Collections should be protected through controlled access methods**

---

*Remember: Encapsulation is about creating well-defined boundaries. Keep data private and expose only what users actually need through well-designed public interfaces.*
- [ ] Design classes with controlled interfaces
- [ ] Recognize violations of encapsulation in code
- [ ] Balance between security and usability in class design