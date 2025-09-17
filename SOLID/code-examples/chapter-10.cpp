// FILE: chapter-10-dip.cpp
// Chapter 10 — Dependency Inversion Principle — Beginner/Intermediate

#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <map>
#include <stdexcept>
#include <algorithm>

// BAD EXAMPLE - Violates DIP
class EmailService {
public:
    void sendEmail(const std::string& to, const std::string& subject, const std::string& body) {
        std::cout << "Sending email to " << to << ": " << subject << std::endl;
    }
};

class SmsService {
public:
    void sendSms(const std::string& phoneNumber, const std::string& message) {
        std::cout << "Sending SMS to " << phoneNumber << ": " << message << std::endl;
    }
};

// High-level module depends directly on low-level modules - DIP violation!
class BadNotificationManager {
private:
    EmailService emailService;  // Direct dependency
    SmsService smsService;      // Direct dependency

public:
    void sendWelcomeNotification(const std::string& email, const std::string& phone, const std::string& userName) {
        // Business logic mixed with specific implementations
        emailService.sendEmail(email, "Welcome!", "Welcome " + userName + "!");
        smsService.sendSms(phone, "Welcome " + userName + "! Thanks for joining.");
    }
};

// GOOD EXAMPLE - Follows DIP

// Abstraction - both high-level and low-level modules depend on this
class NotificationSender {
public:
    virtual ~NotificationSender() = default;
    virtual void send(const std::string& recipient, const std::string& message) = 0;
    virtual std::string getSenderType() const = 0;
};

// Low-level modules implement the abstraction
class EmailNotificationSender : public NotificationSender {
public:
    void send(const std::string& recipient, const std::string& message) override {
        std::cout << "📧 Email to " << recipient << ": " << message << std::endl;
        // Actual email sending implementation would go here
    }
    
    std::string getSenderType() const override {
        return "Email";
    }
};

class SmsNotificationSender : public NotificationSender {
public:
    void send(const std::string& recipient, const std::string& message) override {
        std::cout << "📱 SMS to " << recipient << ": " << message << std::endl;
        // Actual SMS sending implementation would go here
    }
    
    std::string getSenderType() const override {
        return "SMS";
    }
};

class PushNotificationSender : public NotificationSender {
public:
    void send(const std::string& recipient, const std::string& message) override {
        std::cout << "🔔 Push notification to " << recipient << ": " << message << std::endl;
        // Actual push notification implementation would go here
    }
    
    std::string getSenderType() const override {
        return "Push";
    }
};

class SlackNotificationSender : public NotificationSender {
public:
    void send(const std::string& recipient, const std::string& message) override {
        std::cout << "💬 Slack message to " << recipient << ": " << message << std::endl;
        // Actual Slack API implementation would go here
    }
    
    std::string getSenderType() const override {
        return "Slack";
    }
};

// High-level module depends only on abstraction
class NotificationManager {
private:
    std::vector<std::unique_ptr<NotificationSender>> senders;

public:
    // Dependency injection through constructor
    NotificationManager(std::vector<std::unique_ptr<NotificationSender>> senders) 
        : senders(std::move(senders)) {
        if (this->senders.empty()) {
            throw std::invalid_argument("At least one sender must be provided");
        }
    }
    
    void sendWelcomeNotification(const std::string& recipient, const std::string& userName) {
        // Pure business logic - doesn't know about specific implementations
        std::string message = "Welcome " + userName + "! Thanks for joining our platform.";
        
        std::cout << "\nSending welcome notification to " << userName << "..." << std::endl;
        
        for (const auto& sender : senders) {
            std::cout << "Using " << sender->getSenderType() << " sender:" << std::endl;
            sender->send(recipient, message);
        }
    }
    
    void sendUrgentAlert(const std::string& recipient, const std::string& alertMessage) {
        // Business logic for urgent notifications
        std::string urgentMessage = "🚨 URGENT: " + alertMessage;
        
        std::cout << "\nSending urgent alert..." << std::endl;
        
        for (const auto& sender : senders) {
            std::cout << "Using " << sender->getSenderType() << " sender:" << std::endl;
            sender->send(recipient, urgentMessage);
        }
    }
    
