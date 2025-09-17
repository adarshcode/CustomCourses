# Chapter 4: Inheritance - Building on Existing Code

## Child-Friendly Explanation 🧸
Imagine you have a bicycle, and someone invents a motorcycle. Instead of building the motorcycle completely from scratch, they can start with the bicycle (it already has wheels, handlebars, and pedals) and then add an engine, remove the pedals, and make other changes. Inheritance in programming is like this - you can create new things by starting with something that already exists and adding or changing just the parts you need!

```csharp
// Start with a basic Animal
public class Animal
{
    public string Name { get; protected set; }
    
    public Animal(string name)
    {
        Name = name;
    }
    
    public void Eat()
    {
        Console.WriteLine($"{Name} is eating");
    }
    
    public void Sleep()
    {
        Console.WriteLine($"{Name} is sleeping");
    }
}

// Create a Dog by inheriting from Animal
public class Dog : Animal  // Dog gets everything Animal has!
{
    public Dog(string name) : base(name) { }
    
    public void Bark()  // Plus new abilities
    {
        Console.WriteLine($"{Name} says: Woof!");
    }
}

// Create a Cat by inheriting from Animal  
public class Cat : Animal  // Cat also gets everything Animal has!
{
    public Cat(string name) : base(name) { }
    
    public void Meow()  // Plus different new abilities
    {
        Console.WriteLine($"{Name} says: Meow!");
    }
}

// Both work because they inherit from Animal
var buddy = new Dog("Buddy");
var whiskers = new Cat("Whiskers");

// They can do Animal things
buddy.Eat();     // Buddy is eating
whiskers.Sleep(); // Whiskers is sleeping

// Plus their own special things
buddy.Bark();    // Buddy says: Woof!
whiskers.Meow(); // Whiskers says: Meow!
```

**Code Explanation:**
This example demonstrates **basic inheritance** where `Dog` and `Cat` classes inherit from `Animal`. Both child classes automatically get the `Name` property, `Eat()`, and `Sleep()` methods from the parent class, while adding their own specific behaviors (`Bark()`, `Meow()`). The `: base(name)` syntax passes the name parameter to the parent constructor.

## Developer-Level Explanation
Inheritance is a mechanism that allows a new class (child/derived class) to inherit properties and methods from an existing class (parent/base class). The child class can use everything from the parent class and can also add new features or modify existing ones.

**Why inheritance matters:**
- **Code Reuse**: Avoid duplicating code by building on existing functionality
- **Hierarchical Organization**: Model real-world relationships naturally
- **Polymorphism Enablement**: Allows objects of different types to be treated uniformly
- **Maintainability**: Changes to common functionality can be made in one place

**Inheritance relationships:**
- **"Is-a" relationship**: Dog is-a Animal (use inheritance)
- **"Has-a" relationship**: Car has-a Engine (use composition, not inheritance)

**Common pitfalls:**
- Using inheritance for "has-a" relationships instead of "is-a"
- Creating deep inheritance hierarchies that become hard to understand
- Inheriting from classes just to reuse some code (favor composition)
- Breaking the Liskov Substitution Principle

### Method Overriding: Customizing Inherited Behavior

```csharp
public class Vehicle 
{
    public string Brand { get; set; }
    
    public virtual void Start()  // 'virtual' allows overriding
    {
        Console.WriteLine($"{Brand} vehicle is starting...");
    }
    
    public virtual void Stop() 
    {
        Console.WriteLine($"{Brand} vehicle has stopped.");
    }
}

public class Car : Vehicle 
{
    public override void Start()  // Customize the inherited method
    {
        Console.WriteLine("🔑 Inserting key...");
        Console.WriteLine("🚗 Car engine started!");
        base.Start();  // Call parent method if needed
    }
}

public class ElectricCar : Vehicle 
{
    public override void Start() 
    {
        Console.WriteLine("🔋 Checking battery level...");
        Console.WriteLine("⚡ Electric motor started silently!");
        // Notice: no base.Start() call - completely different behavior
    }
}

// Usage shows polymorphism in action
Vehicle[] vehicles = { 
    new Car { Brand = "Toyota" },
    new ElectricCar { Brand = "Tesla" }
};

foreach (Vehicle vehicle in vehicles) 
{
    vehicle.Start();  // Calls the appropriate overridden method
}
```

### Inheritance Hierarchies: Building Complex Relationships

