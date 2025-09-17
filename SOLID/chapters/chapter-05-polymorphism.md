# Chapter 5: Polymorphism - One Interface, Many Forms

## Child-Friendly Explanation
Imagine you have a remote control that has a "play" button. When you point it at a TV and press play, it plays a TV show. When you point it at a music player and press play, it plays music. When you point it at a game console and press play, it starts a game. It's the same button doing the same action ("play"), but each device responds differently! Polymorphism in programming is just like this - the same action can work differently depending on what object you're using it with.

```csharp
// Same method name, different behaviors
public class TV
{
    public void Play()
    {
        Console.WriteLine("📺 Playing TV show...");
    }
}

public class MusicPlayer
{
    public void Play()
    {
        Console.WriteLine("🎵 Playing music...");
    }
}

public class GameConsole
{
    public void Play()
    {
        Console.WriteLine("🎮 Starting game...");
    }
}

// Same remote control works with all devices!
public static void UseRemoteControl(dynamic device)
{
    Console.WriteLine("Pressing PLAY button...");
    device.Play();  // Same method call, different behavior!
}

// All work with the same interface
var tv = new TV();
var music = new MusicPlayer();
var game = new GameConsole();

UseRemoteControl(tv);     // 📺 Playing TV show...
UseRemoteControl(music);  // 🎵 Playing music...
UseRemoteControl(game);   // 🎮 Starting game...
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

```csharp
// Multiple interfaces that objects can implement
public interface IDrawable
{
    void Draw();
}

public interface IMovable
{
    void Move(int x, int y);
}

public interface IResizable
{
    void Resize(double factor);
}

// Objects can implement multiple interfaces
public class GameSprite : IDrawable, IMovable, IResizable
{
    protected string name;
    protected int x, y;
    protected double size;
    
    public GameSprite(string name, int x = 0, int y = 0, double size = 1)
    {
        this.name = name;
        this.x = x;
        this.y = y;
        this.size = size;
    }
    
    public virtual void Draw()
    {
        Console.WriteLine($"Drawing {name} at ({x}, {y}) with size {size}");
    }
    
    public virtual void Move(int deltaX, int deltaY)
    {
        x += deltaX;
        y += deltaY;
        Console.WriteLine($"{name} moved to ({x}, {y})");
    }
    
    public virtual void Resize(double factor)
    {
        size *= factor;
        Console.WriteLine($"{name} resized to {size}");
    }
}

public class Player : GameSprite
{
    private int health;
    
    public Player(string name, int health = 100) : base(name)
    {
        this.health = health;
    }
    
    public override void Draw()  // Override with player-specific drawing
    {
        Console.WriteLine($"🏃 Drawing player {name} (HP: {health}) at ({x}, {y})");
    }
}

public class Enemy : GameSprite
{
    private int damage;
    
    public Enemy(string name, int damage = 10) : base(name)
    {
        this.damage = damage;
    }
    
    public override void Draw()  // Override with enemy-specific drawing
    {
        Console.WriteLine($"👹 Drawing enemy {name} (DMG: {damage}) at ({x}, {y})");
    }
}

// Polymorphic functions work with any object implementing the interface
public static void RenderAll(List<IDrawable> drawables)
{
    foreach (var drawable in drawables)
    {
        drawable.Draw();  // Calls the appropriate draw method
    }
}

public static void MoveAll(List<IMovable> movables, int dx, int dy)
{
    foreach (var movable in movables)
    {
        movable.Move(dx, dy);  // Calls the appropriate move method
    }
}

// Usage - polymorphism in action
var gameObjects = new List<GameSprite>
{
    new Player("Hero"),
    new Enemy("Goblin"),
    new Enemy("Orc"),
    new GameSprite("Treasure Chest")
};

Console.WriteLine("=== Rendering all objects ===");
RenderAll(gameObjects.Cast<IDrawable>().ToList());  // Works with any Drawable object

Console.WriteLine("\n=== Moving all objects ===");
MoveAll(gameObjects.Cast<IMovable>().ToList(), 5, 3);  // Works with any Movable object
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

```csharp
public abstract class Document 
{
    protected string title;
    
    public Document(string title) 
    {
        this.title = title;
    }
    
    public abstract void Open();  // Must be implemented
    public abstract void Save();  // Must be implemented
    
    public virtual void Print() 
    {
        Console.WriteLine($"Printing document: {title}");
    }
    
    public string GetTitle() => title;
}

public class TextDocument : Document 
{
    public TextDocument(string title) : base(title) { }
    
    public override void Open() 
    {
        Console.WriteLine($"📄 Opening text document: {title}");
    }
    
    public override void Save() 
    {
        Console.WriteLine($"💾 Saving text document: {title}");
    }
    
    public override void Print() 
    {
        Console.WriteLine($"🖨️ Printing text document: {title} (plain text)");
    }
}

public class SpreadsheetDocument : Document 
{
    public SpreadsheetDocument(string title) : base(title) { }
    
    public override void Open() 
    {
        Console.WriteLine($"📊 Opening spreadsheet: {title}");
    }
    
    public override void Save() 
    {
        Console.WriteLine($"💾 Saving spreadsheet: {title} (with formulas)");
    }
    
    public override void Print() 
    {
        Console.WriteLine($"🖨️ Printing spreadsheet: {title} (with grid lines)");
    }
}

public class PresentationDocument : Document 
{
    public PresentationDocument(string title) : base(title) { }
    
    public override void Open() 
    {
        Console.WriteLine($"🎞️ Opening presentation: {title}");
    }
    
    public override void Save() 
    {
        Console.WriteLine($"💾 Saving presentation: {title} (with slides)");
    }
    
    public override void Print() 
    {
        Console.WriteLine($"🖨️ Printing presentation: {title} (slide handouts)");
    }
}

// Document Manager works with any type of document
public class DocumentManager 
{
    private List<Document> documents = new List<Document>();
    
    public void AddDocument(Document doc) 
    {
        documents.Add(doc);
    }
    
    public void OpenAll() 
    {
        Console.WriteLine("\n=== Opening all documents ===");
        foreach (var doc in documents) 
        {
            doc.Open();  // Polymorphic call
        }
    }
    
    public void SaveAll() 
    {
        Console.WriteLine("\n=== Saving all documents ===");
        foreach (var doc in documents) 
        {
            doc.Save();  // Polymorphic call
        }
    }
    
    public void PrintAll() 
    {
        Console.WriteLine("\n=== Printing all documents ===");
        foreach (var doc in documents) 
        {
            doc.Print();  // Polymorphic call - each type prints differently!
        }
    }
}

// Usage - manager works with any document type
var manager = new DocumentManager();
manager.AddDocument(new TextDocument("Meeting Notes"));
manager.AddDocument(new SpreadsheetDocument("Budget 2024"));
manager.AddDocument(new PresentationDocument("Sales Pitch"));

manager.OpenAll();   // Each document opens in its own way
manager.SaveAll();   // Each document saves in its own way  
manager.PrintAll();  // Each document prints in its own way
```

**Code Explanation:**
This demonstrates **polymorphic collections** where a single `DocumentManager` class can handle different document types (`TextDocument`, `SpreadsheetDocument`, `PresentationDocument`) uniformly. Each document type overrides the abstract methods with its own specific behavior, but the manager treats them all as `Document` objects. This shows how polymorphism enables **flexible collection handling** and **type-safe operations** across diverse object types.

## Code Examples

The following examples demonstrate polymorphism with practical scenarios showing how different objects can be treated uniformly while maintaining their specific behaviors.

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

**Expected Output:** Shows different payment methods being processed through the same interface.

### Advanced Example

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