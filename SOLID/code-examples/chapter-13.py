# FILE: chapter-13.py
# Chapter 13 — KISS, YAGNI, DRY — Python

# ❌ DRY Violation - Repeated validation logic
class BadOrderProcessor:
    def process_customer_order(self, customer_name, email, amount):
        # Repeated validation
        if not customer_name:
            raise ValueError("Name required")
        if not email:
            raise ValueError("Email required")
        if amount <= 0:
            raise ValueError("Amount must be positive")
        
        print(f"Processing customer order: {customer_name}, {email}, ${amount}")
    
    def process_supplier_order(self, supplier_name, email, amount):
        # Same validation repeated!
        if not supplier_name:
            raise ValueError("Name required")
        if not email:
            raise ValueError("Email required")
        if amount <= 0:
            raise ValueError("Amount must be positive")
        
        print(f"Processing supplier order: {supplier_name}, {email}, ${amount}")

# ✅ DRY Solution - Extract common validation
class OrderValidator:
    def validate_order(self, name, email, amount):
        if not name:
            raise ValueError("Name required")
        if not email:
            raise ValueError("Email required")
        if amount <= 0:
            raise ValueError("Amount must be positive")

# KISS - Simple order processor
class SimpleOrderProcessor:
    def __init__(self):
        self.validator = OrderValidator()
    
    def process_order(self, name, email, amount, order_type):
        # YAGNI - Only handle current requirements, not hypothetical future ones
        self.validator.validate_order(name, email, amount)
        print(f"Processing {order_type} order: {name}, {email}, ${amount}")

def main():
    print("🎯 KISS, YAGNI, DRY Principles (Python)")
    print("=======================================\n")
    
    processor = SimpleOrderProcessor()
    
    # Process different types of orders using same logic (DRY)
    processor.process_order("John Customer", "john@example.com", 100.00, "customer")
    processor.process_order("ABC Supplier", "abc@supplier.com", 500.00, "supplier")
    
    print("\n💡 Principles Applied:")
    print("   🎨 KISS: Simple, straightforward processing")
    print("   🔮 YAGNI: Only current features, no speculation")
    print("   🔁 DRY: Shared validation logic")

if __name__ == "__main__":
    main()