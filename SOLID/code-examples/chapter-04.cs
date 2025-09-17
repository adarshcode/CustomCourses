// FILE: chapter-04-basic.cs
// Chapter 4 — Inheritance — Basic

using System;

// Base class (parent) - simple animal with name
public class Animal
{
    public string Name;
    
    public Animal(string name)
    {
        Name = name;
    }
    
    public void Eat()
    {
        Console.WriteLine($"{Name} is eating");
    }
    
    public virtual void MakeSound()
    {
        Console.WriteLine($"{Name} makes a sound");
    }
}

// Derived class (child) - inherits from Animal
public class Dog : Animal
{
    public Dog(string name) : base(name)
    {
    }
    
    // Override parent method
    public override void MakeSound()
    {
        Console.WriteLine($"{Name} says: Woof!");
    }
    
    // Dog-specific behavior
    public void WagTail()
    {
        Console.WriteLine($"{Name} wags tail");
    }
}

class Program
{
    static void Main()
    {
        Console.WriteLine("🐕 Basic Inheritance Example");
        Console.WriteLine("===========================\n");
        
        // Create a dog
        Dog myDog = new Dog("Buddy");
        
        // Use inherited method
        myDog.Eat();        // From Animal class
        
        // Use overridden method
        myDog.MakeSound();  // Dog's version
        
        // Use dog-specific method
        myDog.WagTail();    // Only dogs can do this
        
        Console.WriteLine("\n💡 Inheritance Benefits:");
        Console.WriteLine("   ✓ Code reuse: Dog inherits Eat() from Animal");
        Console.WriteLine("   ✓ Customization: Dog overrides MakeSound()");
        Console.WriteLine("   ✓ Extension: Dog adds WagTail() behavior");
    }
}