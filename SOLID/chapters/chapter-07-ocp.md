# Chapter 7: Open/Closed Principle (OCP)

## Child-Friendly Explanation
Imagine you have a really nice toy box that you love. Now, you get new toys for your birthday, but they don't fit in your current box. Instead of breaking apart your perfect toy box and rebuilding it (which might ruin it!), you can add a new section or put an extension on top. The Open/Closed Principle is like this - when you need new features in your code, you should be able to add them without breaking the code that already works perfectly!

```csharp
// BAD - Breaking the toy box every time we get new toys
public class ShapeCalculator
{
    public double CalculateArea(List<dynamic> shapes)
    {
        double totalArea = 0;
        foreach (var shape in shapes)
        {
            if (shape.Type == "circle")
            {
                totalArea += 3.14 * shape.Radius * shape.Radius;
            }
            else if (shape.Type == "rectangle")
            {
                totalArea += shape.Width * shape.Height;
            }
            // OH NO! To add triangle, we have to MODIFY this working code!
            // else if (shape.Type == "triangle")  // This breaks our perfect code!
            // {
            //     totalArea += 0.5 * shape.Base * shape.Height;
            // }
        }
        return totalArea;
    }
}

// GOOD - Extending without breaking what works
public abstract class Shape
{
    public abstract double CalculateArea();  // Each shape knows how to calculate its own area
}

public class Circle : Shape
{
    private double radius;
    
    public Circle(double radius)
    {
        this.radius = radius;
    }
    
    public override double CalculateArea()
    {
        return 3.14 * radius * radius;
    }
}

public class Rectangle : Shape
{
    private double width, height;
    
    public Rectangle(double width, double height)
    {
        this.width = width;
        this.height = height;
    }
    
    public override double CalculateArea()
    {
        return width * height;
    }
}

// Adding new shapes doesn't break existing code!
public class Triangle : Shape  // NEW! But doesn't break anything
{
    private double baseLength, height;
    
    public Triangle(double baseLength, double height)
    {
        this.baseLength = baseLength;
        this.height = height;
    }
    
    public override double CalculateArea()
    {
        return 0.5 * baseLength * height;
    }
}

public class AreaCalculator
{
    public double CalculateTotalArea(List<Shape> shapes)
    {
        return shapes.Sum(shape => shape.CalculateArea());
        // This code NEVER needs to change, no matter how many shapes we add!
    }
}
```

## Developer-Level Explanation
The Open/Closed Principle states that software entities (classes, modules, functions) should be **open for extension but closed for modification**. This means you should be able to add new functionality without changing existing code that is already working and tested.

**Why OCP matters:**
- **Stability**: Existing code that works doesn't get broken by new features
- **Maintainability**: Changes are localized to new code rather than scattered throughout
- **Testing**: Existing tests remain valid; only new functionality needs new tests
- **Risk Reduction**: Less chance of introducing bugs in working code

### Real-World OCP Example: Payment Processing

**Violating OCP (Bad):**
```csharp
public class PaymentProcessor 
{
    public void ProcessPayment(Payment payment) 
    {
        if (payment.Type == "CreditCard") 
        {
            // Credit card processing logic
            Console.WriteLine("Processing credit card payment...");
        }
        else if (payment.Type == "PayPal") 
        {
            // PayPal processing logic  
            Console.WriteLine("Processing PayPal payment...");
        }
        // OH NO! To add Bitcoin, we must MODIFY this working code!
        // else if (payment.Type == "Bitcoin") 
        // {
        //     Console.WriteLine("Processing Bitcoin payment...");
        // }
        
        // This class keeps growing and changing with every new payment type!
    }
}
```

