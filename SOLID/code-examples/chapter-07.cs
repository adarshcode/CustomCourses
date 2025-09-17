// FILE: chapter-07-basic.cs
// Chapter 7 — Open/Closed Principle — Basic

using System;
using System.Collections.Generic;

// BAD EXAMPLE - Violates OCP
public enum ShapeType
{
    Rectangle,
    Circle
}

public class BadShape
{
    public ShapeType Type { get; set; }
    public double Width { get; set; }
    public double Height { get; set; }
    public double Radius { get; set; }
}

public class BadAreaCalculator
{
    public double CalculateArea(BadShape shape)
    {
        // This method needs to be modified every time we add a new shape!
        switch (shape.Type)
        {
            case ShapeType.Rectangle:
                return shape.Width * shape.Height;
            case ShapeType.Circle:
                return Math.PI * shape.Radius * shape.Radius;
            default:
                throw new ArgumentException("Unknown shape type");
        }
        // If we want to add Triangle, we need to modify this method!
    }
}

// GOOD EXAMPLE - Follows OCP
// Open for extension (new shapes), closed for modification (existing code)

public abstract class Shape
{
    // Abstract method forces all shapes to implement area calculation
    public abstract double CalculateArea();
}

public class Rectangle : Shape
{
    private double width;
    private double height;
    
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

public class Circle : Shape
{
    private double radius;
    
    public Circle(double radius)
    {
        this.radius = radius;
    }
    
    public override double CalculateArea()
    {
        return Math.PI * radius * radius;
    }
}

// NEW SHAPE - Can be added without modifying existing code!
public class Triangle : Shape
{
    private double baseLength;
    private double height;
    
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

// This can be added later without modifying any existing code
public class Hexagon : Shape
{
    private double sideLength;
    
    public Hexagon(double sideLength)
    {
        this.sideLength = sideLength;
    }
    
    public override double CalculateArea()
    {
        return (3 * Math.Sqrt(3) / 2) * sideLength * sideLength;
    }
}

public class GoodAreaCalculator
{
    // This method NEVER needs to be modified when adding new shapes!
    public double CalculateArea(Shape shape)
    {
        return shape.CalculateArea();
    }
    
    // Can work with any collection of shapes
    public double CalculateTotalArea(List<Shape> shapes)
    {
        double totalArea = 0;
        foreach (Shape shape in shapes)
        {
            totalArea += shape.CalculateArea();
        }
        return totalArea;
    }
}

class Program
{
    static void Main()
    {
        Console.WriteLine("=== OCP Violation Example ===");
        
        var badCalculator = new BadAreaCalculator();
        var badRect = new BadShape { Type = ShapeType.Rectangle, Width = 6, Height = 4 };
        var badCircle = new BadShape { Type = ShapeType.Circle, Radius = 5 };
        
        Console.WriteLine($"Rectangle area: {badCalculator.CalculateArea(badRect)}");
        Console.WriteLine($"Circle area: {badCalculator.CalculateArea(badCircle):F2}");
        
        Console.WriteLine("\n=== OCP Compliant Example ===");
        
        var goodCalculator = new GoodAreaCalculator();
        var shapes = new List<Shape>
        {
            new Rectangle(6, 4),
            new Circle(5),
            new Triangle(3, 4)
        };
        
        foreach (Shape shape in shapes)
        {
            Console.WriteLine($"{shape.GetType().Name} area: {goodCalculator.CalculateArea(shape):F2}");
        }
        
        Console.WriteLine("\nAdding new shape without modifying existing code!");
        shapes.Add(new Hexagon(5));
        
        Console.WriteLine($"Hexagon area: {goodCalculator.CalculateArea(shapes[3]):F0}");
        
        Console.WriteLine($"\nTotal area of all shapes: {goodCalculator.CalculateTotalArea(shapes):F2}");
        
        Console.WriteLine("\n=== OCP Benefits ===");
        Console.WriteLine("✓ New shapes can be added without changing existing code");
        Console.WriteLine("✓ Existing code remains stable and tested");
        Console.WriteLine("✓ Each shape encapsulates its own calculation logic");
        Console.WriteLine("✓ No need to modify calculator for new shapes");
    }
}