// FILE: chapter-02-basic.cs
// Chapter 2 — Encapsulation — Basic

using System;

public class Temperature
{
    // Private field - can only be accessed within this class
    private double celsius;
    
    // Constructor
    public Temperature(double initialTemp)
    {
        SetTemperature(initialTemp);
    }
    
    // Public method to get temperature (getter)
    public double GetTemperature()
    {
        return celsius;
    }
    
    // Public method to set temperature with validation (setter)
    public void SetTemperature(double newTemp)
    {
        // Absolute zero is -273.15°C - nothing can be colder
        if (newTemp >= -273.15)
        {
            celsius = newTemp;
            Console.WriteLine($"New temperature: {celsius}°C");
        }
        else
        {
            Console.WriteLine("Error: Temperature cannot be below absolute zero (-273.15°C)");
        }
    }
    
    // Public method to display temperature
    public void Display()
    {
        Console.WriteLine($"Current temperature: {celsius}°C");
    }
}

class Program
{
    static void Main()
    {
        // Create a temperature object
        Temperature roomTemp = new Temperature(20.0);
        
        Console.WriteLine($"Initial temperature: {roomTemp.GetTemperature()}°C");
        
        // Try to set a valid temperature
        roomTemp.SetTemperature(25.0);
        
        // Try to set an invalid temperature
        roomTemp.SetTemperature(-300.0);
        
        Console.WriteLine($"Temperature remains: {roomTemp.GetTemperature()}°C");
        
        // Note: We cannot do roomTemp.celsius = -300; 
        // because celsius is private!
    }
}