**Following OCP (Good):**
```csharp
// Abstract interface - defines the contract
public abstract class PaymentMethod 
{
    public abstract void ProcessPayment(decimal amount);
}

// Existing implementations - these never change
public class CreditCardPayment : PaymentMethod 
{
    public override void ProcessPayment(decimal amount) 
    {
        Console.WriteLine($"Processing ${amount} via credit card...");
        // Credit card specific logic
    }
}

public class PayPalPayment : PaymentMethod 
{
    public override void ProcessPayment(decimal amount) 
    {
        Console.WriteLine($"Processing ${amount} via PayPal...");
        // PayPal specific logic
    }
}

// NEW payment type - extends without modifying existing code!
public class BitcoinPayment : PaymentMethod 
{
    public override void ProcessPayment(decimal amount) 
    {
        Console.WriteLine($"Processing ${amount} via Bitcoin...");
        // Bitcoin specific logic
    }
}

// Payment processor - NEVER needs to change!
public class PaymentProcessor 
{
    public void ProcessPayment(PaymentMethod paymentMethod, decimal amount) 
    {
        paymentMethod.ProcessPayment(amount);
        // This code works with any payment method, existing or future!
    }
}

// Usage - can add new payment types without breaking anything
PaymentProcessor processor = new PaymentProcessor();
processor.ProcessPayment(new CreditCardPayment(), 100);
processor.ProcessPayment(new PayPalPayment(), 200);
processor.ProcessPayment(new BitcoinPayment(), 300);  // NEW! But no existing code changed
```

**Code Explanation:**
This payment system perfectly demonstrates OCP. The `PaymentMethod` abstract class provides the extension point, while the `PaymentProcessor` class is closed for modification. When we added `BitcoinPayment`, we didn't change a single line in `PaymentProcessor` or any existing payment classes. The system automatically works with the new payment type because it follows the same contract. This is the power of OCP - new functionality through extension, not modification.

### Plugin Architecture: Ultimate OCP Example

```csharp
// Base interface that plugins implement
public interface IDataProcessor
{
    void ProcessData(string data);
    string GetProcessorName();
}

// Core system that uses plugins - NEVER changes!
public class DataProcessingEngine
{
    private List<IDataProcessor> processors = new List<IDataProcessor>();
    
    public void RegisterProcessor(IDataProcessor processor)
    {
        Console.WriteLine($"Registered processor: {processor.GetProcessorName()}");
        processors.Add(processor);
    }
    
    public void ProcessAllData(string data)
    {
        Console.WriteLine("\n=== Processing data with all registered processors ===");
        foreach (var processor in processors)
        {
            Console.WriteLine($"\n--- {processor.GetProcessorName()} ---");
            processor.ProcessData(data);
        }
    }
}

// Original processors - shipped with v1.0
public class JsonProcessor : IDataProcessor
{
    public void ProcessData(string data)
    {
        Console.WriteLine("Parsing JSON data...");
        Console.WriteLine("Validating JSON structure...");
        Console.WriteLine("JSON processing complete!");
    }
    
    public string GetProcessorName()
    {
        return "JSON Processor v1.0";
    }
}

public class XmlProcessor : IDataProcessor
{
    public void ProcessData(string data)
    {
        Console.WriteLine("Parsing XML data...");
        Console.WriteLine("Validating against schema...");
        Console.WriteLine("XML processing complete!");
    }
    
    public string GetProcessorName()
    {
        return "XML Processor v1.0";
    }
}

// NEW processors added in v2.0 - NO existing code modified!
public class CsvProcessor : IDataProcessor
{
    public void ProcessData(string data)
    {
        Console.WriteLine("Parsing CSV data...");
        Console.WriteLine("Detecting column headers...");
        Console.WriteLine("CSV processing complete!");
    }
    
    public string GetProcessorName()
    {
        return "CSV Processor v2.0";  // NEW!
    }
}

public class YamlProcessor : IDataProcessor
{
    public void ProcessData(string data)
    {
        Console.WriteLine("Parsing YAML data...");
        Console.WriteLine("Processing nested structures...");
        Console.WriteLine("YAML processing complete!");
    }
    
    public string GetProcessorName()
    {
        return "YAML Processor v2.0";  // NEW!
    }
}

// FUTURE processors for v3.0 - still no existing code modified!
public class ProtobufProcessor : IDataProcessor
{
    public void ProcessData(string data)
    {
        Console.WriteLine("Deserializing Protocol Buffer data...");
        Console.WriteLine("Validating message types...");
        Console.WriteLine("Protobuf processing complete!");
    }
    
    public string GetProcessorName()
    {
        return "Protocol Buffer Processor v3.0";  // FUTURE!
    }
}

// Usage - system extends without any core changes
public static void Main()
{
    var engine = new DataProcessingEngine();
    
    // Version 1.0 - original processors
    engine.RegisterProcessor(new JsonProcessor());
    engine.RegisterProcessor(new XmlProcessor());
    
    // Version 2.0 - add new processors without changing engine!
    // Version 2.0 - add more processors!
    engine.RegisterProcessor(new CsvProcessor());
    engine.RegisterProcessor(new YamlProcessor());
    
    // Version 3.0 - add even more processors!
    engine.RegisterProcessor(new ProtobufProcessor());
    
    // Engine works with all processors without modification
    engine.ProcessAllData("sample data");
}
```

