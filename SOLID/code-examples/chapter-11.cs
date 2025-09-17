// FILE: chapter-11-basic.cs
// Chapter 11 — Essential Design Patterns — Basic

using System;

// Abstract base class for all animals
public abstract class Animal
{
    public abstract void MakeSound();
}

// Concrete animal implementations
public class Dog : Animal
{
    public override void MakeSound()
    {
        Console.WriteLine("Dog says: Woof!");
    }
}

public class Cat : Animal  
{
    public override void MakeSound()
    {
        Console.WriteLine("Cat says: Meow!");
    }
}

public class Bird : Animal
{
    public override void MakeSound()
    {
        Console.WriteLine("Bird says: Tweet!");
    }
}

// Factory Pattern Implementation
public static class AnimalFactory
{
    public static Animal CreateAnimal(string animalType)
    {
        switch (animalType.ToLower())
        {
            case "dog":
                return new Dog();
            case "cat":
                return new Cat();
            case "bird":
                return new Bird();
            default:
                throw new ArgumentException($"Unknown animal type: {animalType}");
        }
    }
}

class Program
{
    static void Main()
    {
        Console.WriteLine("=== Factory Pattern Demo ===");
        
        // Creating animals directly
        Animal dog = new Dog();
        Animal cat = new Cat();
        Animal bird = new Bird();
        
        dog.MakeSound();
        cat.MakeSound();
        bird.MakeSound();
        
        Console.WriteLine("\nCreating animals by type:");
        
        // Using factory pattern - we don't need to know specific classes
        Animal factoryDog = AnimalFactory.CreateAnimal("dog");
        Animal factoryCat = AnimalFactory.CreateAnimal("cat");
        
        factoryDog.MakeSound();
        factoryCat.MakeSound();
        
        // Factory pattern benefits:
        // 1. Client code doesn't need to know about specific classes
        // 2. Easy to add new animal types without changing client code
        // 3. Centralized creation logic
        
        Console.WriteLine("\n=== Factory Pattern Benefits ===");
        Console.WriteLine("✓ Easy to extend with new animal types");
        Console.WriteLine("✓ Client code doesn't depend on concrete classes");
        Console.WriteLine("✓ Creation logic is centralized");
    }
}