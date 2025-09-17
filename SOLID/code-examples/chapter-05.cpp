// FILE: chapter-05.cpp
// Chapter 5 — Polymorphism — C++

#include <iostream>
#include <vector>
#include <memory>
using namespace std;

// Base class with virtual methods
class Shape {
public:
    virtual double getArea() = 0;  // Pure virtual method
    virtual void draw() = 0;       // Pure virtual method
    virtual ~Shape() = default;    // Virtual destructor
};

// Derived classes
class Circle : public Shape {
private:
    double radius;
    
public:
    Circle(double r) : radius(r) {}
    
    double getArea() override {
        return 3.14159 * radius * radius;
    }
    
    void draw() override {
        cout << "Drawing a circle with radius " << radius << endl;
    }
};

class Rectangle : public Shape {
private:
    double width, height;
    
public:
    Rectangle(double w, double h) : width(w), height(h) {}
    
    double getArea() override {
        return width * height;
    }
    
    void draw() override {
        cout << "Drawing a rectangle " << width << "x" << height << endl;
    }
};

int main() {
    cout << "🔄 Polymorphism Example (C++)" << endl;
    cout << "=============================" << endl << endl;
    
    // Create different shapes
    vector<unique_ptr<Shape>> shapes;
    shapes.push_back(make_unique<Circle>(5.0));
    shapes.push_back(make_unique<Rectangle>(4.0, 6.0));
    shapes.push_back(make_unique<Circle>(3.0));
    
    // Polymorphism in action - same interface, different behavior
    for (const auto& shape : shapes) {
        shape->draw();                    // Calls appropriate draw method
        cout << "Area: " << shape->getArea() << endl << endl;  // Calls appropriate getArea method
    }
    
    cout << "💡 Polymorphism Benefits:" << endl;
    cout << "   ✓ Same interface for different objects" << endl;
    cout << "   ✓ Runtime method resolution" << endl;
    cout << "   ✓ Extensible - can add new shapes easily" << endl;
    
    return 0;
}