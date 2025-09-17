# FILE: chapter-05.py
# Chapter 5 — Polymorphism — Python

from abc import ABC, abstractmethod
import math

# Base class with abstract methods
class Shape(ABC):
    @abstractmethod
    def get_area(self):
        pass
    
    @abstractmethod
    def draw(self):
        pass

# Derived classes
class Circle(Shape):
    def __init__(self, radius):
        self.radius = radius
    
    def get_area(self):
        return math.pi * self.radius ** 2
    
    def draw(self):
        print(f"Drawing a circle with radius {self.radius}")

class Rectangle(Shape):
    def __init__(self, width, height):
        self.width = width
        self.height = height
    
    def get_area(self):
        return self.width * self.height
    
    def draw(self):
        print(f"Drawing a rectangle {self.width}x{self.height}")

def main():
    print("🔄 Polymorphism Example (Python)")
    print("=================================\n")
    
    # Create different shapes
    shapes = [
        Circle(5.0),
        Rectangle(4.0, 6.0),
        Circle(3.0)
    ]
    
    # Polymorphism in action - same interface, different behavior
    for shape in shapes:
        shape.draw()                           # Calls appropriate draw method
        print(f"Area: {shape.get_area():.2f}\n")  # Calls appropriate get_area method
    
    print("💡 Polymorphism Benefits:")
    print("   ✓ Same interface for different objects")
    print("   ✓ Runtime method resolution")
    print("   ✓ Extensible - can add new shapes easily")

if __name__ == "__main__":
    main()