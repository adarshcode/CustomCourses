# FILE: chapter-14-clean-code.py
# Chapter 14 — Clean Code Basics — Beginner/Intermediate

from abc import ABC, abstractmethod
from typing import List, Optional
from enum import Enum
from dataclasses import dataclass
import re

# BAD EXAMPLE - Messy, unreadable code
class u:
    def __init__(self, n, e, a):
        self.n = n
        self.e = e
        self.a = a
        self.v = False
    
    def chk(self):
        if not self.n or len(self.n) < 2:
            return False
        if not self.e or "@" not in self.e:
            return False
        if self.a < 0 or self.a > 150:
            return False
        return True
    
    def p(self):
        if self.chk():
            if self.a >= 18:
                print("Adult:", self.n)
            else:
                print("Minor:", self.n)

# GOOD EXAMPLE - Clean, readable code

# Constants for clarity
class UserValidationConstants:
    MINIMUM_NAME_LENGTH = 2
    MINIMUM_AGE = 0
    MAXIMUM_AGE = 150
    ADULT_AGE_THRESHOLD = 18
    EMAIL_PATTERN = r'^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$'

class UserType(Enum):
    """Enumeration for different user age categories"""
    MINOR = "minor"
    ADULT = "adult"
    SENIOR = "senior"

class ValidationError(Exception):
    """Custom exception for validation errors"""
    pass

class User:
    """
    Represents a user in the system with proper validation and encapsulation.
    
    This class ensures that all user data meets business requirements
    and provides clear methods for user operations.
    """
    
    def __init__(self, name: str, email: str, age: int):
        """
        Initialize a new user with validation.
        
        Args:
            name: User's full name (must be at least 2 characters)
            email: User's email address (must be valid format)
            age: User's age (must be between 0 and 150)
            
        Raises:
            ValidationError: If any input data is invalid
        """
        self._name = self._validate_name(name)
        self._email = self._validate_email(email)
        self._age = self._validate_age(age)
        self._is_verified = False

    @property
    def name(self) -> str:
        """Get the user's name"""
        return self._name

    @property
    def email(self) -> str:
        """Get the user's email"""
        return self._email

    @property
    def age(self) -> int:
        """Get the user's age"""
        return self._age

    @property
    def is_verified(self) -> bool:
        """Get the user's verification status"""
        return self._is_verified

    @is_verified.setter
    def is_verified(self, value: bool) -> None:
        """Set the user's verification status"""
        self._is_verified = value

    @property
    def user_type(self) -> UserType:
        """Determine the user's category based on age"""
        if self._age >= 65:
            return UserType.SENIOR
        elif self._age >= UserValidationConstants.ADULT_AGE_THRESHOLD:
            return UserType.ADULT
        else:
            return UserType.MINOR

    def is_valid(self) -> bool:
        """
        Check if all user data is valid according to business rules.
        
        Returns:
            True if all data is valid, False otherwise
        """
        try:
            self._validate_name(self._name)
            self._validate_email(self._email)
            self._validate_age(self._age)
            return True
        except ValidationError:
            return False

    def get_user_description(self) -> str:
        """
        Get a display-friendly description of the user.
        
        Returns:
            Formatted string describing the user
        """
        if not self.is_valid():
            return "Invalid User"

        age_category = "Adult" if self.user_type == UserType.ADULT else self.user_type.value.title()
        verification_status = "Verified" if self._is_verified else "Unverified"
        
        return f"{age_category} User: {self._name} ({verification_status})"

    def display_user_info(self) -> None:
        """Display user information to console with proper formatting"""
        if not self.is_valid():
            print("❌ Cannot display invalid user information")
            return

        print("👤 User Information:")
        print(f"   Name: {self._name}")
        print(f"   Email: {self._email}")
        print(f"   Age: {self._age}")
        print(f"   Type: {self.user_type.value.title()}")
        print(f"   Status: {'✅ Verified' if self._is_verified else '⚠️ Unverified'}")

    def _validate_name(self, name: str) -> str:
        """Validate and normalize the user's name"""
        if not name or not name.strip():
            raise ValidationError("Name cannot be null or empty")

        cleaned_name = name.strip()
        if len(cleaned_name) < UserValidationConstants.MINIMUM_NAME_LENGTH:
            raise ValidationError(f"Name must be at least {UserValidationConstants.MINIMUM_NAME_LENGTH} characters long")

        return cleaned_name

    def _validate_email(self, email: str) -> str:
        """Validate and normalize the user's email"""
        if not email or not email.strip():
            raise ValidationError("Email cannot be null or empty")

        cleaned_email = email.strip().lower()
        if not re.match(UserValidationConstants.EMAIL_PATTERN, cleaned_email):
            raise ValidationError("Email must be in valid format")

        return cleaned_email

    def _validate_age(self, age: int) -> int:
        """Validate the user's age"""
        if not isinstance(age, int):
            raise ValidationError("Age must be an integer")
            
        if age < UserValidationConstants.MINIMUM_AGE or age > UserValidationConstants.MAXIMUM_AGE:
            raise ValidationError(f"Age must be between {UserValidationConstants.MINIMUM_AGE} and {UserValidationConstants.MAXIMUM_AGE}")

        return age

