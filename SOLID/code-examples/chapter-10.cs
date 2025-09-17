// FILE: chapter-10-dip.cs
// Chapter 10 — Dependency Inversion Principle — Beginner/Intermediate

using System;
using System.Collections.Generic;

namespace DIPExample
{
    // BAD EXAMPLE - Violates DIP
    public class EmailService
    {
        public void SendEmail(string to, string subject, string body)
        {
            Console.WriteLine($"Sending email to {to}: {subject}");
        }
    }

    public class SmsService
    {
        public void SendSms(string phoneNumber, string message)
        {
            Console.WriteLine($"Sending SMS to {phoneNumber}: {message}");
        }
    }

    // High-level module depends directly on low-level modules - DIP violation!
    public class BadNotificationManager
    {
        private EmailService emailService = new EmailService();  // Direct dependency
        private SmsService smsService = new SmsService();        // Direct dependency
        
        public void SendWelcomeNotification(string email, string phone, string userName)
        {
            // Business logic mixed with specific implementations
            emailService.SendEmail(email, "Welcome!", $"Welcome {userName}!");
            smsService.SendSms(phone, $"Welcome {userName}! Thanks for joining.");
        }
    }

    // GOOD EXAMPLE - Follows DIP

    // Abstraction - both high-level and low-level modules depend on this
    public interface INotificationSender
    {
        void Send(string recipient, string message);
        string GetSenderType();
    }

    // Low-level modules implement the abstraction
    public class EmailNotificationSender : INotificationSender
    {
        public void Send(string recipient, string message)
        {
            Console.WriteLine($"📧 Email to {recipient}: {message}");
            // Actual email sending implementation would go here
        }
        
        public string GetSenderType() => "Email";
    }

    public class SmsNotificationSender : INotificationSender
    {
        public void Send(string recipient, string message)
        {
            Console.WriteLine($"📱 SMS to {recipient}: {message}");
            // Actual SMS sending implementation would go here
        }
        
        public string GetSenderType() => "SMS";
    }

    public class PushNotificationSender : INotificationSender
    {
        public void Send(string recipient, string message)
        {
            Console.WriteLine($"🔔 Push notification to {recipient}: {message}");
            // Actual push notification implementation would go here
        }
        
        public string GetSenderType() => "Push";
    }

    public class SlackNotificationSender : INotificationSender
    {
        public void Send(string recipient, string message)
        {
            Console.WriteLine($"💬 Slack message to {recipient}: {message}");
            // Actual Slack API implementation would go here
        }
        
        public string GetSenderType() => "Slack";
    }

    // High-level module depends only on abstraction
    public class NotificationManager
    {
        private readonly List<INotificationSender> senders;
        
        // Dependency injection through constructor
        public NotificationManager(List<INotificationSender> senders)
        {
            this.senders = senders ?? throw new ArgumentNullException(nameof(senders));
        }
        
        public void SendWelcomeNotification(string recipient, string userName)
        {
            // Pure business logic - doesn't know about specific implementations
            string message = $"Welcome {userName}! Thanks for joining our platform.";
            
            Console.WriteLine($"\nSending welcome notification to {userName}...");
            
            foreach (var sender in senders)
            {
                Console.WriteLine($"Using {sender.GetSenderType()} sender:");
                sender.Send(recipient, message);
            }
        }
        
        public void SendUrgentAlert(string recipient, string alertMessage)
        {
            // Business logic for urgent notifications
            string urgentMessage = $"🚨 URGENT: {alertMessage}";
            
            Console.WriteLine($"\nSending urgent alert...");
            
            foreach (var sender in senders)
            {
                Console.WriteLine($"Using {sender.GetSenderType()} sender:");
                sender.Send(recipient, urgentMessage);
            }
        }
        
        public void SendCustomMessage(string recipient, string message, string senderType = null)
        {
            if (string.IsNullOrEmpty(senderType))
            {
                // Send via all senders
                foreach (var sender in senders)
                {
                    sender.Send(recipient, message);
                }
            }
            else
            {
                // Send via specific sender type
                var specificSender = senders.Find(s => s.GetSenderType().Equals(senderType, StringComparison.OrdinalIgnoreCase));
                if (specificSender != null)
                {
                    specificSender.Send(recipient, message);
                }
                else
                {
                    Console.WriteLine($"No sender of type '{senderType}' found.");
                }
            }
        }
    }

