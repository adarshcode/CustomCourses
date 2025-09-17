# FILE: chapter-01-basic.py
# Chapter 1 — What is OOP? — Basic

# A simple class representing a Dog
class Dog:
    # Constructor method - runs when we create a new dog
    def __init__(self, name, age):
        # Properties (data/state)
        self.name = name
        self.age = age
    
    # Method (behavior)
    def bark(self):
        print(f"{self.name} says: Woof!")

def main():
    # Creating objects (instances) from our class
    my_dog = Dog("Buddy", 3)
    friend_dog = Dog("Max", 5)
    
    # Using the objects
    print(f"My dog {my_dog.name} is {my_dog.age} years old")
    my_dog.bark()
    
    print(f"Friend's dog {friend_dog.name} is {friend_dog.age} years old")
    friend_dog.bark()

if __name__ == "__main__":
    main()