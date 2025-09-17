// FILE: chapter-07.cpp
// Chapter 7 — Open-Closed Principle — C++

#include <iostream>
#include <vector>
#include <memory>
using namespace std;

// ❌ BAD: Violates OCP - must modify class to add new shapes
class BadShapeCalculator {
public:
    double calculateArea(const string& shapeType, double param1, double param2 = 0) {
        if (shapeType == "circle") {
            return 3.14159 * param1 * param1;
        } else if (shapeType == "rectangle") {
            return param1 * param2;
        }
        // To add triangle, we'd have to modify this class!
        return 0;
    }
};

// ✅ GOOD: Follows OCP - open for extension, closed for modification
class Shape {
public:
    virtual double calculateArea() = 0;
    virtual ~Shape() = default;
};

class Circle : public Shape {
private:
    double radius;
public:
    Circle(double r) : radius(r) {}
    double calculateArea() override {
        return 3.14159 * radius * radius;
    }
};

class Rectangle : public Shape {
private:
    double width, height;
public:
    Rectangle(double w, double h) : width(w), height(h) {}
    double calculateArea() override {
        return width * height;
    }
};

// NEW: Can add triangle without modifying existing code!
class Triangle : public Shape {
private:
    double base, height;
public:
    Triangle(double b, double h) : base(b), height(h) {}
    double calculateArea() override {
        return 0.5 * base * height;
    }
};

class AreaCalculator {
public:
    double calculateTotalArea(const vector<unique_ptr<Shape>>& shapes) {
        double total = 0;
        for (const auto& shape : shapes) {
            total += shape->calculateArea();
        }
        return total;
    }
};

int main() {
    cout << "🔓 Open-Closed Principle (C++)" << endl;
    cout << "===============================" << endl << endl;
    
    cout << "❌ Bad example (violates OCP):" << endl;
    BadShapeCalculator badCalc;
    cout << "Circle area: " << badCalc.calculateArea("circle", 5) << endl;
    cout << "Rectangle area: " << badCalc.calculateArea("rectangle", 4, 6) << endl;
    
    cout << endl << "✅ Good example (follows OCP):" << endl;
    vector<unique_ptr<Shape>> shapes;
    shapes.push_back(make_unique<Circle>(5));
    shapes.push_back(make_unique<Rectangle>(4, 6));
    shapes.push_back(make_unique<Triangle>(3, 8));  // New shape added easily!
    
    AreaCalculator calculator;
    cout << "Total area: " << calculator.calculateTotalArea(shapes) << endl;
    
    cout << endl << "💡 OCP Benefits:" << endl;
    cout << "   ✓ Can add new shapes without modifying existing code" << endl;
    cout << "   ✓ Existing code remains stable and tested" << endl;
    cout << "   ✓ Easy to extend functionality" << endl;
    
    return 0;
}