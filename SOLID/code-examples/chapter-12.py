# FILE: chapter-12.py
# Chapter 12 — SOLID in Practice — Python

from abc import ABC, abstractmethod

# SOLID Principles working together in an e-commerce system

# SRP - Single Responsibility
class Product:
    def __init__(self, product_id, name, price):
        self.id = product_id
        self.name = name
        self.price = price

class Order:
    def __init__(self, order_id):
        self.id = order_id
        self.products = []
    
    def add_product(self, product):
        self.products.append(product)
    
    def get_total(self):
        return sum(product.price for product in self.products)

# OCP & DIP - Payment strategies (open for extension)
class PaymentProcessor(ABC):
    @abstractmethod
    def process_payment(self, amount):
        pass

class CreditCardProcessor(PaymentProcessor):
    def process_payment(self, amount):
        print(f"💳 Processing ${amount} via Credit Card")
        return True  # Simulated success

class PayPalProcessor(PaymentProcessor):
    def process_payment(self, amount):
        print(f"🅿️ Processing ${amount} via PayPal")
        return True  # Simulated success

# ISP - Segregated interfaces
class Notifiable(ABC):
    @abstractmethod
    def send_notification(self, message):
        pass

class Trackable(ABC):
    @abstractmethod
    def track_order(self, order_id):
        pass

# LSP - Substitutable implementations
class EmailNotificationService(Notifiable):
    def send_notification(self, message):
        print(f"📧 Email: {message}")

class SMSNotificationService(Notifiable):
    def send_notification(self, message):
        print(f"📱 SMS: {message}")

# Main service coordinating everything
class OrderService:
    def __init__(self, payment_processor, notification_service):
        self.payment_processor = payment_processor
        self.notification_service = notification_service
    
    def process_order(self, order):
        print(f"🛒 Processing order {order.id}")
        
        # Process payment
        payment_success = self.payment_processor.process_payment(order.get_total())
        
        if payment_success:
            # Send confirmation
            self.notification_service.send_notification(
                f"Order {order.id} confirmed!"
            )
            return True
        
        return False

def main():
    print("🏢 SOLID Principles in Practice (Python)")
    print("========================================\n")
    
    # Create order
    order = Order("ORD-001")
    order.add_product(Product("P1", "Laptop", 999.99))
    order.add_product(Product("P2", "Mouse", 29.99))
    
    print(f"📦 Order created with total: ${order.get_total()}\n")
    
    # Process with different payment methods and notifications
    print("Processing with Credit Card + Email:")
    service1 = OrderService(
        CreditCardProcessor(),
        EmailNotificationService()
    )
    service1.process_order(order)
    
    print("\nProcessing with PayPal + SMS:")
    service2 = OrderService(
        PayPalProcessor(),
        SMSNotificationService()
    )
    service2.process_order(order)
    
    print("\n💡 SOLID Principles Applied:")
    print("   🎯 SRP: Each class has one responsibility")
    print("   🔓 OCP: Can add new payment/notification methods")
    print("   🔄 LSP: All implementations are substitutable")
    print("   🔀 ISP: Interfaces are focused and specific")
    print("   ⬇️ DIP: Depends on abstractions, not concretions")

if __name__ == "__main__":
    main()