# Chapter 8: Liskov Substitution Principle (LSP)

## Child-Friendly Explanation
Imagine you have a toy box, and someone tells you "put any kind of ball in this box." You should be able to put a soccer ball, basketball, tennis ball, or any other ball in there, and the box should work just fine with all of them. The Liskov Substitution Principle is like this - if your code is designed to work with a "parent" type, it should work perfectly with any "child" type too, without any surprises or problems!

```python
# All balls should work the same way in our toy box
class Ball:
    def __init__(self, size):
        self.size = size
    
    def bounce(self):
        print(f"Ball bouncing to height {self.size * 2}")
    
    def roll(self):
        print(f"Ball rolling {self.size * 3} meters")

class SoccerBall(Ball):
    def bounce(self):
        print(f"Soccer ball bouncing to height {self.size * 2}")  # Same behavior!
    
    def roll(self):
        print(f"Soccer ball rolling {self.size * 3} meters")  # Same behavior!

class Basketball(Ball):
    def bounce(self):
        print(f"Basketball bouncing to height {self.size * 2}")  # Same behavior!
    
    def roll(self):
        print(f"Basketball rolling {self.size * 3} meters")  # Same behavior!

# This toy box works with ANY ball - no surprises!
def play_with_ball(ball: Ball):
    ball.bounce()  # Works with any ball type
    ball.roll()    # Works with any ball type

# All balls work perfectly in the same toy box
soccer = SoccerBall(5)
basketball = Basketball(6)

play_with_ball(soccer)     # Works perfectly
play_with_ball(basketball) # Works perfectly too!
```

## Developer-Level Explanation
The Liskov Substitution Principle states that objects of a superclass should be replaceable with objects of its subclasses without breaking the application. In other words, if S is a subtype of T, then objects of type T may be replaced with objects of type S without altering the correctness of the program.

**Why LSP matters:**
- **Reliability**: Code behaves predictably when working with inheritance hierarchies
- **Polymorphism**: Enables safe use of polymorphism without unexpected behavior
- **Contract Compliance**: Ensures that subclasses honor the contracts established by their parent classes
- **Maintainability**: Changes to subclasses don't break existing code that uses the base class

### LSP Violations: When Substitution Breaks

**Bad Example - Violating LSP:**
```csharp
public class Bird 
{
    public virtual void Fly() 
    {
        Console.WriteLine("Bird is flying");
    }
}

public class Penguin : Bird 
{
    public override void Fly() 
    {
        throw new InvalidOperationException("Penguins can't fly!");
        // This breaks LSP! Code expecting any Bird to fly will crash
    }
}

// This code will break when given a Penguin!
public void MakeBirdFly(Bird bird) 
{
    bird.Fly();  // Crashes if bird is a Penguin!
}

Bird penguin = new Penguin();
MakeBirdFly(penguin);  // BOOM! Exception thrown
```

**Good Example - Following LSP:**
```csharp
// Better design - separate flying and non-flying birds
public abstract class Bird 
{
    public abstract void Move();
}

public abstract class FlyingBird : Bird 
{
    public virtual void Fly() 
    {
        Console.WriteLine("Flying through the air");
    }
    
    public override void Move() 
    {
        Fly();
    }
}

public abstract class GroundBird : Bird 
{
    public virtual void Walk() 
    {
        Console.WriteLine("Walking on the ground");
    }
    
    public override void Move() 
    {
        Walk();
    }
}

public class Eagle : FlyingBird 
{
    public override void Fly() 
    {
        Console.WriteLine("Eagle soaring majestically");
    }
}

public class Penguin : GroundBird 
{
    public override void Walk() 
    {
        Console.WriteLine("Penguin waddling adorably");
    }
}

// Now substitution works perfectly!
public void MoveBird(Bird bird) 
{
    bird.Move();  // Works with ANY bird type - no surprises!
}

Bird eagle = new Eagle();
Bird penguin = new Penguin();

MoveBird(eagle);    // Eagle soaring majestically
MoveBird(penguin);  // Penguin waddling adorably
```

### Contract Compliance: Preconditions and Postconditions