# Clean Order Processing Example
@dataclass
class OrderItem:
    """Represents an item in an order"""
    product_name: str
    price: float
    quantity: int
    category: str

    @property
    def total_price(self) -> float:
        """Calculate total price for this item"""
        return self.price * self.quantity

class OrderConstants:
    """Constants for order processing business rules"""
    ELECTRONICS_TAX_RATE = 0.1
    BOOK_BULK_DISCOUNT_RATE = 0.05
    BOOK_BULK_QUANTITY_THRESHOLD = 5
    FREE_SHIPPING_THRESHOLD = 100.0
    STANDARD_SHIPPING_COST = 10.0
    
    ELECTRONICS_CATEGORY = "ELECTRONICS"
    BOOKS_CATEGORY = "BOOKS"

class OrderCalculator:
    """
    Handles order total calculations including taxes, discounts, and shipping.
    
    This class encapsulates all the business logic for order pricing
    in a clean, testable way.
    """

    def calculate_order_total(self, order_items: List[OrderItem]) -> float:
        """
        Calculate the total order amount including taxes, discounts, and shipping.
        
        Args:
            order_items: List of items in the order
            
        Returns:
            Total order amount rounded to 2 decimal places
            
        Raises:
            ValueError: If order items are invalid
        """
        self._validate_order_items(order_items)

        subtotal = self._calculate_subtotal(order_items)
        shipping_cost = self._calculate_shipping_cost(subtotal)
        total_amount = subtotal + shipping_cost

        return round(total_amount, 2)

    def _validate_order_items(self, order_items: List[OrderItem]) -> None:
        """Validate that all order items are valid"""
        if not order_items:
            raise ValueError("Order must contain at least one item")

        for item in order_items:
            if item.quantity <= 0:
                raise ValueError(f"Invalid quantity for item {item.product_name}")

            if item.price < 0:
                raise ValueError(f"Invalid price for item {item.product_name}")

            if not item.product_name or not item.product_name.strip():
                raise ValueError("Product name cannot be empty")

    def _calculate_subtotal(self, order_items: List[OrderItem]) -> float:
        """Calculate subtotal for all items including category-specific pricing"""
        subtotal = 0.0

        for item in order_items:
            item_total = self._calculate_item_total(item)
            subtotal += item_total

        return subtotal

    def _calculate_item_total(self, item: OrderItem) -> float:
        """Calculate total for a single item including category-specific pricing"""
        base_total = item.total_price

        # Apply category-specific pricing rules
        category = item.category.upper() if item.category else ""
        
        if category == OrderConstants.ELECTRONICS_CATEGORY:
            return self._apply_electronics_tax(base_total)
        elif category == OrderConstants.BOOKS_CATEGORY:
            return self._apply_book_discount(base_total, item.quantity)
        else:
            return base_total

    def _apply_electronics_tax(self, amount: float) -> float:
        """Apply tax to electronics items"""
        return amount * (1 + OrderConstants.ELECTRONICS_TAX_RATE)

    def _apply_book_discount(self, amount: float, quantity: int) -> float:
        """Apply bulk discount to books if quantity threshold is met"""
        if quantity >= OrderConstants.BOOK_BULK_QUANTITY_THRESHOLD:
            return amount * (1 - OrderConstants.BOOK_BULK_DISCOUNT_RATE)
        
        return amount

    def _calculate_shipping_cost(self, subtotal: float) -> float:
        """Calculate shipping cost based on order subtotal"""
        return 0 if subtotal >= OrderConstants.FREE_SHIPPING_THRESHOLD else OrderConstants.STANDARD_SHIPPING_COST

