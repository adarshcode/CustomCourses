# FILE: chapter-10-dip.py
# Chapter 10 — Dependency Inversion Principle — Beginner/Intermediate

from abc import ABC, abstractmethod
from typing import List, Optional
from enum import Enum

# BAD EXAMPLE - Violates DIP
class EmailService:
    def send_email(self, to: str, subject: str, body: str) -> None:
        print(f"Sending email to {to}: {subject}")

class SmsService:
    def send_sms(self, phone_number: str, message: str) -> None:
        print(f"Sending SMS to {phone_number}: {message}")

# High-level module depends directly on low-level modules - DIP violation!
class BadNotificationManager:
    def __init__(self):
        self.email_service = EmailService()  # Direct dependency
        self.sms_service = SmsService()      # Direct dependency
    
    def send_welcome_notification(self, email: str, phone: str, user_name: str) -> None:
        # Business logic mixed with specific implementations
        self.email_service.send_email(email, "Welcome!", f"Welcome {user_name}!")
        self.sms_service.send_sms(phone, f"Welcome {user_name}! Thanks for joining.")

# GOOD EXAMPLE - Follows DIP

# Abstraction - both high-level and low-level modules depend on this
class NotificationSender(ABC):
    @abstractmethod
    def send(self, recipient: str, message: str) -> None:
        pass
    
    @abstractmethod
    def get_sender_type(self) -> str:
        pass

# Low-level modules implement the abstraction
class EmailNotificationSender(NotificationSender):
    def send(self, recipient: str, message: str) -> None:
        print(f"📧 Email to {recipient}: {message}")
        # Actual email sending implementation would go here
    
    def get_sender_type(self) -> str:
        return "Email"

class SmsNotificationSender(NotificationSender):
    def send(self, recipient: str, message: str) -> None:
        print(f"📱 SMS to {recipient}: {message}")
        # Actual SMS sending implementation would go here
    
    def get_sender_type(self) -> str:
        return "SMS"

class PushNotificationSender(NotificationSender):
    def send(self, recipient: str, message: str) -> None:
        print(f"🔔 Push notification to {recipient}: {message}")
        # Actual push notification implementation would go here
    
    def get_sender_type(self) -> str:
        return "Push"

class SlackNotificationSender(NotificationSender):
    def send(self, recipient: str, message: str) -> None:
        print(f"💬 Slack message to {recipient}: {message}")
        # Actual Slack API implementation would go here
    
    def get_sender_type(self) -> str:
        return "Slack"

# High-level module depends only on abstraction
class NotificationManager:
    def __init__(self, senders: List[NotificationSender]):
        if not senders:
            raise ValueError("At least one sender must be provided")
        self.senders = senders
    
    def send_welcome_notification(self, recipient: str, user_name: str) -> None:
        # Pure business logic - doesn't know about specific implementations
        message = f"Welcome {user_name}! Thanks for joining our platform."
        
        print(f"\nSending welcome notification to {user_name}...")
        
        for sender in self.senders:
            print(f"Using {sender.get_sender_type()} sender:")
            sender.send(recipient, message)
    
    def send_urgent_alert(self, recipient: str, alert_message: str) -> None:
        # Business logic for urgent notifications
        urgent_message = f"🚨 URGENT: {alert_message}"
        
        print(f"\nSending urgent alert...")
        
        for sender in self.senders:
            print(f"Using {sender.get_sender_type()} sender:")
            sender.send(recipient, urgent_message)
    
    def send_custom_message(self, recipient: str, message: str, sender_type: Optional[str] = None) -> None:
        if sender_type is None:
            # Send via all senders
            for sender in self.senders:
                sender.send(recipient, message)
        else:
            # Send via specific sender type
            specific_sender = next((s for s in self.senders if s.get_sender_type().lower() == sender_type.lower()), None)
            if specific_sender:
                specific_sender.send(recipient, message)
            else:
                print(f"No sender of type '{sender_type}' found.")

# Factory pattern also following DIP
class NotificationSenderFactory(ABC):
    @abstractmethod
    def create_sender(self, sender_type: str) -> NotificationSender:
        pass
    
    @abstractmethod
    def create_all_senders(self) -> List[NotificationSender]:
        pass

class DefaultNotificationSenderFactory(NotificationSenderFactory):
    def create_sender(self, sender_type: str) -> NotificationSender:
        sender_map = {
            "email": EmailNotificationSender,
            "sms": SmsNotificationSender,
            "push": PushNotificationSender,
            "slack": SlackNotificationSender
        }
        
        sender_class = sender_map.get(sender_type.lower())
        if sender_class:
            return sender_class()
        else:
            raise ValueError(f"Unknown sender type: {sender_type}")
    
    def create_all_senders(self) -> List[NotificationSender]:
        return [
            EmailNotificationSender(),
            SmsNotificationSender(),
            PushNotificationSender(),
            SlackNotificationSender()
        ]