**Violating LSP with Strengthened Preconditions:**
```python
class FileReader:
    def read_file(self, filename):
        # Base class: any valid filename is acceptable
        if not filename:
            raise ValueError("Filename cannot be empty")
        
        print(f"Reading file: {filename}")
        return f"Contents of {filename}"

class SecureFileReader(FileReader):
    def read_file(self, filename):
        # VIOLATION: Strengthening preconditions (more restrictive)
        if not filename:
            raise ValueError("Filename cannot be empty")
        
        if not filename.endswith('.secure'):
            raise ValueError("Can only read .secure files")  # BREAKS LSP!
        
        if len(filename) < 10:
            raise ValueError("Secure filenames must be at least 10 chars")  # BREAKS LSP!
        
        print(f"Reading secure file: {filename}")
        return f"Secure contents of {filename}"

# This breaks because SecureFileReader is MORE restrictive than FileReader
def process_files(reader: FileReader, filenames):
    for filename in filenames:
        try:
            content = reader.read_file(filename)  # May break with SecureFileReader!
            print(f"✅ Successfully read: {filename}")
        except ValueError as e:
            print(f"❌ Failed to read {filename}: {e}")

# Usage shows the violation
files = ["document.txt", "data.csv", "config.json"]

print("=== Using base FileReader ===")
base_reader = FileReader()
process_files(base_reader, files)  # Works fine

print("\n=== Using SecureFileReader (LSP Violation) ===")
secure_reader = SecureFileReader()
process_files(secure_reader, files)  # Breaks! More restrictive than base class
```

**Following LSP with Proper Contract Design:**
```python
from abc import ABC, abstractmethod

class FileReader(ABC):
    @abstractmethod
    def can_read(self, filename: str) -> bool:
        """Check if this reader can handle the file"""
        pass
    
    @abstractmethod
    def read_file(self, filename: str) -> str:
        """Read file if can_read returns True"""
        pass

class TextFileReader(FileReader):
    def can_read(self, filename: str) -> bool:
        return filename.endswith(('.txt', '.csv', '.json'))
    
    def read_file(self, filename: str) -> str:
        if not self.can_read(filename):
            raise ValueError(f"Cannot read {filename} with TextFileReader")
        print(f"Reading text file: {filename}")
        return f"Text contents of {filename}"

class SecureFileReader(FileReader):
    def can_read(self, filename: str) -> bool:
        return filename.endswith('.secure') and len(filename) >= 10
    
    def read_file(self, filename: str) -> str:
        if not self.can_read(filename):
            raise ValueError(f"Cannot read {filename} with SecureFileReader")
        print(f"Reading secure file: {filename}")
        return f"Secure contents of {filename}"

# Now the contract is clear and substitution works
def process_files_safely(readers: list[FileReader], filenames: list[str]):
    for filename in filenames:
        print(f"\nProcessing: {filename}")
        
        # Find a reader that can handle this file
        handled = False
        for reader in readers:
            if reader.can_read(filename):
                try:
                    content = reader.read_file(filename)
                    print(f"✅ Read with {reader.__class__.__name__}")
                    handled = True
                    break
                except ValueError as e:
                    print(f"❌ Error: {e}")
        
        if not handled:
            print(f"⚠️ No reader available for {filename}")

# Usage - LSP compliant
files = ["document.txt", "secret.secure", "verylongname.secure", "data.csv"]
readers = [TextFileReader(), SecureFileReader()]

process_files_safely(readers, files)  # Works correctly with all readers
```

### Real-World LSP: Database Connections

