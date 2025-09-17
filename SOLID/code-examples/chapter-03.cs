// FILE: chapter-03-basic.cs
// Chapter 3 — Abstraction — Basic

using System;

// Abstract the complexity of TV operations behind simple methods
public class Television
{
    private bool isOn;
    private int volume;
    private int channel;
    private bool hasPower;
    
    public Television()
    {
        isOn = false;
        volume = 10;
        channel = 1;
        hasPower = true; // Assume TV is plugged in
    }
    
    // Simple interface - user doesn't need to know about internal state management
    public void TurnOn()
    {
        if (hasPower)
        {
            isOn = true;
            Console.WriteLine("TV is now ON");
        }
        else
        {
            Console.WriteLine("TV has no power!");
        }
    }
    
    public void TurnOff()
    {
        isOn = false;
        Console.WriteLine("TV is now OFF");
    }
    
    public void VolumeUp()
    {
        if (isOn && volume < 100)
        {
            volume++;
            Console.WriteLine($"Volume: {volume}");
        }
    }
    
    public void VolumeDown()
    {
        if (isOn && volume > 0)
        {
            volume--;
            Console.WriteLine($"Volume: {volume}");
        }
    }
    
    public void ChangeChannel(int newChannel)
    {
        if (isOn && newChannel > 0 && newChannel <= 999)
        {
            channel = newChannel;
            Console.WriteLine($"Channel: {channel}");
        }
    }
}

// Another example - Coffee machine abstraction
public class CoffeeMachine
{
    private bool hasWater;
    private bool hasBeans;
    private bool isReady;
    
    public CoffeeMachine()
    {
        hasWater = true;
        hasBeans = true;
        isReady = true;
    }
    
    // Simple interface hides complex coffee brewing process
    public void MakeCoffee()
    {
        if (CheckReadiness())
        {
            PerformBrewingProcess();
            Console.WriteLine("☕ Your coffee is ready!");
        }
        else
        {
            Console.WriteLine("❌ Cannot make coffee - machine needs maintenance");
        }
    }
    
    // Private methods hide implementation complexity
    private bool CheckReadiness()
    {
        return hasWater && hasBeans && isReady;
    }
    
    private void PerformBrewingProcess()
    {
        // Complex internal process hidden from user
        Console.WriteLine("Making coffee...");
        // Simulate brewing: heat water, grind beans, extract, etc.
        System.Threading.Thread.Sleep(1000); // Simulate brewing time
    }
}

class Program
{
    static void Main()
    {
        Console.WriteLine("=== TV Remote Control Demo ===");
        
        Television tv = new Television();
        
        // User interacts with simple interface
        tv.TurnOn();
        tv.VolumeUp();
        tv.VolumeUp();
        tv.VolumeUp();
        tv.VolumeUp();
        tv.VolumeUp();
        tv.ChangeChannel(5);
        tv.TurnOff();
        
        Console.WriteLine("\n=== Coffee Machine Demo ===");
        
        CoffeeMachine coffee = new CoffeeMachine();
        coffee.MakeCoffee();
        
        // Note: Users don't need to know about:
        // - Internal state management
        // - Complex brewing algorithms  
        // - Hardware control details
        // - Error handling mechanisms
    }
}