# User repository example following DIP
class UserRepository(ABC):
    @abstractmethod
    def save_user(self, user_name: str, email: str) -> None:
        pass
    
    @abstractmethod
    def get_user_email(self, user_name: str) -> str:
        pass

class DatabaseUserRepository(UserRepository):
    def save_user(self, user_name: str, email: str) -> None:
        print(f"💾 Saving user {user_name} to database")
    
    def get_user_email(self, user_name: str) -> str:
        return f"{user_name}@example.com"  # Simulated database lookup

class FileUserRepository(UserRepository):
    def save_user(self, user_name: str, email: str) -> None:
        print(f"📁 Saving user {user_name} to file")
    
    def get_user_email(self, user_name: str) -> str:
        return f"{user_name}@example.com"  # Simulated file lookup

# User service depends on abstractions, not concrete implementations
class UserService:
    def __init__(self, user_repository: UserRepository, sender_factory: NotificationSenderFactory):
        self.user_repository = user_repository
        self.sender_factory = sender_factory
    
    def register_user(self, user_name: str, email: str, preferred_notification_types: List[str]) -> None:
        # Save user
        self.user_repository.save_user(user_name, email)
        
        # Create notification senders based on user preferences
        senders = []
        for notification_type in preferred_notification_types:
            try:
                sender = self.sender_factory.create_sender(notification_type)
                senders.append(sender)
            except ValueError as e:
                print(f"Warning: {e}")
        
        # Send welcome notification
        if senders:
            notification_manager = NotificationManager(senders)
            notification_manager.send_welcome_notification(email, user_name)

# Mock implementation for testing (demonstrates DIP benefit)
class MockNotificationSender(NotificationSender):
    def __init__(self, sender_type: str = "Mock"):
        self.sent_messages = []
        self.sender_type = sender_type
    
    def send(self, recipient: str, message: str) -> None:
        self.sent_messages.append((recipient, message))
        print(f"🧪 Mock {self.sender_type} to {recipient}: {message}")
    
    def get_sender_type(self) -> str:
        return self.sender_type

def main():
    print("=== DIP Violation Demo ===")
    
    bad_manager = BadNotificationManager()
    bad_manager.send_welcome_notification("user@example.com", "+1234567890", "John")
    
    print("\n=== DIP Compliant Demo ===")
    
    # Create notification senders (could be configured via DI container)
    senders = [
        EmailNotificationSender(),
        SmsNotificationSender(),
        PushNotificationSender()
    ]
    
    # Inject dependencies
    good_manager = NotificationManager(senders)
    good_manager.send_welcome_notification("user@example.com", "John")
    good_manager.send_urgent_alert("admin@example.com", "Server CPU usage is at 95%")
    
    print("\n=== Factory Pattern with DIP ===")
    
    factory = DefaultNotificationSenderFactory()
    user_repository = DatabaseUserRepository()
    user_service = UserService(user_repository, factory)
    
    # User chooses their preferred notification methods
    preferences = ["email", "push", "slack"]
    user_service.register_user("Alice", "alice@example.com", preferences)
    
    print("\n=== Easy Testing Demo ===")
    
    # Demonstrate how DIP makes testing easy
    mock_senders = [
        MockNotificationSender("Email"),
        MockNotificationSender("SMS")
    ]
    
    test_manager = NotificationManager(mock_senders)
    test_manager.send_welcome_notification("test@example.com", "TestUser")
    
    # In real tests, you could assert on mock_senders[0].sent_messages
    print(f"Mock email sender captured {len(mock_senders[0].sent_messages)} messages")
    print(f"Mock SMS sender captured {len(mock_senders[1].sent_messages)} messages")
    
    print("\n=== Different Repository Implementation ===")
    
    # Easy to switch to file-based storage
    file_repository = FileUserRepository()
    file_user_service = UserService(file_repository, factory)
    file_user_service.register_user("Bob", "bob@example.com", ["email", "sms"])
    
    print("\n=== DIP Benefits ===")
    print("✓ Easy to test with mock implementations")
    print("✓ Can add new notification types without changing existing code")
    print("✓ Business logic is separated from implementation details")
    print("✓ High-level modules are stable and reusable")
    print("✓ Dependencies can be configured externally")
    print("✓ Can switch between different storage mechanisms easily")

if __name__ == "__main__":
    main()