# Clean User Service with proper separation of concerns
class UserRepository(ABC):
    """Abstract base class for user data persistence"""
    
    @abstractmethod
    async def save_user(self, user: User) -> None:
        """Save a user to the data store"""
        pass

    @abstractmethod
    async def get_user_by_email(self, email: str) -> Optional[User]:
        """Retrieve a user by email address"""
        pass

class EmailService(ABC):
    """Abstract base class for email operations"""
    
    @abstractmethod
    async def send_welcome_email(self, email: str, name: str) -> None:
        """Send a welcome email to a new user"""
        pass

class UserService:
    """
    Service for user management operations.
    
    This service coordinates between the repository and email service
    to handle user registration with proper error handling.
    """

    def __init__(self, user_repository: UserRepository, email_service: EmailService):
        """
        Initialize the service with its dependencies.
        
        Args:
            user_repository: Repository for user data persistence
            email_service: Service for sending emails
        """
        self._user_repository = user_repository
        self._email_service = email_service

    async def register_user(self, name: str, email: str, age: int) -> User:
        """
        Register a new user in the system.
        
        Args:
            name: User's full name
            email: User's email address  
            age: User's age
            
        Returns:
            Newly created user
            
        Raises:
            ValidationError: If user data is invalid
            ValueError: If user already exists
        """
        await self._validate_user_does_not_exist(email)

        new_user = self._create_user(name, email, age)
        await self._save_user(new_user)
        await self._send_welcome_email(new_user)

        return new_user

    async def _validate_user_does_not_exist(self, email: str) -> None:
        """Check that no user exists with the given email"""
        existing_user = await self._user_repository.get_user_by_email(email)
        if existing_user is not None:
            raise ValueError(f"User with email {email} already exists")

    def _create_user(self, name: str, email: str, age: int) -> User:
        """Create a new user with validation"""
        try:
            return User(name, email, age)
        except ValidationError as ex:
            raise ValidationError(f"Invalid user data: {ex}") from ex

    async def _save_user(self, user: User) -> None:
        """Save user to repository with error handling"""
        try:
            await self._user_repository.save_user(user)
        except Exception as ex:
            raise RuntimeError("Failed to save user to database") from ex

    async def _send_welcome_email(self, user: User) -> None:
        """Send welcome email with error handling"""
        try:
            await self._email_service.send_welcome_email(user.email, user.name)
        except Exception as ex:
            # Log error but don't fail registration
            print(f"Warning: Failed to send welcome email to {user.email}: {ex}")

# Demo functions showing clean code in action
def demonstrate_clean_user_class() -> None:
    """Demonstrate the clean User class functionality"""
    print("--- User Management Demo ---")

    try:
        # Create valid users
        adult_user = User("Alice Johnson", "alice@example.com", 25)
        minor_user = User("Bob Smith", "bob@example.com", 16)

        adult_user.is_verified = True

        adult_user.display_user_info()
        print()
        minor_user.display_user_info()
        print()
    except ValidationError as ex:
        print(f"❌ Error creating user: {ex}")

    # Demonstrate validation
    try:
        invalid_user = User("X", "invalid-email", -5)
    except ValidationError as ex:
        print(f"✅ Validation working: {ex}")

def demonstrate_clean_order_calculation() -> None:
    """Demonstrate the clean OrderCalculator functionality"""
    print("\n--- Order Calculation Demo ---")

    order_items = [
        OrderItem("Laptop", 999.99, 1, "ELECTRONICS"),
        OrderItem("Programming Books", 29.99, 6, "BOOKS"),
        OrderItem("Mouse Pad", 9.99, 2, "ACCESSORIES")
    ]

    calculator = OrderCalculator()
    total = calculator.calculate_order_total(order_items)

    print("Order Summary:")
    for item in order_items:
        print(f"  {item.product_name}: ${item.price} x {item.quantity} = ${item.total_price}")
    print(f"\nTotal Amount: ${total}")

def main():
    """Main demo function showcasing clean code principles"""
    print("=== Clean Code Demo ===\n")

    demonstrate_clean_user_class()
    demonstrate_clean_order_calculation()

    print("\n=== Clean Code Benefits ===")
    print("✓ Code is easy to read and understand")
    print("✓ Functions have single responsibilities")
    print("✓ Names clearly express intent")
    print("✓ Error handling is explicit and helpful")
    print("✓ Constants replace magic numbers")
    print("✓ Code is well-structured and organized")
    print("✓ Type hints improve code clarity")
    print("✓ Docstrings document purpose and usage")

if __name__ == "__main__":
    main()