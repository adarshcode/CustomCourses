// FILE: chapter-14-clean-code.cpp
// Chapter 14 — Clean Code Basics — Beginner/Intermediate

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include <regex>
#include <iomanip>

// BAD EXAMPLE - Messy, unreadable code
class u {
public:
    std::string n;
    std::string e;
    int a;
    bool v;
    
    u(std::string n, std::string e, int a) : n(n), e(e), a(a), v(false) {}
    
    bool chk() {
        if(n.length() < 2) return false;
        if(e.find("@") == std::string::npos) return false;
        if(a < 0 || a > 150) return false;
        return true;
    }
    
    void p() {
        if(chk()) {
            if(a >= 18) {
                std::cout << "Adult: " << n << std::endl;
            } else {
                std::cout << "Minor: " << n << std::endl;
            }
        }
    }
};

// GOOD EXAMPLE - Clean, readable code

// Constants for clarity
namespace UserValidationConstants {
    const int MINIMUM_NAME_LENGTH = 2;
    const int MINIMUM_AGE = 0;
    const int MAXIMUM_AGE = 150;
    const int ADULT_AGE_THRESHOLD = 18;
    const std::string EMAIL_PATTERN = R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)";
}

// Enum for user types instead of magic numbers
enum class UserType {
    Minor,
    Adult,
    Senior
};

// Custom exception for validation errors
class ValidationError : public std::runtime_error {
public:
    explicit ValidationError(const std::string& message) : std::runtime_error(message) {}
};

// Clean User class with proper encapsulation
class User {
private:
    std::string name;
    std::string email;
    int age;
    bool isVerified;

public:
    /**
     * @brief Construct a new User object with validation
     * 
     * @param name User's full name (must be at least 2 characters)
     * @param email User's email address (must be valid format)
     * @param age User's age (must be between 0 and 150)
     * @throws ValidationError If any input data is invalid
     */
    User(const std::string& name, const std::string& email, int age)
        : name(validateName(name)), email(validateEmail(email)), age(validateAge(age)), isVerified(false) {}

    // Getters with clear names
    const std::string& getName() const { return name; }
    const std::string& getEmail() const { return email; }
    int getAge() const { return age; }
    bool getIsVerified() const { return isVerified; }
    
    // Setter with validation
    void setIsVerified(bool verified) { isVerified = verified; }

    /**
     * @brief Determine the user's category based on age
     * @return UserType category
     */
    UserType getUserType() const {
        if (age >= 65) {
            return UserType::Senior;
        } else if (age >= UserValidationConstants::ADULT_AGE_THRESHOLD) {
            return UserType::Adult;
        } else {
            return UserType::Minor;
        }
    }

    /**
     * @brief Check if all user data is valid according to business rules
     * @return true if all data is valid, false otherwise
     */
    bool isValid() const {
        try {
            validateName(name);
            validateEmail(email);
            validateAge(age);
            return true;
        } catch (const ValidationError&) {
            return false;
        }
    }

    /**
     * @brief Get a display-friendly description of the user
     * @return Formatted string describing the user
     */
    std::string getUserDescription() const {
        if (!isValid()) {
            return "Invalid User";
        }

        std::string ageCategory = getUserTypeString();
        std::string verificationStatus = isVerified ? "Verified" : "Unverified";
        
        return ageCategory + " User: " + name + " (" + verificationStatus + ")";
    }

    /**
     * @brief Display user information to console with proper formatting
     */
    void displayUserInfo() const {
        if (!isValid()) {
            std::cout << "❌ Cannot display invalid user information" << std::endl;
            return;
        }

        std::cout << "👤 User Information:" << std::endl;
        std::cout << "   Name: " << name << std::endl;
        std::cout << "   Email: " << email << std::endl;
        std::cout << "   Age: " << age << std::endl;
        std::cout << "   Type: " << getUserTypeString() << std::endl;
        std::cout << "   Status: " << (isVerified ? "✅ Verified" : "⚠️ Unverified") << std::endl;
    }

private:
    std::string validateName(const std::string& nameValue) const {
        if (nameValue.empty() || nameValue.find_first_not_of(' ') == std::string::npos) {
            throw ValidationError("Name cannot be null or empty");
        }

        std::string trimmedName = trim(nameValue);
        if (trimmedName.length() < UserValidationConstants::MINIMUM_NAME_LENGTH) {
            throw ValidationError("Name must be at least " + 
                std::to_string(UserValidationConstants::MINIMUM_NAME_LENGTH) + " characters long");
        }

        return trimmedName;
    }

    std::string validateEmail(const std::string& emailValue) const {
        if (emailValue.empty()) {
            throw ValidationError("Email cannot be null or empty");
        }

        std::string trimmedEmail = trim(emailValue);
        std::regex emailRegex(UserValidationConstants::EMAIL_PATTERN);
        
        if (!std::regex_match(trimmedEmail, emailRegex)) {
            throw ValidationError("Email must be in valid format");
        }

        // Convert to lowercase for consistency
        std::string lowerEmail = trimmedEmail;
        std::transform(lowerEmail.begin(), lowerEmail.end(), lowerEmail.begin(), ::tolower);
        
        return lowerEmail;
    }

