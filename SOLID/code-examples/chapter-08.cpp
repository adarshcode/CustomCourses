// FILE: chapter-08.cpp
// Chapter 8 — Liskov Substitution Principle — C++

#include <iostream>
#include <vector>
#include <memory>
#include <stdexcept>
using namespace std;

// ❌ BAD: Violates LSP - Square changes Rectangle behavior
class BadRectangle {
protected:
    double width, height;
public:
    BadRectangle(double w, double h) : width(w), height(h) {}
    
    virtual void setWidth(double w) { width = w; }
    virtual void setHeight(double h) { height = h; }
    
    double getWidth() const { return width; }
    double getHeight() const { return height; }
    double getArea() const { return width * height; }
};

class BadSquare : public BadRectangle {
public:
    BadSquare(double side) : BadRectangle(side, side) {}
    
    void setWidth(double w) override {
        width = height = w;  // Changes both dimensions!
    }
    
    void setHeight(double h) override {
        width = height = h;  // Changes both dimensions!
    }
};

// ✅ GOOD: Follows LSP - proper abstraction
class Shape {
public:
    virtual double getArea() const = 0;
    virtual void scale(double factor) = 0;
    virtual ~Shape() = default;
};

class Rectangle : public Shape {
protected:
    double width, height;
public:
    Rectangle(double w, double h) : width(w), height(h) {}
    
    double getArea() const override {
        return width * height;
    }
    
    void scale(double factor) override {
        width *= factor;
        height *= factor;
    }
    
    double getWidth() const { return width; }
    double getHeight() const { return height; }
};

class Square : public Shape {
private:
    double side;
public:
    Square(double s) : side(s) {}
    
    double getArea() const override {
        return side * side;
    }
    
    void scale(double factor) override {
        side *= factor;
    }
    
    double getSide() const { return side; }
};

// Function that works with any Shape (LSP compliant)
void processShapes(const vector<unique_ptr<Shape>>& shapes) {
    for (const auto& shape : shapes) {
        cout << "Shape area: " << shape->getArea() << endl;
        // Can safely call any Shape method on any derived class
    }
}

int main() {
    cout << "🔄 Liskov Substitution Principle (C++)" << endl;
    cout << "=======================================" << endl << endl;
    
    cout << "❌ Bad example (violates LSP):" << endl;
    BadRectangle* rect = new BadSquare(5);
    rect->setWidth(10);
    rect->setHeight(8);
    cout << "Expected area: 80, Actual area: " << rect->getArea() << endl;
    delete rect;
    
    cout << endl << "✅ Good example (follows LSP):" << endl;
    vector<unique_ptr<Shape>> shapes;
    shapes.push_back(make_unique<Rectangle>(4, 6));
    shapes.push_back(make_unique<Square>(5));
    
    processShapes(shapes);
    
    cout << endl << "💡 LSP Benefits:" << endl;
    cout << "   ✓ Derived classes are truly substitutable" << endl;
    cout << "   ✓ No unexpected behavior changes" << endl;
    cout << "   ✓ Polymorphism works correctly" << endl;
    
    return 0;
}