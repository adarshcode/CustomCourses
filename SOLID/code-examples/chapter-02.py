# FILE: chapter-02-basic.py
# Chapter 2 — Encapsulation — Basic

class Temperature:
    """A class demonstrating basic encapsulation with temperature validation"""
    
    def __init__(self, initial_temp):
        # Private attribute (by convention, prefixed with underscore)
        self._celsius = 0
        self.set_temperature(initial_temp)
    
    def get_temperature(self):
        """Public method to get temperature (getter)"""
        return self._celsius
    
    def set_temperature(self, new_temp):
        """Public method to set temperature with validation (setter)"""
        # Absolute zero is -273.15°C - nothing can be colder
        if new_temp >= -273.15:
            self._celsius = new_temp
            print(f"New temperature: {self._celsius}°C")
        else:
            print("Error: Temperature cannot be below absolute zero (-273.15°C)")
    
    def display(self):
        """Public method to display temperature"""
        print(f"Current temperature: {self._celsius}°C")
    
    # Python property decorator - more Pythonic way to create getters/setters
    @property
    def celsius(self):
        """Property getter for temperature"""
        return self._celsius
    
    @celsius.setter
    def celsius(self, value):
        """Property setter for temperature with validation"""
        self.set_temperature(value)

def main():
    # Create a temperature object
    room_temp = Temperature(20.0)
    
    print(f"Initial temperature: {room_temp.get_temperature()}°C")
    
    # Try to set a valid temperature
    room_temp.set_temperature(25.0)
    
    # Try to set an invalid temperature
    room_temp.set_temperature(-300.0)
    
    print(f"Temperature remains: {room_temp.get_temperature()}°C")
    
    # Using the property syntax (more Pythonic)
    print("\n=== Using Properties ===")
    room_temp.celsius = 22.0  # Uses the setter
    print(f"Using property: {room_temp.celsius}°C")  # Uses the getter
    
    # Note: In Python, we could still access room_temp._celsius directly,
    # but the underscore indicates it's "private" by convention

if __name__ == "__main__":
    main()