    int validateAge(int ageValue) const {
        if (ageValue < UserValidationConstants::MINIMUM_AGE || ageValue > UserValidationConstants::MAXIMUM_AGE) {
            throw ValidationError("Age must be between " + 
                std::to_string(UserValidationConstants::MINIMUM_AGE) + " and " + 
                std::to_string(UserValidationConstants::MAXIMUM_AGE));
        }

        return ageValue;
    }

    std::string getUserTypeString() const {
        switch (getUserType()) {
            case UserType::Minor: return "Minor";
            case UserType::Adult: return "Adult";
            case UserType::Senior: return "Senior";
            default: return "Unknown";
        }
    }

    std::string trim(const std::string& str) const {
        size_t first = str.find_first_not_of(' ');
        if (first == std::string::npos) return "";
        size_t last = str.find_last_not_of(' ');
        return str.substr(first, (last - first + 1));
    }
};

// Clean Order Processing Example
struct OrderItem {
    std::string productName;
    double price;
    int quantity;
    std::string category;

    double getTotalPrice() const {
        return price * quantity;
    }
};

namespace OrderConstants {
    const double ELECTRONICS_TAX_RATE = 0.1;
    const double BOOK_BULK_DISCOUNT_RATE = 0.05;
    const int BOOK_BULK_QUANTITY_THRESHOLD = 5;
    const double FREE_SHIPPING_THRESHOLD = 100.0;
    const double STANDARD_SHIPPING_COST = 10.0;
    
    const std::string ELECTRONICS_CATEGORY = "ELECTRONICS";
    const std::string BOOKS_CATEGORY = "BOOKS";
}

class OrderCalculator {
public:
    /**
     * @brief Calculate the total order amount including taxes, discounts, and shipping
     * @param orderItems List of items in the order
     * @return Total order amount rounded to 2 decimal places
     * @throws std::invalid_argument If order items are invalid
     */
    double calculateOrderTotal(const std::vector<OrderItem>& orderItems) {
        validateOrderItems(orderItems);

        double subtotal = calculateSubtotal(orderItems);
        double shippingCost = calculateShippingCost(subtotal);
        double totalAmount = subtotal + shippingCost;

        return std::round(totalAmount * 100.0) / 100.0;  // Round to 2 decimal places
    }

private:
    void validateOrderItems(const std::vector<OrderItem>& orderItems) {
        if (orderItems.empty()) {
            throw std::invalid_argument("Order must contain at least one item");
        }

        for (const auto& item : orderItems) {
            if (item.quantity <= 0) {
                throw std::invalid_argument("Invalid quantity for item " + item.productName);
            }

            if (item.price < 0) {
                throw std::invalid_argument("Invalid price for item " + item.productName);
            }

            if (item.productName.empty()) {
                throw std::invalid_argument("Product name cannot be empty");
            }
        }
    }

    double calculateSubtotal(const std::vector<OrderItem>& orderItems) {
        double subtotal = 0.0;

        for (const auto& item : orderItems) {
            double itemTotal = calculateItemTotal(item);
            subtotal += itemTotal;
        }

        return subtotal;
    }

    double calculateItemTotal(const OrderItem& item) {
        double baseTotal = item.getTotalPrice();

        // Apply category-specific pricing rules
        std::string upperCategory = toUpperCase(item.category);
        
        if (upperCategory == OrderConstants::ELECTRONICS_CATEGORY) {
            return applyElectronicsTax(baseTotal);
        } else if (upperCategory == OrderConstants::BOOKS_CATEGORY) {
            return applyBookDiscount(baseTotal, item.quantity);
        } else {
            return baseTotal;
        }
    }

    double applyElectronicsTax(double amount) {
        return amount * (1.0 + OrderConstants::ELECTRONICS_TAX_RATE);
    }

    double applyBookDiscount(double amount, int quantity) {
        if (quantity >= OrderConstants::BOOK_BULK_QUANTITY_THRESHOLD) {
            return amount * (1.0 - OrderConstants::BOOK_BULK_DISCOUNT_RATE);
        }
        
        return amount;
    }

    double calculateShippingCost(double subtotal) {
        return subtotal >= OrderConstants::FREE_SHIPPING_THRESHOLD 
            ? 0.0 
            : OrderConstants::STANDARD_SHIPPING_COST;
    }

    std::string toUpperCase(const std::string& str) {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(), ::toupper);
        return result;
    }
};

// Clean User Service with proper separation of concerns
class UserRepository {
public:
    virtual ~UserRepository() = default;
    virtual void saveUser(const User& user) = 0;
    virtual std::unique_ptr<User> getUserByEmail(const std::string& email) = 0;
};