```csharp
// Base class with common functionality
public class Employee 
{
    protected string name;
    protected int id;
    protected double baseSalary;
    
    public Employee(string name, int id, double baseSalary) 
    {
        this.name = name;
        this.id = id;
        this.baseSalary = baseSalary;
    }
    
    public virtual double CalculateSalary() 
    {
        return baseSalary;
    }
    
    public virtual void DisplayInfo() 
    {
        Console.WriteLine($"Employee: {name} (ID: {id})");
    }
}

// Derived class with additional functionality
public class Manager : Employee 
{
    private double bonus;
    private List<Employee> subordinates = new List<Employee>();
    
    public Manager(string name, int id, double baseSalary, double bonus) 
        : base(name, id, baseSalary) 
    {
        this.bonus = bonus;
    }
    
    // Override to include bonus
    public override double CalculateSalary() 
    {
        return baseSalary + bonus;
    }
    
    // Add manager-specific functionality
    public void AddSubordinate(Employee employee) 
    {
        subordinates.Add(employee);
    }
    
    public override void DisplayInfo() 
    {
        base.DisplayInfo();  // Call parent method
        Console.WriteLine("Position: Manager");
        Console.WriteLine($"Manages {subordinates.Count} employees");
        Console.WriteLine($"Total Salary: ${CalculateSalary()}");
    }
}

// Another derived class with different specialization
public class Developer : Employee 
{
    private string programmingLanguage;
    private int projectsCompleted;
    
    public Developer(string name, int id, double baseSalary, string language) 
        : base(name, id, baseSalary) 
    {
        this.programmingLanguage = language;
        this.projectsCompleted = 0;
    }
    
    // Override with performance-based calculation
    public override double CalculateSalary() 
    {
        double performanceBonus = projectsCompleted * 1000;  // $1000 per project
        return baseSalary + performanceBonus;
    }
    
    public void CompleteProject() 
    {
        projectsCompleted++;
        Console.WriteLine($"{name} completed a project in {programmingLanguage}!");
    }
    
    public override void DisplayInfo() 
    {
        base.DisplayInfo();
        Console.WriteLine("Position: Developer");
        Console.WriteLine($"Language: {programmingLanguage}");
        Console.WriteLine($"Projects: {projectsCompleted}");
        Console.WriteLine($"Total Salary: ${CalculateSalary()}");
    }
}
```

**Code Explanation:**
This demonstrates **inheritance hierarchies** where both `Manager` and `Developer` inherit from `Employee`. Each class overrides the `CalculateSalary()` method with its own logic (managers get bonuses, developers get performance bonuses), showing how inheritance enables **specialized behavior** while maintaining a common interface.

### Constructor Chaining: Proper Initialization

```csharp
public class Vehicle
{
    protected string make;
    protected string model;
    protected int year;
    protected bool isRunning;
    
    public Vehicle(string make, string model, int year)
    {
        this.make = make;
        this.model = model;
        this.year = year;
        this.isRunning = false;
        Console.WriteLine($"Vehicle created: {year} {make} {model}");
    }
    
    public virtual void Start()
    {
        if (!isRunning)
        {
            isRunning = true;
            Console.WriteLine($"{make} {model} started");
        }
        else
        {
            Console.WriteLine("Vehicle is already running");
        }
    }
    
    public virtual void Stop()
    {
        if (isRunning)
        {
            isRunning = false;
            Console.WriteLine($"{make} {model} stopped");
        }
        else
        {
            Console.WriteLine("Vehicle is already stopped");
        }
    }
    
    public string GetInfo()
    {
        return $"{year} {make} {model}";
    }
}

public class Car : Vehicle
{
    private int doors;
    private string fuelType;
    private bool trunkOpen;
    
    public Car(string make, string model, int year, int doors, string fuelType = "gasoline") 
        : base(make, model, year)  // Call parent constructor
    {
        this.doors = doors;
        this.fuelType = fuelType;
        this.trunkOpen = false;
        Console.WriteLine($"Car initialized with {doors} doors and {fuelType} fuel");
    }
    
    public void OpenTrunk()
    {
        trunkOpen = true;
        Console.WriteLine($"Trunk opened on {GetInfo()}");
    }
    
    public void Honk()
    {
        Console.WriteLine($"{make} {model}: BEEP BEEP!");
    }
    
    // Override parent method with car-specific behavior
    public override void Start()
    {
        Console.WriteLine("🔑 Turning key in ignition...");
        base.Start();  // Call parent's start method
        if (isRunning)
        {
            Console.WriteLine("🚗 Ready to drive!");
        }
    }
}

public class Motorcycle : Vehicle
{
    private int engineSize;
    private bool kickstandDown;
    
    public Motorcycle(string make, string model, int year, int engineSize) 
        : base(make, model, year)  // Call parent constructor
    {
        this.engineSize = engineSize;
        this.kickstandDown = true;
        Console.WriteLine($"Motorcycle initialized with {engineSize}cc engine");
    }
    
    public void KickStart()
    {
        if (kickstandDown)
        {
            Console.WriteLine("⚠️ Put kickstand up before starting!");
            return;
        }
        Console.WriteLine("🦵 Kick starting motorcycle...");
        base.Start();
        if (isRunning)
        {
            Console.WriteLine("🏍️ Ready to ride!");
        }
    }
    
    public void PutKickstandUp()
    {
        kickstandDown = false;
        Console.WriteLine("Kickstand up");
    }
    
    // Override with motorcycle-specific behavior
    public override void Start()
    {
        if (kickstandDown)
        {
            Console.WriteLine("⚠️ Cannot start with kickstand down!");
            return;
        }
        Console.WriteLine("🔑 Pressing start button...");
        base.Start();
        if (isRunning)
        {
            Console.WriteLine("🏍️ Motorcycle ready!");
        }
    }
}

// Usage showing inheritance in action
var car = new Car("Toyota", "Camry", 2023, 4, "hybrid");
var motorcycle = new Motorcycle("Harley-Davidson", "Sportster", 2023, 883);

Console.WriteLine("\n--- Car Operations ---");
car.Start();
car.Honk();
car.OpenTrunk();

Console.WriteLine("\n--- Motorcycle Operations ---");
motorcycle.Start();  // Will fail - kickstand down
motorcycle.PutKickstandUp();
motorcycle.Start();  // Now works
```

