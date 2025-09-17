# Chapter 4: Inheritance - Building on Existing Code

## Child-Friendly Explanation
Imagine you have a bicycle, and someone invents a motorcycle. Instead of building the motorcycle completely from scratch, they can start with the bicycle (it already has wheels, handlebars, and pedals) and then add an engine, remove the pedals, and make other changes. Inheritance in programming is like this - you can create new things by starting with something that already exists and adding or changing just the parts you need!

```python
# Start with a basic Animal
class Animal:
    def __init__(self, name):
        self.name = name
    
    def eat(self):
        print(f"{self.name} is eating")
    
    def sleep(self):
        print(f"{self.name} is sleeping")

# Create a Dog by inheriting from Animal
class Dog(Animal):  # Dog gets everything Animal has!
    def bark(self):  # Plus new abilities
        print(f"{self.name} says: Woof!")

# Create a Cat by inheriting from Animal  
class Cat(Animal):  # Cat also gets everything Animal has!
    def meow(self):  # Plus different new abilities
        print(f"{self.name} says: Meow!")

# Both work because they inherit from Animal
buddy = Dog("Buddy")
whiskers = Cat("Whiskers")

# They can do Animal things
buddy.eat()     # Buddy is eating
whiskers.sleep() # Whiskers is sleeping

# Plus their own special things
buddy.bark()    # Buddy says: Woof!
whiskers.meow() # Whiskers says: Meow!
```

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

```cpp
// Base class with common functionality
class Employee {
protected:
    string name;
    int id;
    double baseSalary;
    
public:
    Employee(const string& name, int id, double baseSalary) 
        : name(name), id(id), baseSalary(baseSalary) {}
    
    virtual double calculateSalary() {
        return baseSalary;
    }
    
    virtual void displayInfo() {
        cout << "Employee: " << name << " (ID: " << id << ")" << endl;
    }
    
    virtual ~Employee() = default;  // Important for polymorphism
};

// Derived class with additional functionality
class Manager : public Employee {
private:
    double bonus;
    vector<Employee*> subordinates;
    
public:
    Manager(const string& name, int id, double baseSalary, double bonus) 
        : Employee(name, id, baseSalary), bonus(bonus) {}
    
    // Override to include bonus
    double calculateSalary() override {
        return baseSalary + bonus;
    }
    
    // Add manager-specific functionality
    void addSubordinate(Employee* employee) {
        subordinates.push_back(employee);
    }
    
    void displayInfo() override {
        Employee::displayInfo();  // Call parent method
        cout << "Position: Manager" << endl;
        cout << "Manages " << subordinates.size() << " employees" << endl;
        cout << "Total Salary: $" << calculateSalary() << endl;
    }
};

// Another derived class with different specialization
class Developer : public Employee {
private:
    string programmingLanguage;
    int projectsCompleted;
    
public:
    Developer(const string& name, int id, double baseSalary, const string& language) 
        : Employee(name, id, baseSalary), programmingLanguage(language), projectsCompleted(0) {}
    
    // Override with performance-based calculation
    double calculateSalary() override {
        double performanceBonus = projectsCompleted * 1000;  // $1000 per project
        return baseSalary + performanceBonus;
    }
    
    void completeProject() {
        projectsCompleted++;
        cout << name << " completed a project in " << programmingLanguage << "!" << endl;
    }
    
    void displayInfo() override {
        Employee::displayInfo();
        cout << "Position: Developer" << endl;
        cout << "Language: " << programmingLanguage << endl;
        cout << "Projects: " << projectsCompleted << endl;
        cout << "Total Salary: $" << calculateSalary() << endl;
    }
};
```

### Constructor Chaining: Proper Initialization

