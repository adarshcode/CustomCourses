// FILE: chapter-08-basic.cs
// Chapter 8 — Liskov Substitution Principle — Basic

using System;

// BAD EXAMPLE - Violates LSP
public class BadRectangle
{
    protected double width;
    protected double height;
    
    public virtual double Width
    {
        get => width;
        set => width = value;
    }
    
    public virtual double Height
    {
        get => height;
        set => height = value;
    }
    
    public double Area => width * height;
    
    public BadRectangle(double width, double height)
    {
        this.width = width;
        this.height = height;
    }
}

public class BadSquare : BadRectangle
{
    public BadSquare(double side) : base(side, side) { }
    
    // This violates LSP - changes the behavior of the base class
    public override double Width
    {
        get => width;
        set
        {
            if (Math.Abs(value - height) > 0.001)
                throw new ArgumentException("Square width and height must be equal");
            width = value;
            height = value;
        }
    }
    
    public override double Height
    {
        get => height;
        set
        {
            if (Math.Abs(value - width) > 0.001)
                throw new ArgumentException("Square width and height must be equal");
            width = value;
            height = value;
        }
    }
}

// GOOD EXAMPLE - Follows LSP
public abstract class Shape
{
    public abstract double Area { get; }
    public abstract void Draw();
}

public class GoodRectangle : Shape
{
    protected double width;
    protected double height;
    
    public double Width => width;
    public double Height => height;
    public override double Area => width * height;
    
    public GoodRectangle(double width, double height)
    {
        this.width = width;
        this.height = height;
    }
    
    // Allows modification while maintaining rectangle properties
    public void SetDimensions(double newWidth, double newHeight)
    {
        width = newWidth;
        height = newHeight;
    }
    
    public override void Draw()
    {
        Console.WriteLine($"Drawing rectangle: {width} x {height}");
    }
}

public class GoodSquare : Shape
{
    private double side;
    
    public double Side => side;
    public override double Area => side * side;
    
    public GoodSquare(double side)
    {
        this.side = side;
    }
    
    // Specific to square - maintains square properties
    public void SetSide(double newSide)
    {
        side = newSide;
    }
    
    public override void Draw()
    {
        Console.WriteLine($"Drawing square: {side} x {side}");
    }
}

// Client code that demonstrates LSP
public class ShapeProcessor
{
    // This should work with any Shape without knowing the specific type
    public void ProcessShape(Shape shape)
    {
        Console.WriteLine($"Processing shape with area: {shape.Area}");
        shape.Draw();
    }
    
    // This demonstrates the LSP violation
    public void ProcessBadRectangle(BadRectangle rectangle)
    {
        Console.WriteLine($"Rectangle width: {rectangle.Width}, height: {rectangle.Height}, area: {rectangle.Area}");
        
        // This works fine for BadRectangle but might fail for BadSquare
        try
        {
            rectangle.Width = 10;
            rectangle.Height = 5; // This will throw an exception if rectangle is actually a BadSquare!
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Exception: {ex.Message}");
        }
    }
}

class Program
{
    static void Main()
    {
        var processor = new ShapeProcessor();
        
        Console.WriteLine("=== LSP Violation Example ===");
        
        BadRectangle badRect = new BadRectangle(10, 5);
        processor.ProcessBadRectangle(badRect);
        
        Console.WriteLine("\nTrying to set square dimensions...");
        BadRectangle badSquare = new BadSquare(7); // Treated as a Rectangle
        processor.ProcessBadRectangle(badSquare); // This will throw an exception!
        
        Console.WriteLine("\n=== LSP Compliant Example ===");
        
        Shape goodRect = new GoodRectangle(10, 5);
        Shape goodSquare = new GoodSquare(7);
        
        // Both work correctly with the same interface
        processor.ProcessShape(goodRect);
        processor.ProcessShape(goodSquare);
        
        Console.WriteLine("Both shapes work correctly with the same interface!");
        
        Console.WriteLine("\n=== LSP Benefits ===");
        Console.WriteLine("✓ Subclasses can be used wherever base class is expected");
        Console.WriteLine("✓ No unexpected exceptions or behavior changes");
        Console.WriteLine("✓ Polymorphism works reliably");
        Console.WriteLine("✓ Client code doesn't need to know about specific subclasses");
        
        // Demonstrate safe polymorphism
        Console.WriteLine("\n=== Safe Polymorphism ===");
        Shape[] shapes = { new GoodRectangle(8, 6), new GoodSquare(4) };
        
        foreach (Shape shape in shapes)
        {
            processor.ProcessShape(shape); // Works reliably for all shapes
        }
    }
}