**Code Explanation:**
This demonstrates **constructor chaining** using the `: base()` syntax in C#. Each derived class constructor calls its parent constructor first, ensuring proper initialization order. The example shows **method overriding** where both `Car` and `Motorcycle` override the `Start()` method with vehicle-specific behavior while still calling the parent implementation with `base.Start()`. This pattern ensures consistent functionality while allowing specialization.

## Code Examples

The following examples demonstrate inheritance patterns with increasing complexity, showing proper hierarchies and method overriding.
=== Animal Hierarchy Demo ===
Buddy is eating food
Buddy says: Woof! Woof!
Buddy is wagging tail

Whiskers is eating food
Whiskers says: Meow!
Whiskers is climbing tree
```

### Inheritance vs Composition: Making the Right Choice

**When to Use Inheritance (Is-a relationship):**
```csharp
// Good - Dog IS-A Animal
public class Animal 
{
    public string Name { get; set; }
    public virtual void Eat() { Console.WriteLine($"{Name} is eating"); }
}

public class Dog : Animal  // Dog IS-A Animal ✓
{
    public void Bark() { Console.WriteLine($"{Name} is barking"); }
}
```

**When to Use Composition (Has-a relationship):**
```csharp
// Good - Car HAS-A Engine (not IS-A Engine)
public class Engine 
{
    public void Start() { Console.WriteLine("Engine started"); }
    public void Stop() { Console.WriteLine("Engine stopped"); }
}

public class Car  // Car is NOT an Engine!
{
    private Engine engine = new Engine();  // Composition ✓
    
    public void Start() 
    {
        Console.WriteLine("Starting car...");
        engine.Start();  // Delegate to engine
    }
}

// Bad - Car inheriting from Engine makes no sense
// public class Car : Engine { }  // Car IS-A Engine? No! ✗
```

### Abstract Classes: Enforcing Common Interface

```csharp
// Abstract base class
public abstract class GameCharacter 
{
    public string Name { get; protected set; }
    public int Health { get; protected set; }
    public int Level { get; protected set; } = 1;
    
    public GameCharacter(string name, int health) 
    {
        Name = name;
        Health = health;
    }
    
    // Abstract methods - must be implemented by subclasses
    public abstract void Attack();
    public abstract void SpecialAbility();
    
    // Common implementation
    public void TakeDamage(int damage) 
    {
        Health -= damage;
        Console.WriteLine($"{Name} takes {damage} damage. Health: {Health}");
        if (Health <= 0) 
        {
            Console.WriteLine($"{Name} has been defeated!");
        }
    }
    
    public void LevelUp() 
    {
        Level++;
        Health += 20;
        Console.WriteLine($"{Name} leveled up to {Level}! Health increased.");
    }
}

public class Warrior : GameCharacter 
{
    public int Armor { get; private set; } = 10;
    
