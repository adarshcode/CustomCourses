# FILE: chapter-07.py
# Chapter 7 — Open-Closed Principle — Python

from abc import ABC, abstractmethod
import math

# ❌ BAD: Violates OCP - must modify class to add new shapes
class BadShapeCalculator:
    def calculate_area(self, shape_type, param1, param2=0):
        if shape_type == "circle":
            return math.pi * param1 ** 2
        elif shape_type == "rectangle":
            return param1 * param2
        # To add triangle, we'd have to modify this class!
        return 0

# ✅ GOOD: Follows OCP - open for extension, closed for modification
class Shape(ABC):
    @abstractmethod
    def calculate_area(self):
        pass

class Circle(Shape):
    def __init__(self, radius):
        self.radius = radius
    
    def calculate_area(self):
        return math.pi * self.radius ** 2

class Rectangle(Shape):
    def __init__(self, width, height):
        self.width = width
        self.height = height
    
    def calculate_area(self):
        return self.width * self.height

# NEW: Can add triangle without modifying existing code!
class Triangle(Shape):
    def __init__(self, base, height):
        self.base = base
        self.height = height
    
    def calculate_area(self):
        return 0.5 * self.base * self.height

class AreaCalculator:
    def calculate_total_area(self, shapes):
        return sum(shape.calculate_area() for shape in shapes)

def main():
    print("🔓 Open-Closed Principle (Python)")
    print("==================================\n")
    
    print("❌ Bad example (violates OCP):")
    bad_calc = BadShapeCalculator()
    print(f"Circle area: {bad_calc.calculate_area('circle', 5):.2f}")
    print(f"Rectangle area: {bad_calc.calculate_area('rectangle', 4, 6):.2f}")
    
    print("\n✅ Good example (follows OCP):")
    shapes = [
        Circle(5),
        Rectangle(4, 6),
        Triangle(3, 8)  # New shape added easily!
    ]
    
    calculator = AreaCalculator()
    total_area = calculator.calculate_total_area(shapes)
    print(f"Total area: {total_area:.2f}")
    
    print("\n💡 OCP Benefits:")
    print("   ✓ Can add new shapes without modifying existing code")
    print("   ✓ Existing code remains stable and tested")
    print("   ✓ Easy to extend functionality")

if __name__ == "__main__":
    main()