// FILE: chapter-01-basic.cpp
// Chapter 1 — What is OOP? — Basic

#include <iostream>
#include <string>

// A simple class representing a Dog
class Dog {
public:
    // Properties (data/state)
    std::string name;
    int age;
    
    // Method (behavior)
    void bark() {
        std::cout << name << " says: Woof!" << std::endl;
    }
};

int main() {
    // Creating objects (instances) from our class
    Dog myDog;
    myDog.name = "Buddy";
    myDog.age = 3;
    
    Dog friendDog;
    friendDog.name = "Max";
    friendDog.age = 5;
    
    // Using the objects
    std::cout << "My dog " << myDog.name << " is " << myDog.age << " years old" << std::endl;
    myDog.bark();
    
    std::cout << "Friend's dog " << friendDog.name << " is " << friendDog.age << " years old" << std::endl;
    friendDog.bark();
    
    return 0;
}