    public Warrior(string name) : base(name, 150)  // Warriors have more health
    {
    }
    
    {
        int damage = 25 + (Level * 2);
        Console.WriteLine($"{Name} swings sword for {damage} damage!");
        return damage;
    }
    
    public override void SpecialAbility() 
    {
        Console.WriteLine($"{Name} activates Shield Wall! Defense increased.");
        Armor += 5;
    }
}

public class Mage : GameCharacter 
{
    public int Mana { get; private set; } = 100;
    
    public Mage(string name) : base(name, 80)  // Mages have less health
    {
    }
    
    public override void Attack() 
    {
        if (Mana >= 10) 
        {
            Mana -= 10;
            int damage = 30 + (Level * 3);
            Console.WriteLine($"{Name} casts fireball for {damage} damage! Mana: {Mana}");
        } 
        else 
        {
            Console.WriteLine($"{Name} is out of mana! Basic attack for 5 damage.");
        }
    }
    
    public override void SpecialAbility() 
    {
        if (Mana >= 30) 
        {
            Mana -= 30;
            int damage = 50 + (Level * 5);
            Console.WriteLine($"{Name} casts Lightning Storm for {damage} damage!");
        } 
        else 
        {
            Console.WriteLine($"{Name} needs more mana for special ability!");
        }
    }
}

// Usage showing polymorphism through inheritance
public static void BattleSimulation(GameCharacter character1, GameCharacter character2) 
{
    Console.WriteLine($"\n🗡️ Battle between {character1.Name} and {character2.Name}!");
    
    while (character1.Health > 0 && character2.Health > 0) 
    {
        // Both characters attack (polymorphism in action)
        character1.Attack();
        character2.TakeDamage(10); // Simplified damage for demo
        
        if (character2.Health > 0) 
        {
            character2.Attack();
            character1.TakeDamage(10);
        }
    }
}

var warrior = new Warrior("Conan");
var mage = new Mage("Gandalf");

battle_simulation(warrior, mage)  # Works with any GameCharacter subclass
```

## Exercises

1. **Easy**: Create a `Shape` hierarchy with `Circle`, `Rectangle`, and `Triangle` classes that inherit common properties.
   ```csharp
   public abstract class Shape
   {
       protected string color;
       
       public Shape(string color)
       {
           this.color = color;
       }
       
       public abstract double CalculateArea();  // Override in subclasses
       
       public virtual string Describe()
       {
           return $"A {color} shape";
       }
   }
   
   public class Circle : Shape
   {
       private double radius;
       
       public Circle(string color, double radius) : base(color)
       {
           this.radius = radius;
       }
       
       // Implement the abstract method
       public override double CalculateArea()
       {
           // Your code here
           return 0;
       }
   }
   ```

2. **Medium**: Design an `Employee` hierarchy with `Manager`, `Developer`, and `Intern` classes, each with specific behaviors.
   ```csharp
   public abstract class Employee 
   {
       protected string name;
       protected double baseSalary;
       
       public abstract double CalculateSalary();
       public abstract void DoWork();
       
       public virtual void AttendMeeting() 
       {
           Console.WriteLine($"{name} attends meeting");
       }
   }
   
   public class Manager : Employee 
   {
       private List<Employee> team;
       // Implement CalculateSalary and DoWork
       // Add manager-specific methods
   }
   ```

3. **Hard**: Create a game character system with base `Character` class and specialized classes like `Warrior`, `Mage`, and `Archer`.
   ```csharp
   public abstract class Character 
   {
       protected string name;
       protected int health, mana, level;
       
       public Character(string name, int health, int mana) 
       {
           this.name = name;
           this.health = health;
           this.mana = mana;
           this.level = 1;
       }
       
       public abstract int Attack();  // Pure virtual - must be implemented
       public abstract void UseSpecialAbility();  // Pure virtual
       
       public virtual void TakeDamage(int damage) 
       {
           health -= damage;
           // Your implementation here
       }
       
       public string GetName() => name;
       public int GetHealth() => health;
       public int GetLevel() => level;
   }
   ```

## Chapter Checklist

After completing this chapter, you should be able to:

- [ ] Understand when to use inheritance vs. composition
- [ ] Identify proper "is-a" relationships for inheritance
- [ ] Create base classes with common functionality
- [ ] Implement derived classes that extend base functionality
- [ ] Use method overriding to customize behavior
- [ ] Understand the difference between overriding and overloading
- [ ] Avoid deep inheritance hierarchies
- [ ] Recognize when inheritance might not be the best solution
- [ ] Apply inheritance while maintaining good design principles