    // Factory pattern also following DIP
    public interface INotificationSenderFactory
    {
        INotificationSender CreateSender(string type);
        List<INotificationSender> CreateAllSenders();
    }

    public class NotificationSenderFactory : INotificationSenderFactory
    {
        public INotificationSender CreateSender(string type)
        {
            return type.ToLower() switch
            {
                "email" => new EmailNotificationSender(),
                "sms" => new SmsNotificationSender(),
                "push" => new PushNotificationSender(),
                "slack" => new SlackNotificationSender(),
                _ => throw new ArgumentException($"Unknown sender type: {type}")
            };
        }
        
        public List<INotificationSender> CreateAllSenders()
        {
            return new List<INotificationSender>
            {
                new EmailNotificationSender(),
                new SmsNotificationSender(),
                new PushNotificationSender(),
                new SlackNotificationSender()
            };
        }
    }

    // User repository example following DIP
    public interface IUserRepository
    {
        void SaveUser(string userName, string email);
        string GetUserEmail(string userName);
    }

    public class DatabaseUserRepository : IUserRepository
    {
        public void SaveUser(string userName, string email)
        {
            Console.WriteLine($"💾 Saving user {userName} to database");
        }
        
        public string GetUserEmail(string userName)
        {
            return $"{userName}@example.com"; // Simulated database lookup
        }
    }

    // User service depends on abstractions, not concrete implementations
    public class UserService
    {
        private readonly IUserRepository userRepository;
        private readonly INotificationSenderFactory senderFactory;
        
        public UserService(IUserRepository userRepository, INotificationSenderFactory senderFactory)
        {
            this.userRepository = userRepository ?? throw new ArgumentNullException(nameof(userRepository));
            this.senderFactory = senderFactory ?? throw new ArgumentNullException(nameof(senderFactory));
        }
        
        public void RegisterUser(string userName, string email, List<string> preferredNotificationTypes)
        {
            // Save user
            userRepository.SaveUser(userName, email);
            
            // Create notification senders based on user preferences
            var senders = new List<INotificationSender>();
            foreach (var type in preferredNotificationTypes)
            {
                try
                {
                    senders.Add(senderFactory.CreateSender(type));
                }
                catch (ArgumentException)
                {
                    Console.WriteLine($"Warning: Unknown notification type '{type}' ignored.");
                }
            }
            
            // Send welcome notification
            if (senders.Count > 0)
            {
                var notificationManager = new NotificationManager(senders);
                notificationManager.SendWelcomeNotification(email, userName);
            }
        }
    }

    class Program
    {
        static void Main()
        {
            Console.WriteLine("=== DIP Violation Demo ===");
            
            var badManager = new BadNotificationManager();
            badManager.SendWelcomeNotification("user@example.com", "+1234567890", "John");
            
            Console.WriteLine("\n=== DIP Compliant Demo ===");
            
            // Create notification senders (could be configured via DI container)
            var senders = new List<INotificationSender>
            {
                new EmailNotificationSender(),
                new SmsNotificationSender(),
                new PushNotificationSender()
            };
            
            // Inject dependencies
            var goodManager = new NotificationManager(senders);
            goodManager.SendWelcomeNotification("user@example.com", "John");
            goodManager.SendUrgentAlert("admin@example.com", "Server CPU usage is at 95%");
            
            Console.WriteLine("\n=== Factory Pattern with DIP ===");
            
            var factory = new NotificationSenderFactory();
            var userRepository = new DatabaseUserRepository();
            var userService = new UserService(userRepository, factory);
            
            // User chooses their preferred notification methods
            var preferences = new List<string> { "email", "push", "slack" };
            userService.RegisterUser("Alice", "alice@example.com", preferences);
            
            Console.WriteLine("\n=== DIP Benefits ===");
            Console.WriteLine("✓ Easy to test with mock implementations");
            Console.WriteLine("✓ Can add new notification types without changing existing code");
            Console.WriteLine("✓ Business logic is separated from implementation details");
            Console.WriteLine("✓ High-level modules are stable and reusable");
            Console.WriteLine("✓ Dependencies can be configured externally");
            
            // Demonstrate easy testing scenario
            Console.WriteLine("\n=== Easy Testing Demo ===");
            Console.WriteLine("In tests, we can easily inject mock implementations:");
            Console.WriteLine("var mockSenders = new List<INotificationSender> { new MockEmailSender() };");
            Console.WriteLine("var testManager = new NotificationManager(mockSenders);");
        }
    }
}