# Chapter 5: Polymorphism - One Interface, Many Forms

## Child-Friendly Explanation
Imagine you have a remote control that has a "play" button. When you point it at a TV and press play, it plays a TV show. When you point it at a music player and press play, it plays music. When you point it at a game console and press play, it starts a game. It's the same button doing the same action ("play"), but each device responds differently! Polymorphism in programming is just like this - the same action can work differently depending on what object you're using it with.

```python
# Same method name, different behaviors
class TV:
    def play(self):
        print("📺 Playing TV show...")

class MusicPlayer:
    def play(self):
        print("🎵 Playing music...")

class GameConsole:
    def play(self):
        print("🎮 Starting game...")

# Same remote control works with all devices!
def use_remote_control(device):
    print("Pressing PLAY button...")
    device.play()  # Same method call, different behavior!

# All work with the same interface
tv = TV()
music = MusicPlayer()
game = GameConsole()

use_remote_control(tv)     # 📺 Playing TV show...
use_remote_control(music)  # 🎵 Playing music...
use_remote_control(game)   # 🎮 Starting game...
```

## Developer-Level Explanation
Polymorphism is the ability of different objects to respond to the same interface in their own specific way. It allows a single interface to represent different underlying forms (data types). This is one of the most powerful features of object-oriented programming.

**Types of polymorphism:**
- **Compile-time Polymorphism**: Method overloading, operator overloading
- **Runtime Polymorphism**: Method overriding, interface implementation

**Why polymorphism matters:**
- **Flexibility**: Code can work with objects it wasn't specifically designed for
- **Extensibility**: New types can be added without changing existing code
- **Maintainability**: Changes to specific implementations don't affect client code
- **Code Reuse**: Same algorithms can work with different object types

### Runtime Polymorphism: Dynamic Method Selection

```csharp
// Base class defines the interface
public abstract class Shape 
{
    public abstract double CalculateArea();
    public abstract void Draw();
}

// Different implementations
public class Circle : Shape 
{
    private double radius;
    
    public Circle(double radius) { this.radius = radius; }
    
    public override double CalculateArea() 
    {
        return Math.PI * radius * radius;
    }
    
    public override void Draw() 
    {
        Console.WriteLine($"Drawing a circle with area {CalculateArea():F2}");
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
    
    public override void Draw() 
    {
        Console.WriteLine($"Drawing a rectangle with area {CalculateArea():F2}");
    }
}

// Polymorphism in action - same code works with all shapes!
public static void ProcessShapes(Shape[] shapes) 
{
    foreach (Shape shape in shapes) 
    {
        shape.Draw();  // Calls the correct implementation at runtime
    }
}

// Usage
Shape[] shapes = { 
    new Circle(5), 
    new Rectangle(4, 6),
    new Circle(3)
};
ProcessShapes(shapes);  // Works with any shape type!
```

### Interface-Based Polymorphism: Multiple Contracts

```python
from abc import ABC, abstractmethod

# Multiple interfaces that objects can implement
class Drawable(ABC):
    @abstractmethod
    def draw(self):
        pass

class Movable(ABC):
    @abstractmethod
    def move(self, x, y):
        pass

class Resizable(ABC):
    @abstractmethod
    def resize(self, factor):
        pass

# Objects can implement multiple interfaces
class GameSprite(Drawable, Movable, Resizable):
    def __init__(self, name, x=0, y=0, size=1):
        self.name = name
        self.x = x
        self.y = y
        self.size = size
    
    def draw(self):
        print(f"Drawing {self.name} at ({self.x}, {self.y}) with size {self.size}")
    
    def move(self, x, y):
        self.x += x
        self.y += y
        print(f"{self.name} moved to ({self.x}, {self.y})")
    
    def resize(self, factor):
        self.size *= factor
        print(f"{self.name} resized to {self.size}")

class Player(GameSprite):
    def __init__(self, name, health=100):
        super().__init__(name)
        self.health = health
    
    def draw(self):  # Override with player-specific drawing
        print(f"🏃 Drawing player {self.name} (HP: {self.health}) at ({self.x}, {self.y})")

class Enemy(GameSprite):
    def __init__(self, name, damage=10):
        super().__init__(name)
        self.damage = damage
    
    def draw(self):  # Override with enemy-specific drawing
        print(f"👹 Drawing enemy {self.name} (DMG: {self.damage}) at ({self.x}, {self.y})")

# Polymorphic functions work with any object implementing the interface
def render_all(drawables: list[Drawable]):
    for drawable in drawables:
        drawable.draw()  # Calls the appropriate draw method

def move_all(movables: list[Movable], dx, dy):
    for movable in movables:
        movable.move(dx, dy)  # Calls the appropriate move method

# Usage - polymorphism in action
game_objects = [
    Player("Hero"),
    Enemy("Goblin"),
    Enemy("Orc"),
    GameSprite("Treasure Chest")
]

print("=== Rendering all objects ===")
render_all(game_objects)  # Works with any Drawable object

print("\n=== Moving all objects ===")
move_all(game_objects, 5, 3)  # Works with any Movable object
```

