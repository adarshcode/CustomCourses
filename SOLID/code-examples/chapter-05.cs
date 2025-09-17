// FILE: chapter-05-basic.cs
// Chapter 5 — Polymorphism — Basic

using System;

// Base class with virtual method for polymorphism
public abstract class Shape
{
    protected string name;
    
    public Shape(string name)
    {
        this.name = name;
    }
    
    // Abstract method - must be implemented by derived classes
    public abstract double CalculateArea();
    
    // Virtual method - can be overridden by derived classes
    public virtual void Draw()
    {
        Console.WriteLine($"Drawing a {name}");
    }
    
    public virtual void DisplayInfo()
    {
        Console.WriteLine($"{name} area: {CalculateArea():F2}");
    }
}

// Derived classes implementing polymorphic behavior
public class Circle : Shape
{
    private double radius;
    
    public Circle(double radius) : base("circle")
    {
        this.radius = radius;
    }
    
    // Polymorphic implementation of abstract method
    public override double CalculateArea()
    {
        return Math.PI * radius * radius;
    }
    
    // Polymorphic implementation of virtual method
    public override void Draw()
    {
        Console.WriteLine($"Drawing a circle with radius {radius}");
    }
}

public class Rectangle : Shape
{
    private double width;
    private double height;
    
    public Rectangle(double width, double height) : base("rectangle")
    {
        this.width = width;
        this.height = height;
    }
    
    // Polymorphic implementation
    public override double CalculateArea()
    {
        return width * height;
    }
    
    public override void Draw()
    {
        Console.WriteLine($"Drawing a rectangle {width}x{height}");
    }
}

public class Triangle : Shape
{
    private double baseLength;
    private double height;
    
    public Triangle(double baseLength, double height) : base("triangle")
    {
        this.baseLength = baseLength;
        this.height = height;
    }
    
    // Polymorphic implementation
    public override double CalculateArea()
    {
        return 0.5 * baseLength * height;
    }
    
    public override void Draw()
    {
        Console.WriteLine($"Drawing a triangle with base {baseLength}, height {height}");
    }
}

class Program
{
    static void Main()
    {
        Console.WriteLine("=== Shape Polymorphism Demo ===");
        
        // Create different shapes
        Shape circle = new Circle(5);
        Shape rectangle = new Rectangle(6, 4);
        Shape triangle = new Triangle(3, 4);
        
        // Polymorphism in action - same method call, different behavior
        circle.DisplayInfo();      // Calls Circle's CalculateArea()
        rectangle.DisplayInfo();   // Calls Rectangle's CalculateArea()
        triangle.DisplayInfo();    // Calls Triangle's CalculateArea()
        
        Console.WriteLine("\n=== Polymorphic Array Demo ===");
        
        // Array of base type can hold any derived type
        Shape[] shapes = { circle, rectangle, triangle };
        
        Console.WriteLine("Drawing all shapes:");
        foreach (Shape shape in shapes)
        {
            shape.Draw(); // Calls the appropriate Draw() method for each shape
        }
        
        Console.WriteLine("\n=== Benefits of Polymorphism ===");
        Console.WriteLine("✓ Same interface (CalculateArea) works for all shapes");
        Console.WriteLine("✓ New shapes can be added without changing existing code");
        Console.WriteLine("✓ Collections can hold different types uniformly");
        Console.WriteLine("✓ No need for if-else chains to handle different types");
        
        // Demonstrate adding new functionality without changing existing code
        Console.WriteLine("\n=== Processing All Shapes ===");
        ProcessShapes(shapes);
    }
    
    // This method works with any Shape, demonstrating polymorphism
    static void ProcessShapes(Shape[] shapes)
    {
        double totalArea = 0;
        
        foreach (Shape shape in shapes)
        {
            // Polymorphic method calls - each shape calculates area differently
            totalArea += shape.CalculateArea();
        }
        
        Console.WriteLine($"Total area of all shapes: {totalArea:F2}");
    }
}