// FILE: chapter-03-basic.cpp
// Chapter 3 — Abstraction — Basic

#include <iostream>
#include <thread>
#include <chrono>

class Television {
private:
    bool isOn;
    int volume;
    int channel;
    bool hasPower;

public:
    Television() : isOn(false), volume(10), channel(1), hasPower(true) {}
    
    // Simple interface - user doesn't need to know about internal state management
    void turnOn() {
        if (hasPower) {
            isOn = true;
            std::cout << "TV is now ON" << std::endl;
        } else {
            std::cout << "TV has no power!" << std::endl;
        }
    }
    
    void turnOff() {
        isOn = false;
        std::cout << "TV is now OFF" << std::endl;
    }
    
    void volumeUp() {
        if (isOn && volume < 100) {
            volume++;
            std::cout << "Volume: " << volume << std::endl;
        }
    }
    
    void volumeDown() {
        if (isOn && volume > 0) {
            volume--;
            std::cout << "Volume: " << volume << std::endl;
        }
    }
    
    void changeChannel(int newChannel) {
        if (isOn && newChannel > 0 && newChannel <= 999) {
            channel = newChannel;
            std::cout << "Channel: " << channel << std::endl;
        }
    }
};

class CoffeeMachine {
private:
    bool hasWater;
    bool hasBeans;
    bool isReady;
    
    // Private methods hide implementation complexity
    bool checkReadiness() const {
        return hasWater && hasBeans && isReady;
    }
    
    void performBrewingProcess() {
        // Complex internal process hidden from user
        std::cout << "Making coffee..." << std::endl;
        // Simulate brewing: heat water, grind beans, extract, etc.
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

public:
    CoffeeMachine() : hasWater(true), hasBeans(true), isReady(true) {}
    
    // Simple interface hides complex coffee brewing process
    void makeCoffee() {
        if (checkReadiness()) {
            performBrewingProcess();
            std::cout << "☕ Your coffee is ready!" << std::endl;
        } else {
            std::cout << "❌ Cannot make coffee - machine needs maintenance" << std::endl;
        }
    }
};

int main() {
    std::cout << "=== TV Remote Control Demo ===" << std::endl;
    
    Television tv;
    
    // User interacts with simple interface
    tv.turnOn();
    tv.volumeUp();
    tv.volumeUp();
    tv.volumeUp();
    tv.volumeUp();
    tv.volumeUp();
    tv.changeChannel(5);
    tv.turnOff();
    
    std::cout << "\n=== Coffee Machine Demo ===" << std::endl;
    
    CoffeeMachine coffee;
    coffee.makeCoffee();
    
    // Note: Users don't need to know about:
    // - Internal state management
    // - Complex brewing algorithms  
    // - Hardware control details
    // - Error handling mechanisms
    
    return 0;
}