    void sendCustomMessage(const std::string& recipient, const std::string& message, 
                          const std::string& senderType = "") {
        if (senderType.empty()) {
            // Send via all senders
            for (const auto& sender : senders) {
                sender->send(recipient, message);
            }
        } else {
            // Send via specific sender type
            auto it = std::find_if(senders.begin(), senders.end(),
                [&senderType](const auto& sender) {
                    return sender->getSenderType() == senderType;
                });
            
            if (it != senders.end()) {
                (*it)->send(recipient, message);
            } else {
                std::cout << "No sender of type '" << senderType << "' found." << std::endl;
            }
        }
    }
};

// Factory pattern also following DIP
class NotificationSenderFactory {
public:
    virtual ~NotificationSenderFactory() = default;
    virtual std::unique_ptr<NotificationSender> createSender(const std::string& type) = 0;
    virtual std::vector<std::unique_ptr<NotificationSender>> createAllSenders() = 0;
};

class DefaultNotificationSenderFactory : public NotificationSenderFactory {
public:
    std::unique_ptr<NotificationSender> createSender(const std::string& type) override {
        std::string lowerType = type;
        std::transform(lowerType.begin(), lowerType.end(), lowerType.begin(), ::tolower);
        
        if (lowerType == "email") {
            return std::make_unique<EmailNotificationSender>();
        } else if (lowerType == "sms") {
            return std::make_unique<SmsNotificationSender>();
        } else if (lowerType == "push") {
            return std::make_unique<PushNotificationSender>();
        } else if (lowerType == "slack") {
            return std::make_unique<SlackNotificationSender>();
        } else {
            throw std::invalid_argument("Unknown sender type: " + type);
        }
    }
    
    std::vector<std::unique_ptr<NotificationSender>> createAllSenders() override {
        std::vector<std::unique_ptr<NotificationSender>> senders;
        senders.push_back(std::make_unique<EmailNotificationSender>());
        senders.push_back(std::make_unique<SmsNotificationSender>());
        senders.push_back(std::make_unique<PushNotificationSender>());
        senders.push_back(std::make_unique<SlackNotificationSender>());
        return senders;
    }
};

// User repository example following DIP
class UserRepository {
public:
    virtual ~UserRepository() = default;
    virtual void saveUser(const std::string& userName, const std::string& email) = 0;
    virtual std::string getUserEmail(const std::string& userName) = 0;
};

class DatabaseUserRepository : public UserRepository {
public:
    void saveUser(const std::string& userName, const std::string& email) override {
        std::cout << "💾 Saving user " << userName << " to database" << std::endl;
    }
    
    std::string getUserEmail(const std::string& userName) override {
        return userName + "@example.com"; // Simulated database lookup
    }
};

class FileUserRepository : public UserRepository {
public:
    void saveUser(const std::string& userName, const std::string& email) override {
        std::cout << "📁 Saving user " << userName << " to file" << std::endl;
    }
    
    std::string getUserEmail(const std::string& userName) override {
        return userName + "@example.com"; // Simulated file lookup
    }
};

// User service depends on abstractions, not concrete implementations
class UserService {
private:
    std::unique_ptr<UserRepository> userRepository;
    std::unique_ptr<NotificationSenderFactory> senderFactory;

public:
    UserService(std::unique_ptr<UserRepository> userRepository, 
                std::unique_ptr<NotificationSenderFactory> senderFactory)
        : userRepository(std::move(userRepository)), senderFactory(std::move(senderFactory)) {}
    
    void registerUser(const std::string& userName, const std::string& email, 
                     const std::vector<std::string>& preferredNotificationTypes) {
        // Save user
        userRepository->saveUser(userName, email);
        
        // Create notification senders based on user preferences
        std::vector<std::unique_ptr<NotificationSender>> senders;
        for (const auto& type : preferredNotificationTypes) {
            try {
                senders.push_back(senderFactory->createSender(type));
            } catch (const std::invalid_argument& e) {
                std::cout << "Warning: " << e.what() << std::endl;
            }
        }
        
        // Send welcome notification
        if (!senders.empty()) {
            NotificationManager notificationManager(std::move(senders));
            notificationManager.sendWelcomeNotification(email, userName);
        }
    }
};

