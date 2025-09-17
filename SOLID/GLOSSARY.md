# Glossary

## Object-Oriented Programming Terms

**Abstraction**
- Hiding implementation details while exposing only essential features
- Allows users to interact with objects without knowing how they work internally
- Example: Using a car without knowing how the engine works

**Class**
- A blueprint or template for creating objects
- Defines what properties and methods objects of that type will have
- Example: A "Car" class defines that all cars have properties like color and methods like start()

**Encapsulation**
- Bundling data (attributes) and methods that operate on that data within a single unit
- Restricting direct access to some of an object's components
- Example: Making bank account balance private and only allowing access through specific methods

**Inheritance**
- Mechanism allowing a new class to acquire properties and methods of an existing class
- Promotes code reuse and establishes relationships between classes
- Example: A "Dog" class inheriting from an "Animal" class

**Method**
- A function that belongs to a class and defines behavior of objects
- Can perform actions and return values
- Example: A "bark()" method in a Dog class

**Object**
- An instance of a class
- Contains actual data and can perform actions defined by its class
- Example: "myDog" is an object created from the Dog class

**Polymorphism**
- Ability to treat objects of different types uniformly through a common interface
- Same method name can behave differently based on the object calling it
- Example: Different animals implementing "makeSound()" differently

**Property/Attribute**
- Data stored within an object
- Represents the state or characteristics of an object
- Example: A car's color, speed, or fuel level

## SOLID Principles

**Single Responsibility Principle (SRP)**
- A class should have only one reason to change
- Each class should have only one job or responsibility
- Promotes focused, cohesive classes

**Open-Closed Principle (OCP)**
- Software entities should be open for extension but closed for modification
- You should be able to add new functionality without changing existing code
- Achieved through abstraction and polymorphism

**Liskov Substitution Principle (LSP)**
- Objects of a superclass should be replaceable with objects of a subclass without breaking the application
- Derived classes must be substitutable for their base classes
- Ensures that inheritance hierarchies are well-designed

**Interface Segregation Principle (ISP)**
- Clients should not be forced to depend on interfaces they don't use
- Many specific interfaces are better than one general-purpose interface
- Prevents "fat" interfaces that force implementations of unused methods

**Dependency Inversion Principle (DIP)**
- High-level modules should not depend on low-level modules; both should depend on abstractions
- Abstractions should not depend on details; details should depend on abstractions
- Promotes loose coupling between components

## Design Patterns

**Observer Pattern**
- Defines a one-to-many dependency between objects
- When one object changes state, all dependent objects are notified automatically
- Example: Newsletter subscription system

**Strategy Pattern**
- Defines a family of algorithms and makes them interchangeable
- Allows algorithm selection at runtime
- Example: Different payment methods in an e-commerce system

**Factory Pattern**
- Creates objects without specifying their exact classes
- Provides an interface for creating families of related objects
- Example: Creating different types of database connections

**Decorator Pattern**
- Adds new functionality to objects without altering their structure
- Provides a flexible alternative to subclassing
- Example: Adding features to a basic text editor

## Clean Code Principles

**DRY (Don't Repeat Yourself)**
- Avoid code duplication
- Each piece of knowledge should have a single, authoritative representation
- Promotes maintainability and reduces errors

**KISS (Keep It Simple, Stupid)**
- Prefer simple solutions over complex ones
- Avoid unnecessary complexity
- Simple code is easier to understand, test, and maintain

**YAGNI (You Aren't Gonna Need It)**
- Don't implement functionality until you actually need it
- Avoid building features based on speculation
- Prevents over-engineering and wasted effort

## Testing Terms

**Test-Driven Development (TDD)**
- Development approach where tests are written before the actual code
- Follows Red-Green-Refactor cycle
- Ensures code meets requirements and is testable

**Unit Test**
- Tests individual components or methods in isolation
- Verifies that each part works correctly on its own
- Foundation of automated testing strategies

**Refactoring**
- Process of improving code structure without changing its external behavior
- Makes code more readable, maintainable, and efficient
- Should be done incrementally with test coverage

## General Programming Terms

**Interface**
- Contract that defines what methods a class must implement
- Provides a way to achieve abstraction and multiple inheritance of type
- Example: IDrawable interface requiring a Draw() method

**Abstract Class**
- Class that cannot be instantiated directly
- May contain both implemented and abstract methods
- Used as a base class for other classes

**Composition**
- Design principle where classes contain instances of other classes
- Alternative to inheritance for code reuse
- Example: A car "has-a" engine rather than "is-a" engine

**Dependency Injection**
- Technique for providing dependencies to an object rather than having it create them
- Promotes loose coupling and testability
- Common implementation of Dependency Inversion Principle