### Method Overloading vs Overriding: Compile vs Runtime Polymorphism

```csharp
public class Calculator 
{
    // Method Overloading (Compile-time polymorphism)
    // Same method name, different parameters
    public int Add(int a, int b) 
    {
        Console.WriteLine("Adding integers");
        return a + b;
    }
    
    public double Add(double a, double b) 
    {
        Console.WriteLine("Adding doubles");
        return a + b;
    }
    
    public string Add(string a, string b) 
    {
        Console.WriteLine("Concatenating strings");
        return a + b;
    }
    
    // The compiler chooses which method to call based on parameters
}

// Runtime polymorphism through inheritance
public abstract class Animal 
{
    public abstract void MakeSound();  // To be overridden
    
    public virtual void Sleep()  // Can be overridden
    {
        Console.WriteLine("Animal is sleeping");
    }
}

public class Dog : Animal 
{
    public override void MakeSound()  // Method Overriding
    {
        Console.WriteLine("Woof!");
    }
    
    public override void Sleep() 
    {
        Console.WriteLine("Dog is sleeping in doghouse");
    }
}

public class Cat : Animal 
{
    public override void MakeSound()  // Different implementation
    {
        Console.WriteLine("Meow!");
    }
    
    // Uses inherited Sleep method
}

// Usage demonstrates both types of polymorphism
Calculator calc = new Calculator();
calc.Add(5, 3);        // Calls int version
calc.Add(5.5, 3.2);    // Calls double version
calc.Add("Hello", " World");  // Calls string version

Animal[] animals = { new Dog(), new Cat() };
foreach (Animal animal in animals) 
{
    animal.MakeSound();  // Runtime polymorphism - correct method chosen at runtime
    animal.Sleep();      // Runtime polymorphism
}
```

### Polymorphic Collections: Working with Mixed Types

```cpp
#include <vector>
#include <memory>
#include <iostream>

class Document {
protected:
    std::string title;
    
public:
    Document(const std::string& title) : title(title) {}
    virtual ~Document() = default;  // Important for polymorphism!
    
    virtual void open() = 0;  // Pure virtual
    virtual void save() = 0;  // Pure virtual
    virtual void print() {
        std::cout << "Printing document: " << title << std::endl;
    }
    
    const std::string& getTitle() const { return title; }
};

class TextDocument : public Document {
public:
    TextDocument(const std::string& title) : Document(title) {}
    
    void open() override {
        std::cout << "📄 Opening text document: " << title << std::endl;
    }
    
    void save() override {
        std::cout << "💾 Saving text document: " << title << std::endl;
    }
    
    void print() override {
        std::cout << "🖨️ Printing text document: " << title << " (plain text)" << std::endl;
    }
};

class SpreadsheetDocument : public Document {
public:
    SpreadsheetDocument(const std::string& title) : Document(title) {}
    
    void open() override {
        std::cout << "📊 Opening spreadsheet: " << title << std::endl;
    }
    
    void save() override {
        std::cout << "💾 Saving spreadsheet: " << title << " (with formulas)" << std::endl;
    }
    
    void print() override {
        std::cout << "🖨️ Printing spreadsheet: " << title << " (with grid lines)" << std::endl;
    }
};

class PresentationDocument : public Document {
public:
    PresentationDocument(const std::string& title) : Document(title) {}
    
    void open() override {
        std::cout << "🎞️ Opening presentation: " << title << std::endl;
    }
    
    void save() override {
        std::cout << "💾 Saving presentation: " << title << " (with slides)" << std::endl;
    }
    
    void print() override {
        std::cout << "🖨️ Printing presentation: " << title << " (slide handouts)" << std::endl;
    }
};

// Document Manager works with any type of document
class DocumentManager {
private:
    std::vector<std::unique_ptr<Document>> documents;
    
public:
    void addDocument(std::unique_ptr<Document> doc) {
        documents.push_back(std::move(doc));
    }
    
    void openAll() {
        std::cout << "\n=== Opening all documents ===" << std::endl;
        for (auto& doc : documents) {
            doc->open();  // Polymorphic call
        }
    }
    
    void saveAll() {
        std::cout << "\n=== Saving all documents ===" << std::endl;
        for (auto& doc : documents) {
            doc->save();  // Polymorphic call
        }
    }
    
    void printAll() {
        std::cout << "\n=== Printing all documents ===" << std::endl;
        for (auto& doc : documents) {
            doc->print();  // Polymorphic call - each type prints differently!
        }
    }
};

// Usage - manager works with any document type
DocumentManager manager;
manager.addDocument(std::make_unique<TextDocument>("Meeting Notes"));
manager.addDocument(std::make_unique<SpreadsheetDocument>("Budget 2024"));
manager.addDocument(std::make_unique<PresentationDocument>("Sales Pitch"));

manager.openAll();   // Each document opens in its own way
manager.saveAll();   // Each document saves in its own way  
manager.printAll();  // Each document prints in its own way
```

