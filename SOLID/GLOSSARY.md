# Glossary

## Technical Terms Used Throughout the Course

**Abstract Class** - A class that cannot be instantiated directly and typically contains one or more abstract methods that must be implemented by subclasses.

**Abstraction** - The principle of hiding complex implementation details while exposing only the necessary interface to the user.

**Aggregation** - A "has-a" relationship where one object contains references to other objects, but the contained objects can exist independently.

**Class** - A blueprint or template that defines the structure and behavior of objects, including their properties and methods.

**Cohesion** - The degree to which elements within a single class or module work together toward a single, well-defined purpose.

**Composition** - A strong "has-a" relationship where one object owns other objects and the contained objects cannot exist without the container.

**Constructor** - A special method that is called when an object is created, typically used to initialize the object's state.

**Coupling** - The degree of interdependence between different classes or modules; lower coupling is generally better.

**Dependency Injection** - A technique where dependencies are provided to an object rather than the object creating them itself.

**Dependency Inversion Principle (DIP)** - High-level modules should not depend on low-level modules; both should depend on abstractions.

**DRY (Don't Repeat Yourself)** - A principle that aims to reduce repetition of software patterns by extracting common functionality.

**Encapsulation** - The bundling of data and methods that operate on that data within a single unit, while restricting direct access to some components.

**Inheritance** - A mechanism where a new class is based on an existing class, inheriting its properties and methods.

**Instance** - A specific occurrence of a class; an object created from a class blueprint.

**Interface** - A contract that defines a set of method signatures that implementing classes must provide.

**Interface Segregation Principle (ISP)** - Clients should not be forced to depend on interfaces they do not use.

**KISS (Keep It Simple, Stupid)** - A design principle that states systems work best when they are kept simple rather than made complex.

**Liskov Substitution Principle (LSP)** - Objects of a superclass should be replaceable with objects of its subclasses without breaking the application.

**Method** - A function that belongs to a class and defines behavior that objects of that class can perform.

**Method Overloading** - Defining multiple methods with the same name but different parameters (compile-time polymorphism).

**Method Overriding** - Redefining a parent class method in a child class with the same signature (runtime polymorphism).

**Object** - An instance of a class that contains both data (attributes) and code (methods) that manipulates that data.

**Object-Oriented Programming (OOP)** - A programming paradigm based on the concept of objects, which contain data and code.

**Open/Closed Principle (OCP)** - Software entities should be open for extension but closed for modification.

**Polymorphism** - The ability of different classes to be treated as instances of the same type through a common interface.

**Property** - A member of a class that provides a controlled way to access private fields, often through getter and setter methods.

**Refactoring** - The process of restructuring existing code without changing its external behavior to improve readability and reduce complexity.

**Single Responsibility Principle (SRP)** - A class should have only one reason to change, meaning it should have only one job or responsibility.

**SOLID** - An acronym for five design principles: Single Responsibility, Open/Closed, Liskov Substitution, Interface Segregation, and Dependency Inversion.

**State** - The current values of all attributes/properties of an object at any given time.

**TDD (Test-Driven Development)** - A development approach where tests are written before the actual code, driving the design.

**UML (Unified Modeling Language)** - A standardized modeling language used to visualize the design of object-oriented systems.

**YAGNI (You Aren't Gonna Need It)** - A principle that suggests not adding functionality until it is necessary.

## Common Design Patterns

**Factory Pattern** - A creational pattern that provides an interface for creating objects without specifying their exact classes.

**Observer Pattern** - A behavioral pattern where an object maintains a list of dependents and notifies them of state changes.

**Singleton Pattern** - A creational pattern that ensures a class has only one instance and provides global access to it.

**Strategy Pattern** - A behavioral pattern that defines a family of algorithms and makes them interchangeable.

**Adapter Pattern** - A structural pattern that allows incompatible interfaces to work together.

**Decorator Pattern** - A structural pattern that allows behavior to be added to objects dynamically without altering their structure.

## Access Modifiers

**Private** - Members that can only be accessed within the same class.

**Protected** - Members that can be accessed within the same class and its subclasses.

**Public** - Members that can be accessed from anywhere in the program.

**Internal/Package** - Members that can be accessed within the same package or assembly (language-specific).

---

*This glossary covers the essential terminology used throughout the course. Each term is explained in simple language to aid understanding for beginners while remaining accurate for more advanced learners.*