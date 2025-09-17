# FILE: chapter-03-basic.py
# Chapter 3 — Abstraction — Basic

import time

class Television:
    """Abstract the complexity of TV operations behind simple methods"""
    
    def __init__(self):
        self._is_on = False
        self._volume = 10
        self._channel = 1
        self._has_power = True  # Assume TV is plugged in
    
    def turn_on(self):
        """Simple interface - user doesn't need to know about internal state management"""
        if self._has_power:
            self._is_on = True
            print("TV is now ON")
        else:
            print("TV has no power!")
    
    def turn_off(self):
        self._is_on = False
        print("TV is now OFF")
    
    def volume_up(self):
        if self._is_on and self._volume < 100:
            self._volume += 1
            print(f"Volume: {self._volume}")
    
    def volume_down(self):
        if self._is_on and self._volume > 0:
            self._volume -= 1
            print(f"Volume: {self._volume}")
    
    def change_channel(self, new_channel):
        if self._is_on and 0 < new_channel <= 999:
            self._channel = new_channel
            print(f"Channel: {self._channel}")

class CoffeeMachine:
    """Another example - Coffee machine abstraction"""
    
    def __init__(self):
        self._has_water = True
        self._has_beans = True
        self._is_ready = True
    
    def make_coffee(self):
        """Simple interface hides complex coffee brewing process"""
        if self._check_readiness():
            self._perform_brewing_process()
            print("☕ Your coffee is ready!")
        else:
            print("❌ Cannot make coffee - machine needs maintenance")
    
    def _check_readiness(self):
        """Private method hides implementation complexity"""
        return self._has_water and self._has_beans and self._is_ready
    
    def _perform_brewing_process(self):
        """Complex internal process hidden from user"""
        print("Making coffee...")
        # Simulate brewing: heat water, grind beans, extract, etc.
        time.sleep(1)  # Simulate brewing time

def main():
    print("=== TV Remote Control Demo ===")
    
    tv = Television()
    
    # User interacts with simple interface
    tv.turn_on()
    tv.volume_up()
    tv.volume_up()
    tv.volume_up()
    tv.volume_up()
    tv.volume_up()
    tv.change_channel(5)
    tv.turn_off()
    
    print("\n=== Coffee Machine Demo ===")
    
    coffee = CoffeeMachine()
    coffee.make_coffee()
    
    # Note: Users don't need to know about:
    # - Internal state management
    # - Complex brewing algorithms  
    # - Hardware control details
    # - Error handling mechanisms

if __name__ == "__main__":
    main()