**Code Explanation:**
This plugin architecture shows OCP at its finest. The `DataProcessingEngine` is completely closed for modification - it has never been changed since v1.0. Yet we've extended it through three major versions by adding new processor plugins. Each processor implements the `IDataProcessor` interface, making them compatible with the engine. Notice how we went from 2 processors to 5 processors without touching the core engine code. This is how real-world systems achieve scalability while maintaining stability.

### Strategy Pattern: OCP in Action

```csharp
// Abstract strategy interface
public interface ICompressionStrategy
{
    string Compress(string data);
    string Decompress(string compressedData);
    string GetName();
}

// Original compression algorithms
public class ZipCompression : ICompressionStrategy
{
    public string Compress(string data)
    {
        return $"ZIP[{data}]";
    }
    
    public string Decompress(string compressedData)
    {
        return compressedData.Replace("ZIP[", "").Replace("]", "");
    }
    
    public string GetName()
    {
        return "ZIP Compression";
    }
}

public class GzipCompression : ICompressionStrategy
{
    public string Compress(string data)
    {
        return $"GZIP[{data}]";
    }
    
    public string Decompress(string compressedData)
    {
        return compressedData.Replace("GZIP[", "").Replace("]", "");
    }
    
    public string GetName()
    {
        return "GZIP Compression";
    }
}

// NEW algorithms added later - no existing code modified!
public class Bzip2Compression : ICompressionStrategy
{
    public string Compress(string data)
    {
        return $"BZIP2[{data}]";
    }
    
    public string Decompress(string compressedData)
    {
        return compressedData.Replace("BZIP2[", "").Replace("]", "");
    }
    
    public string GetName()
    {
        return "BZIP2 Compression";
    }
}

public class LzmaCompression : ICompressionStrategy
{
    public string Compress(string data)
    {
        return $"LZMA[{data}]";
    }
    
    public string Decompress(string compressedData)
    {
        return compressedData.Replace("LZMA[", "").Replace("]", "");
    }
    
    public string GetName()
    {
        return "LZMA Compression";
    }
}

// Context class - NEVER needs modification for new compression types!
public class FileCompressor
{
    private ICompressionStrategy _strategy;
    
    public FileCompressor(ICompressionStrategy strategy)
    {
        _strategy = strategy;
    }
    
    public void SetStrategy(ICompressionStrategy strategy)
    {
        _strategy = strategy;
        Console.WriteLine($"Switched to {strategy.GetName()}");
    }
    
    public string CompressFile(string filename, string data)
    {
        Console.WriteLine($"Compressing {filename} using {_strategy.GetName()}");
        var compressed = _strategy.Compress(data);
        Console.WriteLine($"Compressed: {data} -> {compressed}");
        return compressed;
    }
    
    public string DecompressFile(string compressedData)
    {
        Console.WriteLine($"Decompressing using {_strategy.GetName()}");
        var decompressed = _strategy.Decompress(compressedData);
        Console.WriteLine($"Decompressed: {compressedData} -> {decompressed}");
        return decompressed;
    }
}

// Factory for creating compression strategies - easy to extend!
public static class CompressionFactory
{
    public static ICompressionStrategy CreateCompressor(string compressionType)
    {
        var strategies = new Dictionary<string, ICompressionStrategy>
        {
            ["zip"] = new ZipCompression(),
            ["gzip"] = new GzipCompression(),
            ["bzip2"] = new Bzip2Compression(),  // NEW!
            ["lzma"] = new LzmaCompression()     // NEW!
        };
        
        if (strategies.TryGetValue(compressionType.ToLower(), out var strategy))
        {
            return strategy;
        }
        
        throw new ArgumentException($"Unknown compression type: {compressionType}");
    }
}

// Usage - system works with all compression types without modification
public static void DemonstrateOcp()
{
    string sampleData = "Hello, World! This is test data for compression.";
    
    // Try all compression types
    string[] compressionTypes = { "zip", "gzip", "bzip2", "lzma" };
    
    foreach (var compType in compressionTypes)
    {
        Console.WriteLine($"\n{"=".PadLeft(50, '=')}");
        Console.WriteLine($"Testing {compType.ToUpper()} compression");
        Console.WriteLine("=".PadLeft(50, '='));
        
        // Create strategy and compressor
        var strategy = CompressionFactory.CreateCompressor(compType);
        var compressor = new FileCompressor(strategy);
        
        // Compress and decompress
        var compressed = compressor.CompressFile("test.txt", sampleData);
        var decompressed = compressor.DecompressFile(compressed);
        
        // Verify round-trip
        if (decompressed == sampleData)
        {
            Console.WriteLine("✅ Round-trip successful!");
        }
    }
}

// Adding new compression is easy - just implement the interface!
public class LZ4Compression : ICompressionStrategy  // BRAND NEW!
{
    public string Compress(string data)
    {
        return $"LZ4[{data}]";
    }
    
    public string Decompress(string compressedData)
    {
        return compressedData.Replace("LZ4[", "").Replace("]", "");
    }
    
    public string GetName()
    {
        return "LZ4 Fast Compression";
    }
}

// Just add to factory - no other code changes needed!
// CompressionFactory can be extended to include "lz4": new LZ4Compression()
```

