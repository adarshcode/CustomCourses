# Chapter 9: Interface Segregation Principle (ISP)

## Child-Friendly Explanation
Imagine you go to a restaurant and they give you a huge menu with 1000 different foods - pizza, sushi, burgers, Indian food, Chinese food, desserts, drinks, everything! It's overwhelming and confusing. You just want pizza, but you have to look through all this other stuff you don't care about. The Interface Segregation Principle says it's better to have smaller, focused menus - one for pizza, one for drinks, one for desserts. That way, you only see what you actually need!

## Developer-Level Explanation
The Interface Segregation Principle states that **no client should be forced to depend on methods it does not use**. Instead of one large interface, it's better to have many smaller, focused interfaces that are specific to the needs of different clients.

**Why ISP matters:**
- **Reduces coupling**: Classes only depend on functionality they actually use
- **Improves maintainability**: Changes to unused methods don't affect clients
- **Enhances flexibility**: Different clients can implement only what they need
- **Prevents bloated interfaces**: Keeps interfaces focused and manageable

**How to identify ISP violations:**
- Large interfaces with many unrelated methods
- Classes implementing interfaces but leaving methods empty or throwing exceptions
- Different clients using completely different subsets of an interface

Let's see this in practice with code examples:

### Code Example: ISP Violation
```csharp
// BAD - Fat interface forces all implementations to have methods they don't need
public interface IWorker
{
    void Work();
    void Eat();       // Not all workers eat (robots don't)
    void Sleep();     // Not all workers sleep (robots don't)
    void GetSalary(); // Not all workers get salary (volunteers don't)
}

public class Human : IWorker
{
    public void Work() { Console.WriteLine("Human working"); }
    public void Eat() { Console.WriteLine("Human eating"); }
    public void Sleep() { Console.WriteLine("Human sleeping"); }
    public void GetSalary() { Console.WriteLine("Human getting salary"); }
}

public class Robot : IWorker
{
    public void Work() { Console.WriteLine("Robot working"); }
    public void Eat() { throw new NotImplementedException(); } // Problem!
    public void Sleep() { throw new NotImplementedException(); } // Problem!
    public void GetSalary() { throw new NotImplementedException(); } // Problem!
}
```

### Code Example: ISP Compliant
```csharp
// GOOD - Segregated interfaces
public interface IWorkable
{
    void Work();
}

public interface IEatable
{
    void Eat();
}

public interface ISleepable
{
    void Sleep();
}

public interface IPayable
{
    void GetSalary();
}

// Now each class implements only what it needs
public class Human : IWorkable, IEatable, ISleepable, IPayable
{
    public void Work() { Console.WriteLine("Human working"); }
    public void Eat() { Console.WriteLine("Human eating"); }
    public void Sleep() { Console.WriteLine("Human sleeping"); }
    public void GetSalary() { Console.WriteLine("Human getting salary"); }
}

public class Robot : IWorkable  // Only implements what it can do
{
    public void Work() { Console.WriteLine("Robot working"); }
}

public class Volunteer : IWorkable, IEatable, ISleepable  // No salary
{
    public void Work() { Console.WriteLine("Volunteer working"); }
    public void Eat() { Console.WriteLine("Volunteer eating"); }
    public void Sleep() { Console.WriteLine("Volunteer sleeping"); }
}
```

## Code Examples

### C# Example

**Expected Output:**
```
=== ISP Violation Demo ===
Human: Work, Eat, Sleep, GetSalary all work fine
Robot: Only Work works, others throw exceptions!

=== ISP Compliant Demo ===
Human worker: Human working
Human can eat: Human eating
Robot worker: Robot working
Volunteer worker: Volunteer working
Volunteer can eat: Volunteer eating
All workers can work, but each has only the capabilities they need!
```

## Exercises

1. **Easy**: Fix a `IVehicle` interface that has methods like `fly()`, `drive()`, and `sail()` - not all vehicles can do all these things.
   - *Hint: Create separate interfaces for different capabilities*

2. **Medium**: Design a document processor system where some processors can read, some can write, and some can both - without forcing unnecessary dependencies.
   - *Hint: Think about what each processor type actually needs*

3. **Hard**: Refactor a large `IMediaPlayer` interface that handles audio, video, streaming, and offline content into properly segregated interfaces.
   - *Hint: Different media players have different capabilities*

## Chapter Checklist

After completing this chapter, you should be able to:

- [ ] Identify fat interfaces that violate ISP
- [ ] Split large interfaces into smaller, focused ones
- [ ] Design interfaces based on client needs, not implementation convenience
- [ ] Recognize when classes are forced to implement unused methods
- [ ] Use composition of multiple small interfaces instead of one large interface
- [ ] Understand the relationship between ISP and other SOLID principles
- [ ] Balance between too many tiny interfaces and too few large ones
- [ ] Apply ISP to improve code maintainability and flexibility