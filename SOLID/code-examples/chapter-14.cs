// FILE: chapter-14-clean-code.cs
// Chapter 14 — Clean Code Basics — Beginner/Intermediate

using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace CleanCodeExample
{
    // BAD EXAMPLE - Messy, unreadable code
    public class u {
        public string n;
        public string e;
        public int a;
        public bool v;
        
        public bool chk() {
            if(n == null || n.Length < 2) return false;
            if(e == null || !e.Contains("@")) return false;
            if(a < 0 || a > 150) return false;
            return true;
        }
        
        public void p() {
            if(chk()) {
                if(a >= 18) {
                    Console.WriteLine("Adult: " + n);
                } else {
                    Console.WriteLine("Minor: " + n);
                }
            }
        }
    }

    // GOOD EXAMPLE - Clean, readable code

    // Constants for clarity
    public static class UserValidationConstants
    {
        public const int MINIMUM_NAME_LENGTH = 2;
        public const int MINIMUM_AGE = 0;
        public const int MAXIMUM_AGE = 150;
        public const int ADULT_AGE_THRESHOLD = 18;
    }

    // Enum for user types instead of magic numbers
    public enum UserType
    {
        Minor = 0,
        Adult = 1,
        Senior = 2
    }

    // Clean User class with proper encapsulation
    public class User
    {
        private string name;
        private string email;
        private int age;
        private bool isVerified;

        public string Name 
        { 
            get => name; 
            set => name = ValidateName(value); 
        }
        
        public string Email 
        { 
            get => email; 
            set => email = ValidateEmail(value); 
        }
        
        public int Age 
        { 
            get => age; 
            set => age = ValidateAge(value); 
        }
        
        public bool IsVerified 
        { 
            get => isVerified; 
            set => isVerified = value; 
        }

        public UserType UserType => DetermineUserType();

        public User(string name, string email, int age)
        {
            Name = name;
            Email = email;
            Age = age;
            IsVerified = false;
        }

        /// <summary>
        /// Validates all user data according to business rules
        /// </summary>
        /// <returns>True if all data is valid, false otherwise</returns>
        public bool IsValid()
        {
            try
            {
                ValidateName(name);
                ValidateEmail(email);
                ValidateAge(age);
                return true;
            }
            catch (ArgumentException)
            {
                return false;
            }
        }

        /// <summary>
        /// Gets a display-friendly description of the user
        /// </summary>
        /// <returns>Formatted string describing the user</returns>
        public string GetUserDescription()
        {
            if (!IsValid())
            {
                return "Invalid User";
            }

            string ageCategory = UserType == UserType.Adult ? "Adult" : "Minor";
            string verificationStatus = IsVerified ? "Verified" : "Unverified";
            
            return $"{ageCategory} User: {Name} ({verificationStatus})";
        }

        /// <summary>
        /// Displays user information to console with proper formatting
        /// </summary>
        public void DisplayUserInfo()
        {
            if (!IsValid())
            {
                Console.WriteLine("❌ Cannot display invalid user information");
                return;
            }

            Console.WriteLine("👤 User Information:");
            Console.WriteLine($"   Name: {Name}");
            Console.WriteLine($"   Email: {Email}");
            Console.WriteLine($"   Age: {Age}");
            Console.WriteLine($"   Type: {UserType}");
            Console.WriteLine($"   Status: {(IsVerified ? "✅ Verified" : "⚠️ Unverified")}");
        }

        private string ValidateName(string nameValue)
        {
            if (string.IsNullOrWhiteSpace(nameValue))
            {
                throw new ArgumentException("Name cannot be null or empty");
            }

            if (nameValue.Length < UserValidationConstants.MINIMUM_NAME_LENGTH)
            {
                throw new ArgumentException($"Name must be at least {UserValidationConstants.MINIMUM_NAME_LENGTH} characters long");
            }

            return nameValue.Trim();
        }

        private string ValidateEmail(string emailValue)
        {
            if (string.IsNullOrWhiteSpace(emailValue))
            {
                throw new ArgumentException("Email cannot be null or empty");
            }

            if (!emailValue.Contains("@") || !emailValue.Contains("."))
            {
                throw new ArgumentException("Email must be in valid format");
            }

            return emailValue.Trim().ToLower();
        }

        private int ValidateAge(int ageValue)
        {
            if (ageValue < UserValidationConstants.MINIMUM_AGE || ageValue > UserValidationConstants.MAXIMUM_AGE)
            {
                throw new ArgumentException($"Age must be between {UserValidationConstants.MINIMUM_AGE} and {UserValidationConstants.MAXIMUM_AGE}");
            }

            return ageValue;
        }

        private UserType DetermineUserType()
        {
            if (age >= 65)
                return UserType.Senior;
            else if (age >= UserValidationConstants.ADULT_AGE_THRESHOLD)
                return UserType.Adult;
            else
                return UserType.Minor;
        }
    }

    // Clean Order Processing Example
    public class OrderItem
    {
        public string ProductName { get; set; }
        public decimal Price { get; set; }
        public int Quantity { get; set; }
        public string Category { get; set; }

        public decimal TotalPrice => Price * Quantity;
    }

    public static class OrderConstants
    {
        public const decimal ELECTRONICS_TAX_RATE = 0.1m;
        public const decimal BOOK_BULK_DISCOUNT_RATE = 0.05m;
        public const int BOOK_BULK_QUANTITY_THRESHOLD = 5;
        public const decimal FREE_SHIPPING_THRESHOLD = 100.0m;
        public const decimal STANDARD_SHIPPING_COST = 10.0m;
        
        public const string ELECTRONICS_CATEGORY = "ELECTRONICS";
        public const string BOOKS_CATEGORY = "BOOKS";
    }

    public class OrderCalculator
    {
        /// <summary>
        /// Calculates the total order amount including taxes, discounts, and shipping
        /// </summary>
        /// <param name="orderItems">List of items in the order</param>
        /// <returns>Total order amount</returns>
        public decimal CalculateOrderTotal(List<OrderItem> orderItems)
        {
            ValidateOrderItems(orderItems);

            decimal subtotal = CalculateSubtotal(orderItems);
            decimal shippingCost = CalculateShippingCost(subtotal);
            decimal totalAmount = subtotal + shippingCost;

            return Math.Round(totalAmount, 2);
        }

        private void ValidateOrderItems(List<OrderItem> orderItems)
        {
            if (orderItems == null || !orderItems.Any())
            {
                throw new ArgumentException("Order must contain at least one item");
            }

            foreach (var item in orderItems)
            {
                if (item.Quantity <= 0)
                {
                    throw new ArgumentException($"Invalid quantity for item {item.ProductName}");
                }

                if (item.Price < 0)
                {
                    throw new ArgumentException($"Invalid price for item {item.ProductName}");
                }

                if (string.IsNullOrWhiteSpace(item.ProductName))
                {
                    throw new ArgumentException("Product name cannot be empty");
                }
            }
        }

        private decimal CalculateSubtotal(List<OrderItem> orderItems)
        {
            decimal subtotal = 0;

            foreach (var item in orderItems)
            {
                decimal itemTotal = CalculateItemTotal(item);
                subtotal += itemTotal;
            }

            return subtotal;
        }

        private decimal CalculateItemTotal(OrderItem item)
        {
            decimal baseTotal = item.TotalPrice;

            // Apply category-specific pricing rules
            switch (item.Category?.ToUpper())
            {
                case OrderConstants.ELECTRONICS_CATEGORY:
                    return ApplyElectronicsTax(baseTotal);
                
                case OrderConstants.BOOKS_CATEGORY:
                    return ApplyBookDiscount(baseTotal, item.Quantity);
                
                default:
                    return baseTotal;
            }
        }

        private decimal ApplyElectronicsTax(decimal amount)
        {
            return amount * (1 + OrderConstants.ELECTRONICS_TAX_RATE);
        }

        private decimal ApplyBookDiscount(decimal amount, int quantity)
        {
            if (quantity >= OrderConstants.BOOK_BULK_QUANTITY_THRESHOLD)
            {
                return amount * (1 - OrderConstants.BOOK_BULK_DISCOUNT_RATE);
            }
            
            return amount;
        }

        private decimal CalculateShippingCost(decimal subtotal)
        {
            return subtotal >= OrderConstants.FREE_SHIPPING_THRESHOLD 
                ? 0 
                : OrderConstants.STANDARD_SHIPPING_COST;
        }
    }

    // Clean User Service with proper separation of concerns
    public interface IUserRepository
    {
        Task SaveUserAsync(User user);
        Task<User> GetUserByEmailAsync(string email);
    }

    public interface IEmailService
    {
        Task SendWelcomeEmailAsync(string email, string name);
    }

    public class UserService
    {
        private readonly IUserRepository userRepository;
        private readonly IEmailService emailService;

        public UserService(IUserRepository userRepository, IEmailService emailService)
        {
            this.userRepository = userRepository ?? throw new ArgumentNullException(nameof(userRepository));
            this.emailService = emailService ?? throw new ArgumentNullException(nameof(emailService));
        }

        /// <summary>
        /// Registers a new user in the system
        /// </summary>
        /// <param name="name">User's full name</param>
        /// <param name="email">User's email address</param>
        /// <param name="age">User's age</param>
        /// <returns>Newly created user</returns>
        public async Task<User> RegisterUserAsync(string name, string email, int age)
        {
            await ValidateUserDoesNotExistAsync(email);

            var newUser = CreateUser(name, email, age);
            await SaveUserAsync(newUser);
            await SendWelcomeEmailAsync(newUser);

            return newUser;
        }

        private async Task ValidateUserDoesNotExistAsync(string email)
        {
            var existingUser = await userRepository.GetUserByEmailAsync(email);
            if (existingUser != null)
            {
                throw new InvalidOperationException($"User with email {email} already exists");
            }
        }

        private User CreateUser(string name, string email, int age)
        {
            try
            {
                return new User(name, email, age);
            }
            catch (ArgumentException ex)
            {
                throw new ArgumentException($"Invalid user data: {ex.Message}", ex);
            }
        }

        private async Task SaveUserAsync(User user)
        {
            try
            {
                await userRepository.SaveUserAsync(user);
            }
            catch (Exception ex)
            {
                throw new InvalidOperationException("Failed to save user to database", ex);
            }
        }

        private async Task SendWelcomeEmailAsync(User user)
        {
            try
            {
                await emailService.SendWelcomeEmailAsync(user.Email, user.Name);
            }
            catch (Exception ex)
            {
                // Log error but don't fail registration
                Console.WriteLine($"Warning: Failed to send welcome email to {user.Email}: {ex.Message}");
            }
        }
    }

    // Demo program showing clean code in action
    class Program
    {
        static void Main()
        {
            Console.WriteLine("=== Clean Code Demo ===\n");

            DemonstrateCleanUserClass();
            DemonstrateCleanOrderCalculation();

            Console.WriteLine("\n=== Clean Code Benefits ===");
            Console.WriteLine("✓ Code is easy to read and understand");
            Console.WriteLine("✓ Functions have single responsibilities");
            Console.WriteLine("✓ Names clearly express intent");
            Console.WriteLine("✓ Error handling is explicit and helpful");
            Console.WriteLine("✓ Constants replace magic numbers");
            Console.WriteLine("✓ Code is well-structured and organized");
        }

        private static void DemonstrateCleanUserClass()
        {
            Console.WriteLine("--- User Management Demo ---");

            try
            {
                // Create valid users
                var adultUser = new User("Alice Johnson", "alice@example.com", 25);
                var minorUser = new User("Bob Smith", "bob@example.com", 16);

                adultUser.IsVerified = true;

                adultUser.DisplayUserInfo();
                Console.WriteLine();
                minorUser.DisplayUserInfo();
                Console.WriteLine();
            }
            catch (ArgumentException ex)
            {
                Console.WriteLine($"❌ Error creating user: {ex.Message}");
            }

            // Demonstrate validation
            try
            {
                var invalidUser = new User("X", "invalid-email", -5);
            }
            catch (ArgumentException ex)
            {
                Console.WriteLine($"✅ Validation working: {ex.Message}");
            }
        }

        private static void DemonstrateCleanOrderCalculation()
        {
            Console.WriteLine("\n--- Order Calculation Demo ---");

            var orderItems = new List<OrderItem>
            {
                new OrderItem { ProductName = "Laptop", Price = 999.99m, Quantity = 1, Category = "ELECTRONICS" },
                new OrderItem { ProductName = "Programming Books", Price = 29.99m, Quantity = 6, Category = "BOOKS" },
                new OrderItem { ProductName = "Mouse Pad", Price = 9.99m, Quantity = 2, Category = "ACCESSORIES" }
            };

            var calculator = new OrderCalculator();
            decimal total = calculator.CalculateOrderTotal(orderItems);

            Console.WriteLine("Order Summary:");
            foreach (var item in orderItems)
            {
                Console.WriteLine($"  {item.ProductName}: ${item.Price} x {item.Quantity} = ${item.TotalPrice}");
            }
            Console.WriteLine($"\nTotal Amount: ${total}");
        }
    }
}