// Mock implementation for testing (demonstrates DIP benefit)
class MockNotificationSender : public NotificationSender {
private:
    std::string senderType;
    std::vector<std::pair<std::string, std::string>> sentMessages;

public:
    MockNotificationSender(const std::string& type = "Mock") : senderType(type) {}
    
    void send(const std::string& recipient, const std::string& message) override {
        sentMessages.emplace_back(recipient, message);
        std::cout << "🧪 Mock " << senderType << " to " << recipient << ": " << message << std::endl;
    }
    
    std::string getSenderType() const override {
        return senderType;
    }
    
    size_t getSentMessageCount() const {
        return sentMessages.size();
    }
};

int main() {
    std::cout << "=== DIP Violation Demo ===" << std::endl;
    
    BadNotificationManager badManager;
    badManager.sendWelcomeNotification("user@example.com", "+1234567890", "John");
    
    std::cout << "\n=== DIP Compliant Demo ===" << std::endl;
    
    // Create notification senders (could be configured via DI container)
    std::vector<std::unique_ptr<NotificationSender>> senders;
    senders.push_back(std::make_unique<EmailNotificationSender>());
    senders.push_back(std::make_unique<SmsNotificationSender>());
    senders.push_back(std::make_unique<PushNotificationSender>());
    
    // Inject dependencies
    NotificationManager goodManager(std::move(senders));
    goodManager.sendWelcomeNotification("user@example.com", "John");
    goodManager.sendUrgentAlert("admin@example.com", "Server CPU usage is at 95%");
    
    std::cout << "\n=== Factory Pattern with DIP ===" << std::endl;
    
    auto factory = std::make_unique<DefaultNotificationSenderFactory>();
    auto userRepository = std::make_unique<DatabaseUserRepository>();
    UserService userService(std::move(userRepository), std::move(factory));
    
    // User chooses their preferred notification methods
    std::vector<std::string> preferences = {"email", "push", "slack"};
    userService.registerUser("Alice", "alice@example.com", preferences);
    
    std::cout << "\n=== Easy Testing Demo ===" << std::endl;
    
    // Demonstrate how DIP makes testing easy
    std::vector<std::unique_ptr<NotificationSender>> mockSenders;
    auto emailMock = std::make_unique<MockNotificationSender>("Email");
    auto smsMock = std::make_unique<MockNotificationSender>("SMS");
    
    // Store references before moving
    auto* emailMockPtr = emailMock.get();
    auto* smsMockPtr = smsMock.get();
    
    mockSenders.push_back(std::move(emailMock));
    mockSenders.push_back(std::move(smsMock));
    
    NotificationManager testManager(std::move(mockSenders));
    testManager.sendWelcomeNotification("test@example.com", "TestUser");
    
    // In real tests, you could assert on the captured messages
    std::cout << "Mock email sender captured " << emailMockPtr->getSentMessageCount() << " messages" << std::endl;
    std::cout << "Mock SMS sender captured " << smsMockPtr->getSentMessageCount() << " messages" << std::endl;
    
    std::cout << "\n=== Different Repository Implementation ===" << std::endl;
    
    // Easy to switch to file-based storage
    auto fileFactory = std::make_unique<DefaultNotificationSenderFactory>();
    auto fileRepository = std::make_unique<FileUserRepository>();
    UserService fileUserService(std::move(fileRepository), std::move(fileFactory));
    
    std::vector<std::string> filePreferences = {"email", "sms"};
    fileUserService.registerUser("Bob", "bob@example.com", filePreferences);
    
    std::cout << "\n=== DIP Benefits ===" << std::endl;
    std::cout << "✓ Easy to test with mock implementations" << std::endl;
    std::cout << "✓ Can add new notification types without changing existing code" << std::endl;
    std::cout << "✓ Business logic is separated from implementation details" << std::endl;
    std::cout << "✓ High-level modules are stable and reusable" << std::endl;
    std::cout << "✓ Dependencies can be configured externally" << std::endl;
    std::cout << "✓ Can switch between different storage mechanisms easily" << std::endl;
    
    return 0;
}