# Chapter 8: Liskov Substitution Principle (LSP)

## Child-Friendly Explanation
Imagine you have a toy box, and someone tells you "put any kind of ball in this box." You should be able to put a soccer ball, basketball, tennis ball, or any other ball in there, and the box should work just fine with all of them. The Liskov Substitution Principle is like this - if your code is designed to work with a "parent" type, it should work perfectly with any "child" type too, without any surprises or problems!

```csharp
// All balls should work the same way in our toy box
public class Ball
{
    protected int size;
    
    public Ball(int size)
    {
        this.size = size;
    }
    
    public virtual void Bounce()
    {
        Console.WriteLine($"Ball bouncing to height {size * 2}");
    }
    
    public virtual void Roll()
    {
        Console.WriteLine($"Ball rolling {size * 3} meters");
    }
}

public class SoccerBall : Ball
{
    public SoccerBall(int size) : base(size) { }
    
    public override void Bounce()
    {
        Console.WriteLine($"Soccer ball bouncing to height {size * 2}");  // Same behavior!
    }
    
    public override void Roll()
    {
        Console.WriteLine($"Soccer ball rolling {size * 3} meters");  // Same behavior!
    }
}

public class Basketball : Ball
{
    public Basketball(int size) : base(size) { }
    
    public override void Bounce()
    {
        Console.WriteLine($"Basketball bouncing to height {size * 2}");  // Same behavior!
    }
    
    public override void Roll()
    {
        Console.WriteLine($"Basketball rolling {size * 3} meters");  // Same behavior!
    }
}

// This toy box works with ANY ball - no surprises!
public static void PlayWithBall(Ball ball)
{
    ball.Bounce();  // Works with any ball type
    ball.Roll();    // Works with any ball type
}

// All balls work perfectly in the same toy box
var soccer = new SoccerBall(5);
var basketball = new Basketball(6);

PlayWithBall(soccer);     // Works perfectly
PlayWithBall(basketball); // Works perfectly too!
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
```csharp
public class FileReader
{
    public virtual string ReadFile(string filename)
    {
        // Base class: any valid filename is acceptable
        if (string.IsNullOrEmpty(filename))
        {
            throw new ArgumentException("Filename cannot be empty");
        }
        
        Console.WriteLine($"Reading file: {filename}");
        return $"Contents of {filename}";
    }
}

public class SecureFileReader : FileReader
{
    public override string ReadFile(string filename)
    {
        // VIOLATION: Strengthening preconditions (more restrictive)
        if (string.IsNullOrEmpty(filename))
        {
            throw new ArgumentException("Filename cannot be empty");
        }
        
        if (!filename.EndsWith(".secure"))
        {
            throw new ArgumentException("Can only read .secure files");  // BREAKS LSP!
        }
        
        if (filename.Length < 10)
        {
            throw new ArgumentException("Secure filenames must be at least 10 chars");  // BREAKS LSP!
        }
        
        Console.WriteLine($"Reading secure file: {filename}");
        return $"Secure contents of {filename}";
    }
}

// This breaks because SecureFileReader is MORE restrictive than FileReader
public static void ProcessFiles(FileReader reader, string[] filenames)
{
    foreach (var filename in filenames)
    {
        try
        {
            var content = reader.ReadFile(filename);  // May break with SecureFileReader!
            Console.WriteLine($"✅ Successfully read: {filename}");
        }
        catch (ArgumentException e)
        {
            Console.WriteLine($"❌ Failed to read {filename}: {e.Message}");
        }
    }
}

// Usage shows the violation
var files = new[] { "document.txt", "data.csv", "config.json" };

Console.WriteLine("=== Using base FileReader ===");
var baseReader = new FileReader();
ProcessFiles(baseReader, files);  // Works fine

Console.WriteLine("\n=== Using SecureFileReader (LSP Violation) ===");
var secureReader = new SecureFileReader();
ProcessFiles(secureReader, files);  // Breaks! More restrictive than base class
```

**Following LSP with Proper Contract Design:**
```csharp
public abstract class FileReader
{
    public abstract bool CanRead(string filename);  // Check if this reader can handle the file
    
    public abstract string ReadFile(string filename);  // Read file if CanRead returns True
}

public class TextFileReader : FileReader
{
    public override bool CanRead(string filename)
    {
        return filename.EndsWith(".txt") || filename.EndsWith(".csv") || filename.EndsWith(".json");
    }
    
    public override string ReadFile(string filename)
    {
        if (!CanRead(filename))
        {
            throw new ArgumentException($"Cannot read {filename} with TextFileReader");
        }
        Console.WriteLine($"Reading text file: {filename}");
        return $"Text contents of {filename}";
    }
}

public class SecureFileReader : FileReader
{
    public override bool CanRead(string filename)
    {
        return filename.EndsWith(".secure") && filename.Length >= 10;
    }
    
    public override string ReadFile(string filename)
    {
        if (!CanRead(filename))
        {
            throw new ArgumentException($"Cannot read {filename} with SecureFileReader");
        }
        Console.WriteLine($"Reading secure file: {filename}");
        return $"Secure contents of {filename}";
    }
}

// Now the contract is clear and substitution works
public static void ProcessFilesSafely(List<FileReader> readers, string[] filenames)
{
    foreach (var filename in filenames)
    {
        Console.WriteLine($"\nProcessing: {filename}");
        
        // Find a reader that can handle this file
        bool handled = false;
        foreach (var reader in readers)
        {
            if (reader.CanRead(filename))
            {
                try
                {
                    var content = reader.ReadFile(filename);
                    Console.WriteLine($"✅ Read with {reader.GetType().Name}");
                    handled = true;
                    break;
                }
                catch (ArgumentException e)
                {
                    Console.WriteLine($"❌ Error: {e.Message}");
                }
            }
        }
        
        if (!handled)
        {
            Console.WriteLine($"⚠️ No reader available for {filename}");
        }
    }
}

// Usage - LSP compliant
var files = new[] { "document.txt", "secret.secure", "verylongname.secure", "data.csv" };
var readers = new List<FileReader> { new TextFileReader(), new SecureFileReader() };

ProcessFilesSafely(readers, files);  // Works correctly with all readers
```

### Real-World LSP: Database Connections

**LSP Compliant Database Design:**
```csharp
public abstract class DatabaseConnection
{
    // Contract: All subclasses must be able to execute basic queries
    public abstract bool Connect();
    public abstract void Disconnect();
    public abstract List<string> ExecuteQuery(string query);
    public abstract bool IsConnected { get; }
}

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