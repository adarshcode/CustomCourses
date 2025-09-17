# FILE: chapter-06.py
# Chapter 6 — Single Responsibility Principle — Python

# ❌ BAD: Class with multiple responsibilities
class BadUserManager:
    def save_user(self, name, email):
        # Responsibility 1: Validate user data
        if not name:
            raise ValueError("Name required")
        if not email:
            raise ValueError("Email required")
        
        # Responsibility 2: Save to database
        print(f"Saving user {name} to database...")
        
        # Responsibility 3: Send email
        print(f"Sending welcome email to {email}...")
        
        # Responsibility 4: Log the action
        print(f"User {name} created")

# ✅ GOOD: Each class has one responsibility
class User:
    def __init__(self, name, email):
        self.name = name
        self.email = email

class UserValidator:
    def is_valid(self, user):
        return bool(user.name and user.email)

class UserRepository:
    def save(self, user):
        print(f"Saving user {user.name} to database...")

class EmailService:
    def send_welcome_email(self, email):
        print(f"Sending welcome email to {email}...")

def main():
    print("🎯 Single Responsibility Principle (Python)")
    print("============================================\n")
    
    # Bad example
    print("❌ Bad example (multiple responsibilities):")
    bad_manager = BadUserManager()
    bad_manager.save_user("John Doe", "john@example.com")
    
    print("\n✅ Good example (single responsibilities):")
    user = User("Jane Doe", "jane@example.com")
    validator = UserValidator()
    repository = UserRepository()
    email_service = EmailService()
    
    if validator.is_valid(user):
        repository.save(user)
        email_service.send_welcome_email(user.email)
        print("User created successfully!")

if __name__ == "__main__":
    main()