**Code Explanation:**
This strategy pattern example perfectly demonstrates OCP in action. The `FileCompressor` class is completely closed for modification - it never needs to change when new compression algorithms are added. We've extended the system from 2 compression types to 5 types without touching the core compressor logic. Each new strategy implements the `ICompressionStrategy` interface, making it instantly compatible with the existing system. The factory pattern makes it easy to manage strategy creation, and the runtime strategy switching shows the flexibility that OCP enables.

## Code Examples

The following examples demonstrate OCP through practical scenarios showing how systems can be extended without modification.
Rectangle area: 24
Circle area: 78.54

=== OCP Compliant Example ===
Rectangle area: 24
Circle area: 78.54
Triangle area: 6

Adding new shape without modifying existing code!
Hexagon area: 150
```

### Intermediate Example

**Expected Output:** Shows a discount calculation system that follows OCP.

### Advanced Example

**Why this advanced solution follows OCP:**
- **Strategy Pattern**: Different algorithms can be added without modifying existing code
- **Factory Pattern**: New object types can be created without changing factory logic
- **Plugin Architecture**: New modules can be loaded dynamically
- **Event-Driven Design**: New event handlers can be added without modifying the event system

## Exercises

1. **Easy**: Refactor a simple calculator that uses if-else for operations (+, -, *, /) to follow OCP.
   - *Hint: Create an interface for operations and implement each operation separately*

2. **Medium**: Design a notification system that currently only sends emails, but should be extensible for SMS, push notifications, etc.
   - *Hint: Create an abstraction for notification methods*

3. **Hard**: Create a report generation system that can output different formats (PDF, HTML, CSV) and can be extended with new formats.
   - *Hint: Think about separating report data from report formatting*

## Chapter Checklist

After completing this chapter, you should be able to:

- [ ] Explain the Open/Closed Principle in simple terms
- [ ] Identify code that violates OCP (requires modification for extension)
- [ ] Use abstraction to make code open for extension
- [ ] Design systems that can be extended without modification
- [ ] Apply the Strategy pattern to achieve OCP
- [ ] Recognize when OCP compliance is worth the extra complexity
- [ ] Refactor existing code to follow OCP
- [ ] Balance between flexibility and simplicity
- [ ] Use composition and inheritance appropriately for extension