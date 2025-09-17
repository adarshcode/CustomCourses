# FILE: chapter-11.py
# Chapter 11 — Design Patterns — Python

from abc import ABC, abstractmethod

# Observer Pattern - Notification system
class Observer(ABC):
    @abstractmethod
    def update(self, message):
        pass

class Subject:
    def __init__(self):
        self._observers = []
    
    def attach(self, observer):
        self._observers.append(observer)
    
    def notify(self, message):
        for observer in self._observers:
            observer.update(message)

class EmailNotifier(Observer):
    def __init__(self, email):
        self.email = email
    
    def update(self, message):
        print(f"📧 Email to {self.email}: {message}")

class SMSNotifier(Observer):
    def __init__(self, phone):
        self.phone = phone
    
    def update(self, message):
        print(f"📱 SMS to {self.phone}: {message}")

# Strategy Pattern - Payment processing
class PaymentStrategy(ABC):
    @abstractmethod
    def pay(self, amount):
        pass

class CreditCardPayment(PaymentStrategy):
    def pay(self, amount):
        print(f"💳 Paid ${amount} with Credit Card")

class PayPalPayment(PaymentStrategy):
    def pay(self, amount):
        print(f"🅿️ Paid ${amount} with PayPal")

class ShoppingCart:
    def __init__(self):
        self._payment_strategy = None
        self._total = 0
    
    def set_payment_strategy(self, strategy):
        self._payment_strategy = strategy
    
    def add_item(self, price):
        self._total += price
    
    def checkout(self):
        if self._payment_strategy:
            self._payment_strategy.pay(self._total)

def main():
    print("🎨 Design Patterns Example (Python)")
    print("====================================\n")
    
    # Observer Pattern Demo
    print("📢 Observer Pattern - Notification System:")
    news_service = Subject()
    email_user = EmailNotifier("user@example.com")
    sms_user = SMSNotifier("555-1234")
    
    news_service.attach(email_user)
    news_service.attach(sms_user)
    news_service.notify("Breaking News: Design Patterns are awesome!")
    
    print()
    
    # Strategy Pattern Demo
    print("💰 Strategy Pattern - Payment Processing:")
    cart = ShoppingCart()
    cart.add_item(29.99)
    cart.add_item(15.50)
    
    print("Paying with Credit Card:")
    cart.set_payment_strategy(CreditCardPayment())
    cart.checkout()
    
    print("Paying with PayPal:")
    cart.set_payment_strategy(PayPalPayment())
    cart.checkout()
    
    print("\n💡 Design Patterns Benefits:")
    print("   ✓ Observer: Loose coupling between publisher and subscribers")
    print("   ✓ Strategy: Easily switch algorithms at runtime")
    print("   ✓ Both: Follow SOLID principles")

if __name__ == "__main__":
    main()