class EmailService {
public:
    virtual ~EmailService() = default;
    virtual void sendWelcomeEmail(const std::string& email, const std::string& name) = 0;
};

class UserService {
private:
    std::unique_ptr<UserRepository> userRepository;
    std::unique_ptr<EmailService> emailService;

public:
    /**
     * @brief Construct a new User Service with dependencies
     * @param userRepository Repository for user data persistence
     * @param emailService Service for sending emails
     */
    UserService(std::unique_ptr<UserRepository> userRepository, 
                std::unique_ptr<EmailService> emailService)
        : userRepository(std::move(userRepository)), emailService(std::move(emailService)) {
        
        if (!this->userRepository) {
            throw std::invalid_argument("UserRepository cannot be null");
        }
        if (!this->emailService) {
            throw std::invalid_argument("EmailService cannot be null");
        }
    }

    /**
     * @brief Register a new user in the system
     * @param name User's full name
     * @param email User's email address
     * @param age User's age
     * @return Newly created user
     * @throws ValidationError If user data is invalid
     * @throws std::runtime_error If user already exists or save fails
     */
    User registerUser(const std::string& name, const std::string& email, int age) {
        validateUserDoesNotExist(email);

        User newUser = createUser(name, email, age);
        saveUser(newUser);
        sendWelcomeEmail(newUser);

        return newUser;
    }

private:
    void validateUserDoesNotExist(const std::string& email) {
        auto existingUser = userRepository->getUserByEmail(email);
        if (existingUser != nullptr) {
            throw std::runtime_error("User with email " + email + " already exists");
        }
    }

    User createUser(const std::string& name, const std::string& email, int age) {
        try {
            return User(name, email, age);
        } catch (const ValidationError& ex) {
            throw ValidationError("Invalid user data: " + std::string(ex.what()));
        }
    }

    void saveUser(const User& user) {
        try {
            userRepository->saveUser(user);
        } catch (const std::exception& ex) {
            throw std::runtime_error("Failed to save user to database: " + std::string(ex.what()));
        }
    }

    void sendWelcomeEmail(const User& user) {
        try {
            emailService->sendWelcomeEmail(user.getEmail(), user.getName());
        } catch (const std::exception& ex) {
            // Log error but don't fail registration
            std::cout << "Warning: Failed to send welcome email to " << user.getEmail() 
                      << ": " << ex.what() << std::endl;
        }
    }
};

// Demo functions showing clean code in action
void demonstrateCleanUserClass() {
    std::cout << "--- User Management Demo ---" << std::endl;

    try {
        // Create valid users
        User adultUser("Alice Johnson", "alice@example.com", 25);
        User minorUser("Bob Smith", "bob@example.com", 16);

        adultUser.setIsVerified(true);

        adultUser.displayUserInfo();
        std::cout << std::endl;
        minorUser.displayUserInfo();
        std::cout << std::endl;
    } catch (const ValidationError& ex) {
        std::cout << "❌ Error creating user: " << ex.what() << std::endl;
    }

    // Demonstrate validation
    try {
        User invalidUser("X", "invalid-email", -5);
    } catch (const ValidationError& ex) {
        std::cout << "✅ Validation working: " << ex.what() << std::endl;
    }
}

void demonstrateCleanOrderCalculation() {
    std::cout << "\n--- Order Calculation Demo ---" << std::endl;

    std::vector<OrderItem> orderItems = {
        {"Laptop", 999.99, 1, "ELECTRONICS"},
        {"Programming Books", 29.99, 6, "BOOKS"},
        {"Mouse Pad", 9.99, 2, "ACCESSORIES"}
    };

    OrderCalculator calculator;
    double total = calculator.calculateOrderTotal(orderItems);

    std::cout << "Order Summary:" << std::endl;
    for (const auto& item : orderItems) {
        std::cout << "  " << item.productName << ": $" << std::fixed << std::setprecision(2) 
                  << item.price << " x " << item.quantity << " = $" << item.getTotalPrice() << std::endl;
    }
    std::cout << "\nTotal Amount: $" << std::fixed << std::setprecision(2) << total << std::endl;
}

int main() {
    std::cout << "=== Clean Code Demo ===" << std::endl << std::endl;

    demonstrateCleanUserClass();
    demonstrateCleanOrderCalculation();

    std::cout << "\n=== Clean Code Benefits ===" << std::endl;
    std::cout << "✓ Code is easy to read and understand" << std::endl;
    std::cout << "✓ Functions have single responsibilities" << std::endl;
    std::cout << "✓ Names clearly express intent" << std::endl;
    std::cout << "✓ Error handling is explicit and helpful" << std::endl;
    std::cout << "✓ Constants replace magic numbers" << std::endl;
    std::cout << "✓ Code is well-structured and organized" << std::endl;
    std::cout << "✓ Documentation explains purpose and usage" << std::endl;
    std::cout << "✓ RAII and proper resource management" << std::endl;

    return 0;
}