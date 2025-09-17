// FILE: chapter-11.cpp
// Chapter 11 — Design Patterns — C++

#include <iostream>
#include <vector>
#include <memory>
#include <string>
using namespace std;

// Observer Pattern - Notification system
class Observer {
public:
    virtual void update(const string& message) = 0;
    virtual ~Observer() = default;
};

class Subject {
private:
    vector<Observer*> observers;
public:
    void attach(Observer* observer) {
        observers.push_back(observer);
    }
    
    void notify(const string& message) {
        for (Observer* observer : observers) {
            observer->update(message);
        }
    }
};

class EmailNotifier : public Observer {
private:
    string email;
public:
    EmailNotifier(const string& email) : email(email) {}
    
    void update(const string& message) override {
        cout << "📧 Email to " << email << ": " << message << endl;
    }
};

class SMSNotifier : public Observer {
private:
    string phone;
public:
    SMSNotifier(const string& phone) : phone(phone) {}
    
    void update(const string& message) override {
        cout << "📱 SMS to " << phone << ": " << message << endl;
    }
};

// Strategy Pattern - Payment processing
class PaymentStrategy {
public:
    virtual void pay(double amount) = 0;
    virtual ~PaymentStrategy() = default;
};

class CreditCardPayment : public PaymentStrategy {
public:
    void pay(double amount) override {
        cout << "💳 Paid $" << amount << " with Credit Card" << endl;
    }
};

class PayPalPayment : public PaymentStrategy {
public:
    void pay(double amount) override {
        cout << "🅿️ Paid $" << amount << " with PayPal" << endl;
    }
};

class ShoppingCart {
private:
    unique_ptr<PaymentStrategy> paymentStrategy;
    double total;
public:
    ShoppingCart() : total(0) {}
    
    void setPaymentStrategy(unique_ptr<PaymentStrategy> strategy) {
        paymentStrategy = move(strategy);
    }
    
    void addItem(double price) {
        total += price;
    }
    
    void checkout() {
        if (paymentStrategy) {
            paymentStrategy->pay(total);
        }
    }
};

int main() {
    cout << "🎨 Design Patterns Example (C++)" << endl;
    cout << "=================================" << endl << endl;
    
    // Observer Pattern Demo
    cout << "📢 Observer Pattern - Notification System:" << endl;
    Subject newsService;
    EmailNotifier emailUser("user@example.com");
    SMSNotifier smsUser("555-1234");
    
    newsService.attach(&emailUser);
    newsService.attach(&smsUser);
    newsService.notify("Breaking News: Design Patterns are awesome!");
    
    cout << endl;
    
    // Strategy Pattern Demo
    cout << "💰 Strategy Pattern - Payment Processing:" << endl;
    ShoppingCart cart;
    cart.addItem(29.99);
    cart.addItem(15.50);
    
    cout << "Paying with Credit Card:" << endl;
    cart.setPaymentStrategy(make_unique<CreditCardPayment>());
    cart.checkout();
    
    cout << "Paying with PayPal:" << endl;
    cart.setPaymentStrategy(make_unique<PayPalPayment>());
    cart.checkout();
    
    cout << endl << "💡 Design Patterns Benefits:" << endl;
    cout << "   ✓ Observer: Loose coupling between publisher and subscribers" << endl;
    cout << "   ✓ Strategy: Easily switch algorithms at runtime" << endl;
    cout << "   ✓ Both: Follow SOLID principles" << endl;
    
    return 0;
}