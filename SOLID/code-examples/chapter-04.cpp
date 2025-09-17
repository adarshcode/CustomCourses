// FILE: chapter-04.cpp
// Chapter 4 — Inheritance — C++

#include <iostream>
#include <string>
using namespace std;

// Base class (parent)
class Animal {
protected:
    string name;
    
public:
    Animal(const string& name) : name(name) {}
    
    void eat() {
        cout << name << " is eating" << endl;
    }
    
    virtual void makeSound() {
        cout << name << " makes a sound" << endl;
    }
    
    virtual ~Animal() = default; // Virtual destructor
};

// Derived class (child)
class Dog : public Animal {
public:
    Dog(const string& name) : Animal(name) {}
    
    // Override parent method
    void makeSound() override {
        cout << name << " says: Woof!" << endl;
    }
    
    // Dog-specific behavior
    void wagTail() {
        cout << name << " wags tail" << endl;
    }
};

int main() {
    cout << "🐕 Basic Inheritance Example (C++)" << endl;
    cout << "===================================" << endl << endl;
    
    // Create a dog
    Dog myDog("Buddy");
    
    // Use inherited method
    myDog.eat();        // From Animal class
    
    // Use overridden method
    myDog.makeSound();  // Dog's version
    
    // Use dog-specific method
    myDog.wagTail();    // Only dogs can do this
    
    cout << endl << "💡 Inheritance Benefits:" << endl;
    cout << "   ✓ Code reuse: Dog inherits eat() from Animal" << endl;
    cout << "   ✓ Customization: Dog overrides makeSound()" << endl;
    cout << "   ✓ Extension: Dog adds wagTail() behavior" << endl;
    
    return 0;
}