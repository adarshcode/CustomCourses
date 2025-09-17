// FILE: chapter-06.cpp
// Chapter 6 — Single Responsibility Principle — C++

#include <iostream>
#include <string>
using namespace std;

// ❌ BAD: Class with multiple responsibilities
class BadUserManager {
public:
    void saveUser(const string& name, const string& email) {
        // Responsibility 1: Validate user data
        if (name.empty()) throw invalid_argument("Name required");
        if (email.empty()) throw invalid_argument("Email required");
        
        // Responsibility 2: Save to database
        cout << "Saving user " << name << " to database..." << endl;
        
        // Responsibility 3: Send email
        cout << "Sending welcome email to " << email << "..." << endl;
        
        // Responsibility 4: Log the action
        cout << "User " << name << " created" << endl;
    }
};

// ✅ GOOD: Each class has one responsibility
struct User {
    string name;
    string email;
};

class UserValidator {
public:
    bool isValid(const User& user) {
        return !user.name.empty() && !user.email.empty();
    }
};

class UserRepository {
public:
    void save(const User& user) {
        cout << "Saving user " << user.name << " to database..." << endl;
    }
};

class EmailService {
public:
    void sendWelcomeEmail(const string& email) {
        cout << "Sending welcome email to " << email << "..." << endl;
    }
};

int main() {
    cout << "🎯 Single Responsibility Principle (C++)" << endl;
    cout << "=========================================" << endl << endl;
    
    // Bad example
    cout << "❌ Bad example (multiple responsibilities):" << endl;
    BadUserManager badManager;
    badManager.saveUser("John Doe", "john@example.com");
    
    cout << endl << "✅ Good example (single responsibilities):" << endl;
    User user{"Jane Doe", "jane@example.com"};
    UserValidator validator;
    UserRepository repository;
    EmailService emailService;
    
    if (validator.isValid(user)) {
        repository.save(user);
        emailService.sendWelcomeEmail(user.email);
        cout << "User created successfully!" << endl;
    }
    
    return 0;
}