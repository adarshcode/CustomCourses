// FILE: chapter-13.cpp
// Chapter 13 — KISS, YAGNI, DRY — C++

#include <iostream>
#include <string>
#include <stdexcept>
using namespace std;

// ❌ DRY Violation - Repeated validation logic
class BadOrderProcessor {
public:
    void processCustomerOrder(const string& customerName, const string& email, double amount) {
        // Repeated validation
        if (customerName.empty()) throw invalid_argument("Name required");
        if (email.empty()) throw invalid_argument("Email required");
        if (amount <= 0) throw invalid_argument("Amount must be positive");
        
        cout << "Processing customer order: " << customerName << ", " << email << ", $" << amount << endl;
    }
    
    void processSupplierOrder(const string& supplierName, const string& email, double amount) {
        // Same validation repeated!
        if (supplierName.empty()) throw invalid_argument("Name required");
        if (email.empty()) throw invalid_argument("Email required");
        if (amount <= 0) throw invalid_argument("Amount must be positive");
        
        cout << "Processing supplier order: " << supplierName << ", " << email << ", $" << amount << endl;
    }
};

// ✅ DRY Solution - Extract common validation
class OrderValidator {
public:
    void validateOrder(const string& name, const string& email, double amount) {
        if (name.empty()) throw invalid_argument("Name required");
        if (email.empty()) throw invalid_argument("Email required");
        if (amount <= 0) throw invalid_argument("Amount must be positive");
    }
};

// KISS - Simple order processor
class SimpleOrderProcessor {
private:
    OrderValidator validator;
    
public:
    void processOrder(const string& name, const string& email, double amount, const string& orderType) {
        // YAGNI - Only handle current requirements, not hypothetical future ones
        validator.validateOrder(name, email, amount);
        cout << "Processing " << orderType << " order: " << name << ", " << email << ", $" << amount << endl;
    }
};

int main() {
    cout << "🎯 KISS, YAGNI, DRY Principles (C++)" << endl;
    cout << "=====================================" << endl << endl;
    
    SimpleOrderProcessor processor;
    
    // Process different types of orders using same logic (DRY)
    processor.processOrder("John Customer", "john@example.com", 100.00, "customer");
    processor.processOrder("ABC Supplier", "abc@supplier.com", 500.00, "supplier");
    
    cout << endl << "💡 Principles Applied:" << endl;
    cout << "   🎨 KISS: Simple, straightforward processing" << endl;
    cout << "   🔮 YAGNI: Only current features, no speculation" << endl;
    cout << "   🔁 DRY: Shared validation logic" << endl;
    
    return 0;
}