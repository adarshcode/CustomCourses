# FILE: chapter-04.py
# Chapter 4 — Inheritance — Python

# Base class (parent)
class Animal:
    def __init__(self, name):
        self.name = name
    
    def eat(self):
        print(f"{self.name} is eating")
    
    def make_sound(self):
        print(f"{self.name} makes a sound")

# Derived class (child)
class Dog(Animal):
    def __init__(self, name):
        super().__init__(name)  # Call parent constructor
    
    # Override parent method
    def make_sound(self):
        print(f"{self.name} says: Woof!")
    
    # Dog-specific behavior
    def wag_tail(self):
        print(f"{self.name} wags tail")

def main():
    print("🐕 Basic Inheritance Example (Python)")
    print("=====================================\n")
    
    # Create a dog
    my_dog = Dog("Buddy")
    
    # Use inherited method
    my_dog.eat()        # From Animal class
    
    # Use overridden method
    my_dog.make_sound() # Dog's version
    
    # Use dog-specific method
    my_dog.wag_tail()   # Only dogs can do this
    
    print("\n💡 Inheritance Benefits:")
    print("   ✓ Code reuse: Dog inherits eat() from Animal")
    print("   ✓ Customization: Dog overrides make_sound()")
    print("   ✓ Extension: Dog adds wag_tail() behavior")

if __name__ == "__main__":
    main()