## Code Examples

The following examples demonstrate polymorphism with practical scenarios showing how different objects can be treated uniformly while maintaining their specific behaviors.
- C++: `g++ -std=c++17 chapter-05-basic.cpp -o chapter-05-basic && .\chapter-05-basic.exe`
- Python: `python chapter-05-basic.py`

**Expected Output:**
```
=== Shape Polymorphism Demo ===
Circle area: 78.54
Rectangle area: 24.00
Triangle area: 6.00

=== Polymorphic Array Demo ===
Drawing all shapes:
Drawing a circle with radius 5
Drawing a rectangle 6x4
Drawing a triangle with base 3, height 4
```

### Intermediate Example

**How to run:**
- C#: `csc chapter-05-intermediate.cs && .\chapter-05-intermediate.exe`
- C++: `g++ -std=c++17 chapter-05-intermediate.cpp -o chapter-05-intermediate && .\chapter-05-intermediate.exe`
- Python: `python chapter-05-intermediate.py`

**Expected Output:** Shows different payment methods being processed through the same interface.

### Advanced Example

**How to run:**
- C#: `csc chapter-05-advanced.cs && .\chapter-05-advanced.exe`
- C++: `g++ -std=c++17 chapter-05-advanced.cpp -o chapter-05-advanced && .\chapter-05-advanced.exe`
- Python: `python chapter-05-advanced.py`

**Why this advanced solution demonstrates excellent polymorphism:**
- **Interface Polymorphism**: Multiple interfaces allow objects to be polymorphic in different contexts
- **Strategy Pattern**: Algorithms are interchangeable through polymorphic interfaces
- **Template Method**: Base classes define structure while derived classes provide specific implementations
- **Multiple Inheritance**: Objects can be polymorphic through multiple inheritance paths

## Exercises

1. **Easy**: Create different musical instruments (`Piano`, `Guitar`, `Drums`) that all have a `play()` method but sound different.
   - *Hint: Create a base `Instrument` class with a virtual `play()` method*

2. **Medium**: Design a drawing application where different shapes can be drawn, moved, and resized through the same interface.
   - *Hint: Think about what operations all drawable objects should support*

3. **Hard**: Create a plugin system where different file processors (PDF, Word, Excel) can be loaded and used through the same interface.
   - *Hint: Design an interface that all processors must implement, then create specific implementations*

## Chapter Checklist

After completing this chapter, you should be able to:

- [ ] Understand the difference between compile-time and runtime polymorphism
- [ ] Use method overriding to create polymorphic behavior
- [ ] Design interfaces that enable polymorphism
- [ ] Write code that works with multiple types through a common interface
- [ ] Recognize when polymorphism is beneficial vs. unnecessary
- [ ] Implement polymorphic collections that hold different object types
- [ ] Understand how virtual methods enable runtime polymorphism
- [ ] Use polymorphism to eliminate conditional logic based on object types
- [ ] Design extensible systems using polymorphic interfaces