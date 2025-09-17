# Chapter 2: Encapsulation - Protecting Your Data

## Child-Friendly Explanation
Imagine your bedroom has a treasure box with a special lock. You can put things in and take things out, but only you have the key! Other people can't just grab your treasures directly - they have to ask you nicely. Encapsulation in programming is like that treasure box - it keeps the important parts of your code safe and lets you control how other parts of the program can use them.

```python
class TreasureBox:
    def __init__(self):
        self.__treasures = []  # Double underscore makes it "private"
    
    def add_treasure(self, treasure):
        if treasure and len(treasure) > 0:  # Validation - only real treasures!
            self.__treasures.append(treasure)
            print(f"Added {treasure} to treasure box")
        else:
            print("That's not a real treasure!")
    
    def peek_inside(self):
        return f"You have {len(self.__treasures)} treasures"

# You can't access treasures directly - the box protects them!
my_box = TreasureBox()
my_box.add_treasure("Golden coin")
print(my_box.peek_inside())  # You have 1 treasures
# my_box.__treasures  # This would cause an error - protected!
```

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
```cpp
// Anyone can break our temperature sensor!
struct TemperatureSensor {
    double temperature;  // Public - no protection!
};

TemperatureSensor sensor;
sensor.temperature = -500;  // Impossible temperature - no validation!
```

**With Encapsulation (Good):**
```cpp
class TemperatureSensor {
private:
    double temperature;
    
public:
    void setTemperature(double temp) {
        if (temp >= -273.15) {  // Absolute zero validation
            temperature = temp;
            cout << "Temperature set to " << temp << "°C" << endl;
        } else {
            cout << "Error: Invalid temperature!" << endl;
        }
    }
    
    double getTemperature() const {
        return temperature;
    }
};

TemperatureSensor sensor;
sensor.setTemperature(25);    // Temperature set to 25°C
sensor.setTemperature(-500);  // Error: Invalid temperature!
```

### Properties vs Direct Field Access

Modern languages provide elegant ways to implement encapsulation:

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
            {
                age = value;
            }
            else 
            {
                throw new ArgumentException("Age must be between 0 and 150");
            }
        }
    }
    
    // Auto-property for simple cases
    public string Name { get; set; }
}

Person person = new Person();
person.Age = 25;        // Uses setter validation
person.Age = -5;        // Throws exception
```

```python
class Circle:
    def __init__(self, radius):
        self._radius = radius  # Convention: single underscore = "protected"
    
    @property
    def radius(self):
        return self._radius
    
    @radius.setter
    def radius(self, value):
        if value <= 0:
            raise ValueError("Radius must be positive")
        self._radius = value
    
    @property
    def area(self):
        return 3.14159 * self._radius ** 2
    
    @property  
    def diameter(self):
        return 2 * self._radius

circle = Circle(5)
print(f"Area: {circle.area}")     # Calculated property
circle.radius = 10                # Validated assignment
# circle.radius = -1              # Would raise ValueError
```

### Encapsulation in Collections

Protecting internal collections is crucial:

```cpp
class Playlist {
private:
    vector<string> songs;
    
public:
    // DON'T DO THIS - breaks encapsulation!
    // vector<string>& getSongs() { return songs; }
    
    // DO THIS - controlled access
    void addSong(const string& song) {
        if (!song.empty()) {
            songs.push_back(song);
            cout << "Added: " << song << endl;
        }
    }
    
    void removeSong(const string& song) {
        auto it = find(songs.begin(), songs.end(), song);
        if (it != songs.end()) {
            songs.erase(it);
            cout << "Removed: " << song << endl;
        }
    }
    
    // Safe way to provide read access
    vector<string> getSongsCopy() const {
        return songs;  // Returns a copy, not reference
    }
    
    size_t getSongCount() const {
        return songs.size();
    }
    
    bool hasSong(const string& song) const {
        return find(songs.begin(), songs.end(), song) != songs.end();
    }
};
```

## Code Examples

The following examples demonstrate encapsulation with practical validation and controlled access patterns.

### Real-World Encapsulation Benefits

**1. Data Integrity**
```python
class BankAccount:
    def __init__(self, initial_balance=0):
        self.__balance = max(0, initial_balance)  # No negative starting balance
        self.__transactions = []
    
    def deposit(self, amount):
        if amount > 0:
            self.__balance += amount
            self.__transactions.append(f"Deposited ${amount}")
            return True
        return False
    
    def withdraw(self, amount):
        if 0 < amount <= self.__balance:
            self.__balance -= amount
            self.__transactions.append(f"Withdrew ${amount}")
            return True
        return False  # Insufficient funds or invalid amount
    
    def get_balance(self):
        return self.__balance  # Read-only access
    
    def get_statement(self):
        return self.__transactions.copy()  # Safe copy, not original

# Usage - the class protects itself from invalid operations
account = BankAccount(100)
account.deposit(50)      # ✓ Valid
account.withdraw(200)    # ✗ Invalid - insufficient funds
account.withdraw(-10)    # ✗ Invalid - negative amount
```

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
    {
        // Public interface stays the same regardless of internal changes
        return provider.Send(to, subject, body);
    }
}
```

## Exercises

1. **Easy**: Create a `Counter` class that can only count up (no negative numbers allowed).
   ```python
   class Counter:
       def __init__(self):
           self.__count = 0  # Private field
       
       def increment(self):
           # Your code here - only allow positive changes
           pass
       
       def get_value(self):
           # Your code here - safe read access
           pass
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
   ```cpp
   class BankVault {
   private:
       vector<string> contents;
       bool biometricAuthenticated = false;
       bool pinVerified = false;
       bool keyInserted = false;
       
   public:
       bool authenticateBiometric(string fingerprint);
       bool verifyPin(int pin);
       bool insertKey(string keyId);
       vector<string> getContents();  // Only works if all auth steps passed
   };
   ```

## Chapter Checklist

After completing this chapter, you should be able to:

- [ ] Understand the purpose and benefits of encapsulation
- [ ] Use access modifiers (private, protected, public) appropriately
- [ ] Create properties/getters and setters with validation
- [ ] Identify when data should be private vs. public
- [ ] Protect object state from invalid modifications
- [ ] Design classes with controlled interfaces
- [ ] Recognize violations of encapsulation in code
- [ ] Balance between security and usability in class design