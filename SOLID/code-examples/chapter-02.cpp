// FILE: chapter-02-basic.cpp
// Chapter 2 — Encapsulation — Basic

#include <iostream>

class Temperature {
private:
    // Private member - can only be accessed within this class
    double celsius;

public:
    // Constructor
    Temperature(double initialTemp) {
        setTemperature(initialTemp);
    }
    
    // Public method to get temperature (getter)
    double getTemperature() const {
        return celsius;
    }
    
    // Public method to set temperature with validation (setter)
    void setTemperature(double newTemp) {
        // Absolute zero is -273.15°C - nothing can be colder
        if (newTemp >= -273.15) {
            celsius = newTemp;
            std::cout << "New temperature: " << celsius << "°C" << std::endl;
        } else {
            std::cout << "Error: Temperature cannot be below absolute zero (-273.15°C)" << std::endl;
        }
    }
    
    // Public method to display temperature
    void display() const {
        std::cout << "Current temperature: " << celsius << "°C" << std::endl;
    }
};

int main() {
    // Create a temperature object
    Temperature roomTemp(20.0);
    
    std::cout << "Initial temperature: " << roomTemp.getTemperature() << "°C" << std::endl;
    
    // Try to set a valid temperature
    roomTemp.setTemperature(25.0);
    
    // Try to set an invalid temperature
    roomTemp.setTemperature(-300.0);
    
    std::cout << "Temperature remains: " << roomTemp.getTemperature() << "°C" << std::endl;
    
    // Note: We cannot do roomTemp.celsius = -300; 
    // because celsius is private!
    
    return 0;
}