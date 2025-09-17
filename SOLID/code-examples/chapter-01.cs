// FILE: chapter-01-basic.cs
// Chapter 1 — What is OOP? — Basic

using System;

// A simple class representing a Dog - this is our "blueprint"
public class Dog
{
    // Properties (data/state)
    public string Name;
    public int Age;
    
    // Method (behavior)
    public void Bark()
    {
        Console.WriteLine($"{Name} says: Woof!");
    }
}

class Program
{
    static void Main()
    {
        // Creating objects (instances) from our class
        Dog myDog = new Dog();
        myDog.Name = "Buddy";
        myDog.Age = 3;
        
        Dog friendDog = new Dog();
        friendDog.Name = "Max";
        friendDog.Age = 5;
        
        // Using the objects
        Console.WriteLine($"My dog {myDog.Name} is {myDog.Age} years old");
        myDog.Bark();
        
        Console.WriteLine($"Friend's dog {friendDog.Name} is {friendDog.Age} years old");
        friendDog.Bark();
    }
}