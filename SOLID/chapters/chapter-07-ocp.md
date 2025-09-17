# Chapter 7: Open/Closed Principle (OCP)

## Child-Friendly Explanation
Imagine you have a really nice toy box that you love. Now, you get new toys for your birthday, but they don't fit in your current box. Instead of breaking apart your perfect toy box and rebuilding it (which might ruin it!), you can add a new section or put an extension on top. The Open/Closed Principle is like this - when you need new features in your code, you should be able to add them without breaking the code that already works perfectly!

```python
# BAD - Breaking the toy box every time we get new toys
class ShapeCalculator:
    def calculate_area(self, shapes):
        total_area = 0
        for shape in shapes:
            if shape.type == "circle":
                total_area += 3.14 * shape.radius ** 2
            elif shape.type == "rectangle":
                total_area += shape.width * shape.height
            # OH NO! To add triangle, we have to MODIFY this working code!
            # elif shape.type == "triangle":  # This breaks our perfect code!
            #     total_area += 0.5 * shape.base * shape.height
        return total_area

# GOOD - Extending without breaking what works
class Shape:
    def calculate_area(self):
        pass  # Each shape knows how to calculate its own area

class Circle(Shape):
    def __init__(self, radius):
        self.radius = radius
    
    def calculate_area(self):
        return 3.14 * self.radius ** 2

class Rectangle(Shape):
    def __init__(self, width, height):
        self.width = width
        self.height = height
    
    def calculate_area(self):
        return self.width * self.height

# Adding new shapes doesn't break existing code!
class Triangle(Shape):  # NEW! But doesn't break anything
    def __init__(self, base, height):
        self.base = base
        self.height = height
    
    def calculate_area(self):
        return 0.5 * self.base * self.height

class AreaCalculator:
    def calculate_total_area(self, shapes):
        return sum(shape.calculate_area() for shape in shapes)
        # This code NEVER needs to change, no matter how many shapes we add!
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

### Plugin Architecture: Ultimate OCP Example

```cpp
#include <vector>
#include <memory>
#include <string>
#include <iostream>

// Base interface that plugins implement
class DataProcessor {
public:
    virtual ~DataProcessor() = default;
    virtual void processData(const std::string& data) = 0;
    virtual std::string getProcessorName() const = 0;
};

// Core system that uses plugins - NEVER changes!
class DataProcessingEngine {
private:
    std::vector<std::unique_ptr<DataProcessor>> processors;
    
public:
    void registerProcessor(std::unique_ptr<DataProcessor> processor) {
        std::cout << "Registered processor: " << processor->getProcessorName() << std::endl;
        processors.push_back(std::move(processor));
    }
    
    void processAllData(const std::string& data) {
        std::cout << "\n=== Processing data with all registered processors ===" << std::endl;
        for (const auto& processor : processors) {
            std::cout << "\n--- " << processor->getProcessorName() << " ---" << std::endl;
            processor->processData(data);
        }
    }
};

// Original processors - shipped with v1.0
class JsonProcessor : public DataProcessor {
public:
    void processData(const std::string& data) override {
        std::cout << "Parsing JSON data..." << std::endl;
        std::cout << "Validating JSON structure..." << std::endl;
        std::cout << "JSON processing complete!" << std::endl;
    }
    
    std::string getProcessorName() const override {
        return "JSON Processor v1.0";
    }
};

class XmlProcessor : public DataProcessor {
public:
    void processData(const std::string& data) override {
        std::cout << "Parsing XML data..." << std::endl;
        std::cout << "Validating against schema..." << std::endl;
        std::cout << "XML processing complete!" << std::endl;
    }
    
    std::string getProcessorName() const override {
        return "XML Processor v1.0";
    }
};

// NEW processors added in v2.0 - NO existing code modified!
class CsvProcessor : public DataProcessor {
public:
    void processData(const std::string& data) override {
        std::cout << "Parsing CSV data..." << std::endl;
        std::cout << "Detecting column headers..." << std::endl;
        std::cout << "CSV processing complete!" << std::endl;
    }
    
    std::string getProcessorName() const override {
        return "CSV Processor v2.0";  // NEW!
    }
};

class YamlProcessor : public DataProcessor {
public:
    void processData(const std::string& data) override {
        std::cout << "Parsing YAML data..." << std::endl;
        std::cout << "Processing nested structures..." << std::endl;
        std::cout << "YAML processing complete!" << std::endl;
    }
    
    std::string getProcessorName() const override {
        return "YAML Processor v2.0";  // NEW!
    }
};

// FUTURE processors for v3.0 - still no existing code modified!
class ProtobufProcessor : public DataProcessor {
public:
    void processData(const std::string& data) override {
        std::cout << "Deserializing Protocol Buffer data..." << std::endl;
        std::cout << "Validating message types..." << std::endl;
        std::cout << "Protobuf processing complete!" << std::endl;
    }
    
    std::string getProcessorName() const override {
        return "Protocol Buffer Processor v3.0";  // FUTURE!
    }
};

// Usage - system extends without any core changes
int main() {
    DataProcessingEngine engine;
    
    // Version 1.0 - original processors
    engine.registerProcessor(std::make_unique<JsonProcessor>());
    engine.registerProcessor(std::make_unique<XmlProcessor>());
    
    // Version 2.0 - add new processors without changing engine!
    engine.registerProcessor(std::make_unique<CsvProcessor>());
    engine.registerProcessor(std::make_unique<YamlProcessor>());
    
    // Version 3.0 - add even more processors!
    engine.registerProcessor(std::make_unique<ProtobufProcessor>());
    
    // Engine works with all processors without modification
    engine.processAllData("sample data");
    
    return 0;
}
```

### Strategy Pattern: OCP in Action

```python
from abc import ABC, abstractmethod
from enum import Enum