```python
class Vehicle:
    def __init__(self, make, model, year):
        self.make = make
        self.model = model
        self.year = year
        self.is_running = False
        print(f"Vehicle created: {year} {make} {model}")
    
    def start(self):
        if not self.is_running:
            self.is_running = True
            print(f"{self.make} {self.model} started")
        else:
            print("Vehicle is already running")
    
    def stop(self):
        if self.is_running:
            self.is_running = False
            print(f"{self.make} {self.model} stopped")
        else:
            print("Vehicle is already stopped")
    
    def get_info(self):
        return f"{self.year} {self.make} {self.model}"

class Car(Vehicle):
    def __init__(self, make, model, year, doors, fuel_type="gasoline"):
        super().__init__(make, model, year)  # Call parent constructor
        self.doors = doors
        self.fuel_type = fuel_type
        self.trunk_open = False
        print(f"Car initialized with {doors} doors and {fuel_type} fuel")
    
    def open_trunk(self):
        self.trunk_open = True
        print(f"Trunk opened on {self.get_info()}")
    
    def honk(self):
        print(f"{self.make} {self.model}: BEEP BEEP!")
    
    # Override parent method with car-specific behavior
    def start(self):
        print("🔑 Turning key in ignition...")
        super().start()  # Call parent's start method
        if self.is_running:
            print("🚗 Ready to drive!")

class Motorcycle(Vehicle):
    def __init__(self, make, model, year, engine_size):
        super().__init__(make, model, year)  # Call parent constructor
        self.engine_size = engine_size
        self.kickstand_down = True
        print(f"Motorcycle initialized with {engine_size}cc engine")
    
    def kick_start(self):
        if self.kickstand_down:
            print("⚠️ Put kickstand up before starting!")
            return
        print("🦵 Kick starting motorcycle...")
        super().start()
        if self.is_running:
            print("🏍️ Ready to ride!")
    
    def put_kickstand_up(self):
        self.kickstand_down = False
        print("Kickstand up")
    
    # Override with motorcycle-specific behavior
    def start(self):
        if self.kickstand_down:
            print("⚠️ Cannot start with kickstand down!")
            return
        print("🔑 Pressing start button...")
        super().start()
        if self.is_running:
            print("🏍️ Motorcycle ready!")

# Usage showing inheritance in action
car = Car("Toyota", "Camry", 2023, 4, "hybrid")
motorcycle = Motorcycle("Harley-Davidson", "Sportster", 2023, 883)

print("\n--- Car Operations ---")
car.start()
car.honk()
car.open_trunk()

print("\n--- Motorcycle Operations ---")
motorcycle.start()  # Will fail - kickstand down
motorcycle.put_kickstand_up()
motorcycle.start()  # Now works
```

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

```python
from abc import ABC, abstractmethod

class GameCharacter(ABC):  # Abstract base class
    def __init__(self, name, health):
        self.name = name
        self.health = health
        self.level = 1
    
    @abstractmethod
    def attack(self):  # Must be implemented by subclasses
        pass
    
    @abstractmethod  
    def special_ability(self):  # Must be implemented by subclasses
        pass
    
    def take_damage(self, damage):  # Common implementation
        self.health -= damage
        print(f"{self.name} takes {damage} damage. Health: {self.health}")
        if self.health <= 0:
            print(f"{self.name} has been defeated!")
    
    def level_up(self):  # Common implementation
        self.level += 1
        self.health += 20
        print(f"{self.name} leveled up to {self.level}! Health increased.")

class Warrior(GameCharacter):
    def __init__(self, name):
        super().__init__(name, 150)  # Warriors have more health
        self.armor = 10
    
    def attack(self):
        damage = 25 + (self.level * 2)
        print(f"{self.name} swings sword for {damage} damage!")
        return damage
    
    def special_ability(self):
        print(f"{self.name} activates Shield Wall! Defense increased.")
        self.armor += 5

class Mage(GameCharacter):
    def __init__(self, name):
        super().__init__(name, 80)  # Mages have less health
        self.mana = 100
    
    def attack(self):
        if self.mana >= 10:
            self.mana -= 10
            damage = 30 + (self.level * 3)
            print(f"{self.name} casts fireball for {damage} damage! Mana: {self.mana}")
            return damage
        else:
            print(f"{self.name} is out of mana! Basic attack for 5 damage.")
            return 5
    
    def special_ability(self):
        if self.mana >= 30:
            self.mana -= 30
            damage = 50 + (self.level * 5)
            print(f"{self.name} casts Lightning Storm for {damage} damage!")
            return damage
        else:
            print(f"{self.name} needs more mana for special ability!")
            return 0

# Usage showing polymorphism through inheritance
def battle_simulation(character1: GameCharacter, character2: GameCharacter):
    print(f"\n🗡️ Battle between {character1.name} and {character2.name}!")
    
    while character1.health > 0 and character2.health > 0:
        # Both characters attack (polymorphism in action)
        damage1 = character1.attack()
        character2.take_damage(damage1)
        
        if character2.health > 0:
            damage2 = character2.attack()
            character1.take_damage(damage2)

warrior = Warrior("Conan")
mage = Mage("Gandalf")

battle_simulation(warrior, mage)  # Works with any GameCharacter subclass
```

## Exercises

1. **Easy**: Create a `Shape` hierarchy with `Circle`, `Rectangle`, and `Triangle` classes that inherit common properties.
   ```python
   class Shape:
       def __init__(self, color):
           self.color = color
       
       def calculate_area(self):  # Override in subclasses
           pass
       
       def describe(self):
           return f"A {self.color} shape"
   
   class Circle(Shape):
       def __init__(self, color, radius):
           super().__init__(color)
           self.radius = radius
       
       # Implement the abstract method
       def calculate_area(self):
           # Your code here
           pass
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
   ```cpp
   class Character {
   protected:
       string name;
       int health, mana, level;
       
   public:
       Character(string n, int h, int m) : name(n), health(h), mana(m), level(1) {}
       virtual ~Character() = default;
       
       virtual int attack() = 0;  // Pure virtual
       virtual void useSpecialAbility() = 0;  // Pure virtual
       
       void takeDamage(int damage) {
           health -= damage;
           // Your implementation here
       }
   };
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