**LSP Compliant Database Design:**
```cpp
#include <iostream>
#include <string>
#include <vector>
#include <memory>

class DatabaseConnection {
public:
    virtual ~DatabaseConnection() = default;
    
    // Contract: All subclasses must be able to execute basic queries
    virtual bool connect() = 0;
    virtual void disconnect() = 0;
    virtual std::vector<std::string> executeQuery(const std::string& query) = 0;
    virtual bool isConnected() const = 0;
};

// MySQL implementation follows the contract
class MySQLConnection : public DatabaseConnection {
private:
    bool connected = false;
    
public:
    bool connect() override {
        std::cout << "Connecting to MySQL database..." << std::endl;
        connected = true;
        return true;
    }
    
    void disconnect() override {
        std::cout << "Disconnecting from MySQL..." << std::endl;
        connected = false;
    }
    
    std::vector<std::string> executeQuery(const std::string& query) override {
        if (!connected) {
            throw std::runtime_error("Not connected to MySQL");
        }
        
        std::cout << "Executing MySQL query: " << query << std::endl;
        return {"MySQL Result 1", "MySQL Result 2"};
    }
    
    bool isConnected() const override {
        return connected;
    }
};

// PostgreSQL implementation also follows the contract
class PostgreSQLConnection : public DatabaseConnection {
private:
    bool connected = false;
    
public:
    bool connect() override {
        std::cout << "Connecting to PostgreSQL database..." << std::endl;
        connected = true;
        return true;
    }
    
    void disconnect() override {
        std::cout << "Disconnecting from PostgreSQL..." << std::endl;
        connected = false;
    }
    
    std::vector<std::string> executeQuery(const std::string& query) override {
        if (!connected) {
            throw std::runtime_error("Not connected to PostgreSQL");
        }
        
        std::cout << "Executing PostgreSQL query: " << query << std::endl;
        return {"PostgreSQL Result 1", "PostgreSQL Result 2"};
    }
    
    bool isConnected() const override {
        return connected;
    }
};

// This class works with ANY DatabaseConnection implementation
class DataService {
private:
    std::unique_ptr<DatabaseConnection> db;
    
public:
    DataService(std::unique_ptr<DatabaseConnection> database) 
        : db(std::move(database)) {}
    
    void performOperations() {
        std::cout << "\n=== Data Service Operations ===" << std::endl;
        
        // These operations work with ANY database implementation
        if (db->connect()) {
            std::cout << "✅ Connected successfully" << std::endl;
            
            if (db->isConnected()) {
                auto results = db->executeQuery("SELECT * FROM users");
                
                std::cout << "Query results:" << std::endl;
                for (const auto& result : results) {
                    std::cout << "  - " << result << std::endl;
                }
            }
            
            db->disconnect();
            std::cout << "✅ Disconnected successfully" << std::endl;
        }
    }
};

// Usage demonstrates LSP - same code works with different database types
int main() {
    // Create different database connections
    auto mysqlService = std::make_unique<DataService>(
        std::make_unique<MySQLConnection>()
    );
    
    auto postgresService = std::make_unique<DataService>(
        std::make_unique<PostgreSQLConnection>()
    );
    
    // Same operations work with both database types
    std::cout << "Using MySQL:" << std::endl;
    mysqlService->performOperations();
    
    std::cout << "\nUsing PostgreSQL:" << std::endl;
    postgresService->performOperations();
    
    return 0;
}
```

## Code Examples

The following examples demonstrate LSP compliance through proper inheritance design and contract adherence.

**How to run:**
- C#: `csc chapter-08-advanced.cs && .\chapter-08-advanced.exe`
- C++: `g++ -std=c++17 chapter-08-advanced.cpp -o chapter-08-advanced && .\chapter-08-advanced.exe`
- Python: `python chapter-08-advanced.py`

**Why this advanced solution follows LSP:**
- **Contract-Based Design**: All implementations honor the same contracts
- **Behavioral Consistency**: Subclasses behave predictably within the expected parameters
- **Exception Safety**: No unexpected exceptions are thrown by subclasses
- **Interface Segregation**: Different capabilities are separated into different interfaces

## Exercises

1. **Easy**: Fix a `Bird` hierarchy where `Penguin` inherits from `Bird` but can't fly.
   - *Hint: Not all birds can fly - how should you model this?*

2. **Medium**: Design a payment processing system where different payment methods can be substituted without breaking the code.
   - *Hint: All payment methods should have consistent behavior for success/failure cases*

3. **Hard**: Create a caching system where different cache implementations (memory, disk, distributed) can be used interchangeably.
   - *Hint: Think about what guarantees all cache implementations should provide*

## Chapter Checklist

After completing this chapter, you should be able to:

- [ ] Explain the Liskov Substitution Principle clearly
- [ ] Identify LSP violations in inheritance hierarchies
- [ ] Design subclasses that properly substitute for their parent classes
- [ ] Understand the relationship between LSP and contracts
- [ ] Recognize when inheritance is not the right solution
- [ ] Write unit tests that verify LSP compliance
- [ ] Design interfaces that enable proper substitutability
- [ ] Avoid common LSP pitfalls like the Rectangle/Square problem
- [ ] Balance between behavioral consistency and flexibility