# Abstract strategy interface
class CompressionStrategy(ABC):
    @abstractmethod
    def compress(self, data: str) -> str:
        pass
    
    @abstractmethod
    def decompress(self, compressed_data: str) -> str:
        pass
    
    @abstractmethod
    def get_name(self) -> str:
        pass

# Original compression algorithms
class ZipCompression(CompressionStrategy):
    def compress(self, data: str) -> str:
        return f"ZIP[{data}]"
    
    def decompress(self, compressed_data: str) -> str:
        return compressed_data.replace("ZIP[", "").replace("]", "")
    
    def get_name(self) -> str:
        return "ZIP Compression"

class GzipCompression(CompressionStrategy):
    def compress(self, data: str) -> str:
        return f"GZIP[{data}]"
    
    def decompress(self, compressed_data: str) -> str:
        return compressed_data.replace("GZIP[", "").replace("]", "")
    
    def get_name(self) -> str:
        return "GZIP Compression"

# NEW algorithms added later - no existing code modified!
class Bzip2Compression(CompressionStrategy):
    def compress(self, data: str) -> str:
        return f"BZIP2[{data}]"
    
    def decompress(self, compressed_data: str) -> str:
        return compressed_data.replace("BZIP2[", "").replace("]", "")
    
    def get_name(self) -> str:
        return "BZIP2 Compression"

class LzmaCompression(CompressionStrategy):
    def compress(self, data: str) -> str:
        return f"LZMA[{data}]"
    
    def decompress(self, compressed_data: str) -> str:
        return compressed_data.replace("LZMA[", "").replace("]", "")
    
    def get_name(self) -> str:
        return "LZMA Compression"

# Context class - NEVER needs modification for new compression types!
class FileCompressor:
    def __init__(self, strategy: CompressionStrategy):
        self._strategy = strategy
    
    def set_strategy(self, strategy: CompressionStrategy):
        self._strategy = strategy
        print(f"Switched to {strategy.get_name()}")
    
    def compress_file(self, filename: str, data: str) -> str:
        print(f"Compressing {filename} using {self._strategy.get_name()}")
        compressed = self._strategy.compress(data)
        print(f"Compressed: {data} -> {compressed}")
        return compressed
    
    def decompress_file(self, compressed_data: str) -> str:
        print(f"Decompressing using {self._strategy.get_name()}")
        decompressed = self._strategy.decompress(compressed_data)
        print(f"Decompressed: {compressed_data} -> {decompressed}")
        return decompressed

# Factory for creating compression strategies - easy to extend!
class CompressionFactory:
    @staticmethod
    def create_compressor(compression_type: str) -> CompressionStrategy:
        strategies = {
            "zip": ZipCompression(),
            "gzip": GzipCompression(),
            "bzip2": Bzip2Compression(),  # NEW!
            "lzma": LzmaCompression(),    # NEW!
        }
        
        strategy = strategies.get(compression_type.lower())
        if not strategy:
            raise ValueError(f"Unknown compression type: {compression_type}")
        return strategy

# Usage - system works with all compression types without modification
def demonstrate_ocp():
    sample_data = "Hello, World! This is test data for compression."
    
    # Try all compression types
    compression_types = ["zip", "gzip", "bzip2", "lzma"]
    
    for comp_type in compression_types:
        print(f"\n{'='*50}")
        print(f"Testing {comp_type.upper()} compression")
        print('='*50)
        
        # Create strategy and compressor
        strategy = CompressionFactory.create_compressor(comp_type)
        compressor = FileCompressor(strategy)
        
        # Compress and decompress
        compressed = compressor.compress_file("test.txt", sample_data)
        decompressed = compressor.decompress_file(compressed)
        
        # Verify round-trip
        assert decompressed == sample_data, "Round-trip failed!"
        print("✅ Round-trip successful!")

# Adding new compression is easy - just implement the interface!
class LZ4Compression(CompressionStrategy):  # BRAND NEW!
    def compress(self, data: str) -> str:
        return f"LZ4[{data}]"
    
    def decompress(self, compressed_data: str) -> str:
        return compressed_data.replace("LZ4[", "").replace("]", "")
    
    def get_name(self) -> str:
        return "LZ4 Fast Compression"

# Just add to factory - no other code changes needed!
# CompressionFactory can be extended to include "lz4": LZ4Compression()

demonstrate_ocp()
```

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

**How to run:**
- C#: `csc chapter-07-intermediate.cs && .\chapter-07-intermediate.exe`
- C++: `g++ -std=c++17 chapter-07-intermediate.cpp -o chapter-07-intermediate && .\chapter-07-intermediate.exe`
- Python: `python chapter-07-intermediate.py`

**Expected Output:** Shows a discount calculation system that follows OCP.

### Advanced Example

**How to run:**
- C#: `csc chapter-07-advanced.cs && .\chapter-07-advanced.exe`
- C++: `g++ -std=c++17 chapter-07-advanced.cpp -o chapter-07-advanced && .\chapter-07-advanced.exe`
- Python: `python chapter-07-advanced.py`

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