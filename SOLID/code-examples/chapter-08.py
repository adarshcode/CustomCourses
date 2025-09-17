# FILE: chapter-08.py
# Chapter 8 — Liskov Substitution Principle — Python

from abc import ABC, abstractmethod

# ❌ BAD: Violates LSP - Square changes Rectangle behavior
class BadRectangle:
    def __init__(self, width, height):
        self._width = width
        self._height = height
    
    def set_width(self, width):
        self._width = width
    
    def set_height(self, height):
        self._height = height
    
    def get_width(self):
        return self._width
    
    def get_height(self):
        return self._height
    
    def get_area(self):
        return self._width * self._height

class BadSquare(BadRectangle):
    def __init__(self, side):
        super().__init__(side, side)
    
    def set_width(self, width):
        self._width = self._height = width  # Changes both dimensions!
    
    def set_height(self, height):
        self._width = self._height = height  # Changes both dimensions!

# ✅ GOOD: Follows LSP - proper abstraction
class Shape(ABC):
    @abstractmethod
    def get_area(self):
        pass
    
    @abstractmethod
    def scale(self, factor):
        pass

class Rectangle(Shape):
    def __init__(self, width, height):
        self._width = width
        self._height = height
    
    def get_area(self):
        return self._width * self._height
    
    def scale(self, factor):
        self._width *= factor
        self._height *= factor
    
    def get_width(self):
        return self._width
    
    def get_height(self):
        return self._height

class Square(Shape):
    def __init__(self, side):
        self._side = side
    
    def get_area(self):
        return self._side ** 2
    
    def scale(self, factor):
        self._side *= factor
    
    def get_side(self):
        return self._side

# Function that works with any Shape (LSP compliant)
def process_shapes(shapes):
    for shape in shapes:
        print(f"Shape area: {shape.get_area()}")
        # Can safely call any Shape method on any derived class

def main():
    print("🔄 Liskov Substitution Principle (Python)")
    print("==========================================\n")
    
    print("❌ Bad example (violates LSP):")
    rect = BadSquare(5)  # Treated as BadRectangle
    rect.set_width(10)
    rect.set_height(8)
    print(f"Expected area: 80, Actual area: {rect.get_area()}")
    
    print("\n✅ Good example (follows LSP):")
    shapes = [
        Rectangle(4, 6),
        Square(5)
    ]
    
    process_shapes(shapes)
    
    print("\n💡 LSP Benefits:")
    print("   ✓ Derived classes are truly substitutable")
    print("   ✓ No unexpected behavior changes")
    print("   ✓ Polymorphism works correctly")

if __name__ == "__main__":
    main()