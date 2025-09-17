// FILE: chapter-12.cpp
// Chapter 12 — SOLID in Practice — C++

#include <iostream>
#include <vector>
#include <memory>
#include <string>
using namespace std;

// SOLID Principles working together in an e-commerce system

// SRP - Single Responsibility
class Product {
private:
    string id, name;
    double price;
public:
    Product(const string& id, const string& name, double price) 
        : id(id), name(name), price(price) {}
    
    string getId() const { return id; }
    string getName() const { return name; }
    double getPrice() const { return price; }
};

class Order {
private:
    string id;
    vector<Product> products;
public:
    Order(const string& id) : id(id) {}
    
    void addProduct(const Product& product) {
        products.push_back(product);
    }
    
    double getTotal() const {
        double total = 0;
        for (const auto& product : products) {
            total += product.getPrice();
        }
        return total;
    }
    
    const vector<Product>& getProducts() const { return products; }
    string getId() const { return id; }
};

// OCP & DIP - Payment strategies (open for extension)
class PaymentProcessor {
public:
    virtual bool processPayment(double amount) = 0;
    virtual ~PaymentProcessor() = default;
};

class CreditCardProcessor : public PaymentProcessor {
public:
    bool processPayment(double amount) override {
        cout << "💳 Processing $" << amount << " via Credit Card" << endl;
        return true; // Simulated success
    }
};

class PayPalProcessor : public PaymentProcessor {
public:
    bool processPayment(double amount) override {
        cout << "🅿️ Processing $" << amount << " via PayPal" << endl;
        return true; // Simulated success
    }
};

// ISP - Segregated interfaces
class Notifiable {
public:
    virtual void sendNotification(const string& message) = 0;
    virtual ~Notifiable() = default;
};

class Trackable {
public:
    virtual void trackOrder(const string& orderId) = 0;
    virtual ~Trackable() = default;
};

// LSP - Substitutable implementations
class EmailNotificationService : public Notifiable {
public:
    void sendNotification(const string& message) override {
        cout << "📧 Email: " << message << endl;
    }
};

class SMSNotificationService : public Notifiable {
public:
    void sendNotification(const string& message) override {
        cout << "📱 SMS: " << message << endl;
    }
};

// Main service coordinating everything
class OrderService {
private:
    unique_ptr<PaymentProcessor> paymentProcessor;
    unique_ptr<Notifiable> notificationService;
    
public:
    OrderService(unique_ptr<PaymentProcessor> processor, 
                 unique_ptr<Notifiable> notifier)
        : paymentProcessor(move(processor)), notificationService(move(notifier)) {}
    
    bool processOrder(const Order& order) {
        cout << "🛒 Processing order " << order.getId() << endl;
        
        // Process payment
        bool paymentSuccess = paymentProcessor->processPayment(order.getTotal());
        
        if (paymentSuccess) {
            // Send confirmation
            notificationService->sendNotification(
                "Order " + order.getId() + " confirmed!"
            );
            return true;
        }
        
        return false;
    }
};

int main() {
    cout << "🏢 SOLID Principles in Practice (C++)" << endl;
    cout << "======================================" << endl << endl;
    
    // Create order
    Order order("ORD-001");
    order.addProduct(Product("P1", "Laptop", 999.99));
    order.addProduct(Product("P2", "Mouse", 29.99));
    
    cout << "📦 Order created with total: $" << order.getTotal() << endl << endl;
    
    // Process with different payment methods and notifications
    cout << "Processing with Credit Card + Email:" << endl;
    OrderService service1(
        make_unique<CreditCardProcessor>(),
        make_unique<EmailNotificationService>()
    );
    service1.processOrder(order);
    
    cout << endl << "Processing with PayPal + SMS:" << endl;
    OrderService service2(
        make_unique<PayPalProcessor>(),
        make_unique<SMSNotificationService>()
    );
    service2.processOrder(order);
    
    cout << endl << "💡 SOLID Principles Applied:" << endl;
    cout << "   🎯 SRP: Each class has one responsibility" << endl;
    cout << "   🔓 OCP: Can add new payment/notification methods" << endl;
    cout << "   🔄 LSP: All implementations are substitutable" << endl;
    cout << "   🔀 ISP: Interfaces are focused and specific" << endl;
    cout << "   ⬇️ DIP: Depends on abstractions, not concretions" << endl;
    
    return 0;
}