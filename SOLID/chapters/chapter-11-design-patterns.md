# Chapter 11: Common Design Patterns and SOLID 🏗️

## Child-Friendly Explanation 🧸
Think about how you organize your toys. You might have special boxes for different types - one for LEGOs, one for cars, one for dolls. Design patterns are like having special "toy organization systems" that work really well for specific situations. Just like you don't need to invent a new way to organize toys every time (you can use the systems that work!), programmers use design patterns - tried-and-true ways to solve common problems that come up again and again.

## Developer-Level Explanation 👨‍💻
Design patterns are reusable solutions to common problems in software design. They represent blueprints that you can customize to solve a recurring design problem in your code. Just like an architect might use standard blueprints for certain types of buildings, programmers use design patterns to solve common coding challenges.

Think of design patterns as tried-and-tested recipes for building software. Just like how you might follow a recipe to bake a cake, you can follow a design pattern to solve a specific programming problem. The best part? These patterns often follow SOLID principles naturally!

## Why Use Design Patterns? 🤔

```csharp
// ❌ Without patterns - Hard to maintain and extend
public class VideoUploader
{
    public void UploadVideo(string title)
    {
        Console.WriteLine($"Uploaded: {title}");
        // Hardcoded notifications - difficult to extend
        Console.WriteLine("Email sent to Alice");
        Console.WriteLine("SMS sent to Bob");
        Console.WriteLine("Push notification sent to Charlie");
    }
}

// ✅ With Observer pattern - Easy to extend
public interface INotificationMethod
{
    void Notify(string message);
}

public class EmailNotification : INotificationMethod
{
    public void Notify(string message) => Console.WriteLine($"Email: {message}");
}

public class VideoUploaderWithPattern
{
    private List<INotificationMethod> notifiers = new List<INotificationMethod>();
    
    public void AddNotifier(INotificationMethod notifier) => notifiers.Add(notifier);
    
    public void UploadVideo(string title)
    {
        Console.WriteLine($"Uploaded: {title}");
        foreach (var notifier in notifiers)
            notifier.Notify($"New video: {title}");
    }
}
```

Design patterns help us:
1. **Solve common problems with proven solutions**
2. **Make code easier to understand and maintain**
3. **Follow SOLID principles naturally**
4. **Create flexible, extensible systems**

## Observer Pattern 👀

The Observer pattern is like a newsletter subscription. When something interesting happens (like a new article being published), all the subscribers get notified automatically.

### Real-World Example: YouTube Notification System

```csharp
// Subject interface - what can be observed
public interface IObservable
{
    void Subscribe(IObserver observer);
    void Unsubscribe(IObserver observer);
    void NotifyObservers(string message);
}

// Observer interface - who gets notified
public interface IObserver
{
    void Update(string channelName, string message);
}

// Concrete observers (different types of subscribers)
public class EmailSubscriber : IObserver
{
    private string email;
    
    public EmailSubscriber(string email) => this.email = email;
    
    public void Update(string channelName, string message)
    {
        Console.WriteLine($"📧 Email to {email}: {channelName} - {message}");
    }
}

public class MobileSubscriber : IObserver
{
    private string phoneNumber;
    
    public MobileSubscriber(string phoneNumber) => this.phoneNumber = phoneNumber;
    
    public void Update(string channelName, string message)
    {
        Console.WriteLine($"📱 Push notification to {phoneNumber}: {channelName} - {message}");
    }
}

public class WebSubscriber : IObserver
{
    private string username;
    
    public WebSubscriber(string username) => this.username = username;
    
    public void Update(string channelName, string message)
    {
        Console.WriteLine($"🌐 Web notification for {username}: {channelName} - {message}");
    }
}

// Concrete subject (YouTuber)
public class YouTubeChannel : IObservable
{
    private List<IObserver> subscribers = new List<IObserver>();
    private string channelName;
    
    public YouTubeChannel(string channelName) => this.channelName = channelName;
    
    public void Subscribe(IObserver observer)
    {
        subscribers.Add(observer);
        Console.WriteLine($"✅ New subscriber added to {channelName}");
    }
    
    public void Unsubscribe(IObserver observer)
    {
        subscribers.Remove(observer);
        Console.WriteLine($"❌ Subscriber removed from {channelName}");
    }
    
    public void NotifyObservers(string message)
    {
        Console.WriteLine($"\n📢 {channelName} is notifying {subscribers.Count} subscribers...");
        foreach (var subscriber in subscribers)
        {
            subscriber.Update(channelName, message);
        }
    }
    
    public void UploadVideo(string videoTitle)
    {
        Console.WriteLine($"🎬 {channelName} uploaded: '{videoTitle}'");
        NotifyObservers($"New video uploaded: {videoTitle}");
    }
    
    public void GoLive(string streamTitle)
    {
        Console.WriteLine($"🔴 {channelName} went live: '{streamTitle}'");
        NotifyObservers($"Live stream started: {streamTitle}");
    }
}

// Usage example
public class ObserverDemo
{
    public static void RunDemo()
    {
        var techChannel = new YouTubeChannel("TechExplained");
        
        // Different types of subscribers
        var emailSub = new EmailSubscriber("alice@email.com");
        var mobileSub = new MobileSubscriber("+1-555-0123");
        var webSub = new WebSubscriber("techfan2024");
        
        // Subscribe to the channel
        techChannel.Subscribe(emailSub);
        techChannel.Subscribe(mobileSub);
        techChannel.Subscribe(webSub);
        
        // Channel activities trigger notifications
        techChannel.UploadVideo("SOLID Principles Explained");
        techChannel.GoLive("Live Q&A Session");
        
        // Unsubscribe one user
        techChannel.Unsubscribe(emailSub);
        techChannel.UploadVideo("Design Patterns Tutorial");
    }
}
```

### How Observer Pattern Follows SOLID:

```python
# Python example showing SOLID adherence
from abc import ABC, abstractmethod
from typing import List

# SRP: Each class has one responsibility
class Observer(ABC):
    @abstractmethod
    def update(self, stock_symbol: str, price: float):
        pass

class StockDisplay(Observer):
    """Only responsible for displaying stock prices"""
    def __init__(self, name: str):
        self.name = name
    
    def update(self, stock_symbol: str, price: float):
        print(f"{self.name} Display: {stock_symbol} is now ${price:.2f}")

class StockAlert(Observer):
    """Only responsible for sending alerts"""
    def __init__(self, threshold: float):
        self.threshold = threshold
    
    def update(self, stock_symbol: str, price: float):
        if price > self.threshold:
            print(f"🚨 ALERT: {stock_symbol} exceeded ${self.threshold}! Current: ${price:.2f}")

# OCP: Can add new observer types without modifying existing code
class StockLogger(Observer):
    """New observer type - logs to file"""
    def update(self, stock_symbol: str, price: float):
        with open("stock_log.txt", "a") as f:
            f.write(f"{stock_symbol}: ${price:.2f}\n")

class Stock:
    def __init__(self, symbol: str, price: float):
        self.symbol = symbol
        self._price = price
        self._observers: List[Observer] = []
    
    def attach(self, observer: Observer):
        self._observers.append(observer)
    
    def detach(self, observer: Observer):
        self._observers.remove(observer)
    
    def notify(self):
        for observer in self._observers:
            observer.update(self.symbol, self._price)
    
    @property
    def price(self):
        return self._price
    
    @price.setter
    def price(self, new_price: float):
        self._price = new_price
        self.notify()  # Automatically notify when price changes

# Usage
apple_stock = Stock("AAPL", 150.00)

display1 = StockDisplay("Main Dashboard")
display2 = StockDisplay("Mobile App")
alert = StockAlert(155.00)
logger = StockLogger()

apple_stock.attach(display1)
apple_stock.attach(display2)
apple_stock.attach(alert)
apple_stock.attach(logger)

# Price changes automatically notify all observers
apple_stock.price = 152.30  # Normal update
apple_stock.price = 157.80  # Triggers alert
```

## Strategy Pattern 🎯

The Strategy pattern lets you choose different algorithms at runtime. It's like having multiple tools for the same job and picking the best one for each situation.

### Real-World Example: Payment Processing System

```csharp
// Strategy interface
public interface IPaymentStrategy
{
    PaymentResult ProcessPayment(decimal amount, string merchantId);
    string GetPaymentMethodName();
    decimal CalculateFees(decimal amount);
}

// Result class for payment operations
public class PaymentResult
{
    public bool IsSuccessful { get; set; }
    public string TransactionId { get; set; }
    public string Message { get; set; }
    public decimal ProcessingFee { get; set; }
}

// Concrete strategies
public class CreditCardStrategy : IPaymentStrategy
{
    private string cardNumber;
    private string holderName;
    
    public CreditCardStrategy(string cardNumber, string holderName)
    {
        this.cardNumber = cardNumber.Substring(cardNumber.Length - 4); // Only store last 4 digits
        this.holderName = holderName;
    }
    
    public PaymentResult ProcessPayment(decimal amount, string merchantId)
    {
        // Simulate credit card processing
        var processingFee = CalculateFees(amount);
        var totalAmount = amount + processingFee;
        
        return new PaymentResult
        {
            IsSuccessful = true,
            TransactionId = $"CC_{Guid.NewGuid().ToString()[..8]}",
            Message = $"Credit card ending in {cardNumber} charged ${totalAmount:F2}",
            ProcessingFee = processingFee
        };
    }
    
    public string GetPaymentMethodName() => "Credit Card";
    
    public decimal CalculateFees(decimal amount) => amount * 0.029m; // 2.9% fee
}

public class PayPalStrategy : IPaymentStrategy
{
    private string email;
    
    public PayPalStrategy(string email) => this.email = email;
    
    public PaymentResult ProcessPayment(decimal amount, string merchantId)
    {
        var processingFee = CalculateFees(amount);
        var totalAmount = amount + processingFee;
        
        return new PaymentResult
        {
            IsSuccessful = true,
            TransactionId = $"PP_{Guid.NewGuid().ToString()[..8]}",
            Message = $"PayPal account {email} charged ${totalAmount:F2}",
            ProcessingFee = processingFee
        };
    }
    
    public string GetPaymentMethodName() => "PayPal";
    
    public decimal CalculateFees(decimal amount) => amount * 0.034m; // 3.4% fee
}

public class CryptocurrencyStrategy : IPaymentStrategy
{
    private string walletAddress;
    private string coinType;
    
    public CryptocurrencyStrategy(string walletAddress, string coinType)
    {
        this.walletAddress = walletAddress;
        this.coinType = coinType;
    }
    
    public PaymentResult ProcessPayment(decimal amount, string merchantId)
    {
        var processingFee = CalculateFees(amount);
        var totalAmount = amount + processingFee;
        
        return new PaymentResult
        {
            IsSuccessful = true,
            TransactionId = $"CRYPTO_{Guid.NewGuid().ToString()[..8]}",
            Message = $"{coinType} payment from wallet {walletAddress[..6]}... - ${totalAmount:F2}",
            ProcessingFee = processingFee
        };
    }
    
    public string GetPaymentMethodName() => $"{coinType} Cryptocurrency";
    
    public decimal CalculateFees(decimal amount) => 2.50m; // Flat fee
}

// Context class
public class PaymentProcessor
{
    private IPaymentStrategy paymentStrategy;
    
    public void SetPaymentStrategy(IPaymentStrategy strategy)
    {
        this.paymentStrategy = strategy;
        Console.WriteLine($"Payment method set to: {strategy.GetPaymentMethodName()}");
    }
    
    public PaymentResult ProcessOrder(decimal orderAmount, string merchantId)
    {
        if (paymentStrategy == null)
        {
            return new PaymentResult
            {
                IsSuccessful = false,
                Message = "No payment method selected"
            };
        }
        
        Console.WriteLine($"\nProcessing ${orderAmount:F2} order...");
        Console.WriteLine($"Payment method: {paymentStrategy.GetPaymentMethodName()}");
        Console.WriteLine($"Processing fee: ${paymentStrategy.CalculateFees(orderAmount):F2}");
        
        return paymentStrategy.ProcessPayment(orderAmount, merchantId);
    }
    
    public void ComparePaymentMethods(decimal amount)
    {
        var strategies = new List<IPaymentStrategy>
        {
            new CreditCardStrategy("1234-5678-9012-3456", "John Doe"),
            new PayPalStrategy("john@email.com"),
            new CryptocurrencyStrategy("1A1zP1eP5QGefi2DMPTfTL5SLmv7DivfNa", "Bitcoin")
        };
        
        Console.WriteLine($"\n💰 Comparing payment methods for ${amount:F2}:");
        Console.WriteLine("Method               | Fee      | Total");
        Console.WriteLine("---------------------|----------|----------");
        
        foreach (var strategy in strategies)
        {
            var fee = strategy.CalculateFees(amount);
            var total = amount + fee;
            Console.WriteLine($"{strategy.GetPaymentMethodName(),-20} | ${fee,6:F2} | ${total,6:F2}");
        }
    }
}

// Usage example
public class StrategyDemo
{
    public static void RunDemo()
    {
        var processor = new PaymentProcessor();
        
        // Compare different payment methods
        processor.ComparePaymentMethods(100.00m);
        
        // Process payments with different strategies
        var creditCard = new CreditCardStrategy("1234-5678-9012-3456", "Alice Smith");
        processor.SetPaymentStrategy(creditCard);
        var result1 = processor.ProcessOrder(100.00m, "MERCHANT_123");
        Console.WriteLine($"Result: {result1.Message}");
        
        var paypal = new PayPalStrategy("alice@email.com");
        processor.SetPaymentStrategy(paypal);
        var result2 = processor.ProcessOrder(100.00m, "MERCHANT_123");
        Console.WriteLine($"Result: {result2.Message}");
        
        var crypto = new CryptocurrencyStrategy("1A1zP1eP5QGefi2DMPTfTL5SLmv7DivfNa", "Bitcoin");
        processor.SetPaymentStrategy(crypto);
        var result3 = processor.ProcessOrder(100.00m, "MERCHANT_123");
        Console.WriteLine($"Result: {result3.Message}");
    }
}
```

### Strategy Pattern in Python:

```python
# Python implementation with duck typing
from abc import ABC, abstractmethod
import random

class CompressionStrategy(ABC):
    @abstractmethod
    def compress(self, data: str) -> str:
        pass
    
    @abstractmethod
    def get_compression_ratio(self) -> float:
        pass

class ZipCompression(CompressionStrategy):
    def compress(self, data: str) -> str:
        compressed_size = len(data) * 0.6  # Simulate 60% of original size
        return f"ZIP compressed: {len(data)} bytes → {compressed_size:.0f} bytes"
    
    def get_compression_ratio(self) -> float:
        return 0.6

class RarCompression(CompressionStrategy):
    def compress(self, data: str) -> str:
        compressed_size = len(data) * 0.4  # Simulate 40% of original size
        return f"RAR compressed: {len(data)} bytes → {compressed_size:.0f} bytes"
    
    def get_compression_ratio(self) -> float:
        return 0.4

class LzwCompression(CompressionStrategy):
    def compress(self, data: str) -> str:
        compressed_size = len(data) * 0.7  # Simulate 70% of original size
        return f"LZW compressed: {len(data)} bytes → {compressed_size:.0f} bytes"
    
    def get_compression_ratio(self) -> float:
        return 0.7

class FileCompressor:
    def __init__(self):
        self._strategy = None
    
    def set_strategy(self, strategy: CompressionStrategy):
        self._strategy = strategy
        print(f"Compression strategy set to: {strategy.__class__.__name__}")
    
    def compress_file(self, filename: str, data: str):
        if not self._strategy:
            return "No compression strategy set!"
        
        print(f"\nCompressing {filename}...")
        result = self._strategy.compress(data)
        ratio = self._strategy.get_compression_ratio()
        savings = (1 - ratio) * 100
        print(f"Compression: {savings:.1f}% space saved")
        return result
    
    def auto_select_strategy(self, file_type: str):
        """Automatically select the best strategy based on file type"""
        strategies = {
            'text': ZipCompression(),
            'image': RarCompression(),
            'binary': LzwCompression()
        }
        
        strategy = strategies.get(file_type, ZipCompression())
        self.set_strategy(strategy)
        return strategy

# Usage
compressor = FileCompressor()
test_data = "This is a sample file content that we want to compress efficiently."

# Manual strategy selection
compressor.set_strategy(ZipCompression())
print(compressor.compress_file("document.txt", test_data))

compressor.set_strategy(RarCompression())
print(compressor.compress_file("photo.jpg", test_data))

# Automatic strategy selection
compressor.auto_select_strategy('text')
print(compressor.compress_file("readme.txt", test_data))
```

## Factory Pattern 🏭

The Factory pattern creates objects without specifying their exact classes. It's like ordering from a menu - you ask for "chicken sandwich" and get the right sandwich without knowing exactly how it's made.

```csharp
// Product hierarchy
public abstract class Vehicle
{
    public string Brand { get; protected set; }
    public string Model { get; protected set; }
    public abstract void Start();
    public abstract void Stop();
    public abstract string GetVehicleInfo();
    
    public virtual void Honk() => Console.WriteLine("🔊 Beep beep!");
}

public class Car : Vehicle
{
    public int Doors { get; private set; }
    
    public Car(string brand, string model, int doors = 4)
    {
        Brand = brand;
        Model = model;
        Doors = doors;
    }
    
    public override void Start()
    {
        Console.WriteLine($"🚗 {Brand} {Model} engine started! Vroom!");
    }
    
    public override void Stop()
    {
        Console.WriteLine($"🚗 {Brand} {Model} engine stopped.");
    }
    
    public override string GetVehicleInfo()
    {
        return $"Car: {Brand} {Model} ({Doors} doors)";
    }
}

public class Motorcycle : Vehicle
{
    public int EngineCC { get; private set; }
    
    public Motorcycle(string brand, string model, int engineCC)
    {
        Brand = brand;
        Model = model;
        EngineCC = engineCC;
    }
    
    public override void Start()
    {
        Console.WriteLine($"🏍️ {Brand} {Model} engine roared to life! VROOOOM!");
    }
    
    public override void Stop()
    {
        Console.WriteLine($"🏍️ {Brand} {Model} engine stopped with a rumble.");
    }
    
    public override string GetVehicleInfo()
    {
        return $"Motorcycle: {Brand} {Model} ({EngineCC}cc)";
    }
    
    public override void Honk() => Console.WriteLine("🔊 BEEP BEEP! (motorcycle horn)");
}

public class Truck : Vehicle
{
    public int LoadCapacity { get; private set; }
    
    public Truck(string brand, string model, int loadCapacity)
    {
        Brand = brand;
        Model = model;
        LoadCapacity = loadCapacity;
    }
    
    public override void Start()
    {
        Console.WriteLine($"🚛 {Brand} {Model} diesel engine started! RUMBLE!");
    }
    
    public override void Stop()
    {
        Console.WriteLine($"🚛 {Brand} {Model} engine stopped with air brake hiss.");
    }
    
    public override string GetVehicleInfo()
    {
        return $"Truck: {Brand} {Model} (Load capacity: {LoadCapacity} tons)";
    }
    
    public override void Honk() => Console.WriteLine("🔊 HOOOOOONK! (truck horn)");
}

// Factory interface
public interface IVehicleFactory
{
    Vehicle CreateVehicle(string model, Dictionary<string, object> specifications);
    List<string> GetAvailableModels();
}

// Concrete factories
public class CarFactory : IVehicleFactory
{
    private readonly Dictionary<string, Func<Dictionary<string, object>, Vehicle>> carModels;
    
    public CarFactory()
    {
        carModels = new Dictionary<string, Func<Dictionary<string, object>, Vehicle>>
        {
            ["Sedan"] = specs => new Car("Toyota", "Camry", (int)(specs.GetValueOrDefault("doors", 4))),
            ["SUV"] = specs => new Car("Honda", "CR-V", (int)(specs.GetValueOrDefault("doors", 4))),
            ["Hatchback"] = specs => new Car("Volkswagen", "Golf", (int)(specs.GetValueOrDefault("doors", 4))),
            ["Sports"] = specs => new Car("BMW", "M3", 2)
        };
    }
    
    public Vehicle CreateVehicle(string model, Dictionary<string, object> specifications)
    {
        if (carModels.ContainsKey(model))
        {
            return carModels[model](specifications);
        }
        throw new ArgumentException($"Car model '{model}' not available");
    }
    
    public List<string> GetAvailableModels() => carModels.Keys.ToList();
}

public class MotorcycleFactory : IVehicleFactory
{
    private readonly Dictionary<string, Func<Dictionary<string, object>, Vehicle>> motorcycleModels;
    
    public MotorcycleFactory()
    {
        motorcycleModels = new Dictionary<string, Func<Dictionary<string, object>, Vehicle>>
        {
            ["Sport"] = specs => new Motorcycle("Yamaha", "R1", (int)(specs.GetValueOrDefault("engineCC", 1000))),
            ["Cruiser"] = specs => new Motorcycle("Harley-Davidson", "Street 750", (int)(specs.GetValueOrDefault("engineCC", 750))),
            ["Touring"] = specs => new Motorcycle("BMW", "R1250GS", (int)(specs.GetValueOrDefault("engineCC", 1250))),
            ["Dirt"] = specs => new Motorcycle("KTM", "250 SX-F", (int)(specs.GetValueOrDefault("engineCC", 250)))
        };
    }
    
    public Vehicle CreateVehicle(string model, Dictionary<string, object> specifications)
    {
        if (motorcycleModels.ContainsKey(model))
        {
            return motorcycleModels[model](specifications);
        }
        throw new ArgumentException($"Motorcycle model '{model}' not available");
    }
    
    public List<string> GetAvailableModels() => motorcycleModels.Keys.ToList();
}

// Abstract factory for creating families of vehicles
public class VehicleManufacturer
{
    private readonly Dictionary<string, IVehicleFactory> factories;
    
    public VehicleManufacturer()
    {
        factories = new Dictionary<string, IVehicleFactory>
        {
            ["Car"] = new CarFactory(),
            ["Motorcycle"] = new MotorcycleFactory()
        };
    }
    
    public Vehicle CreateVehicle(string vehicleType, string model, Dictionary<string, object> specifications = null)
    {
        specifications ??= new Dictionary<string, object>();
        
        if (factories.ContainsKey(vehicleType))
        {
            return factories[vehicleType].CreateVehicle(model, specifications);
        }
        
        throw new ArgumentException($"Vehicle type '{vehicleType}' not supported");
    }
    
    public void ShowAvailableVehicles()
    {
        Console.WriteLine("🏭 Available Vehicles:");
        foreach (var (vehicleType, factory) in factories)
        {
            Console.WriteLine($"\n{vehicleType}s:");
            foreach (var model in factory.GetAvailableModels())
            {
                Console.WriteLine($"  - {model}");
            }
        }
    }
    
    public List<Vehicle> CreateFleet(int count)
    {
        var fleet = new List<Vehicle>();
        var random = new Random();
        var vehicleTypes = factories.Keys.ToArray();
        
        for (int i = 0; i < count; i++)
        {
            var vehicleType = vehicleTypes[random.Next(vehicleTypes.Length)];
            var availableModels = factories[vehicleType].GetAvailableModels();
            var model = availableModels[random.Next(availableModels.Count)];
            
            var vehicle = CreateVehicle(vehicleType, model);
            fleet.Add(vehicle);
        }
        
        return fleet;
    }
}

// Usage example
public class FactoryDemo
{
    public static void RunDemo()
    {
        var manufacturer = new VehicleManufacturer();
        
        // Show available vehicles
        manufacturer.ShowAvailableVehicles();
        
        Console.WriteLine("\n🚗 Creating specific vehicles:");
        
        // Create specific vehicles
        var sedan = manufacturer.CreateVehicle("Car", "Sedan", new Dictionary<string, object> { ["doors"] = 4 });
        var sportsBike = manufacturer.CreateVehicle("Motorcycle", "Sport", new Dictionary<string, object> { ["engineCC"] = 1000 });
        
        // Test the vehicles
        TestVehicle(sedan);
        TestVehicle(sportsBike);
        
        Console.WriteLine("\n🚛 Creating a random fleet:");
        var fleet = manufacturer.CreateFleet(5);
        
        foreach (var vehicle in fleet)
        {
            Console.WriteLine($"Fleet vehicle: {vehicle.GetVehicleInfo()}");
        }
    }
    
    private static void TestVehicle(Vehicle vehicle)
    {
        Console.WriteLine($"\n🔧 Testing: {vehicle.GetVehicleInfo()}");
        vehicle.Start();
        vehicle.Honk();
        vehicle.Stop();
    }
}
```

## Decorator Pattern 🎨

The Decorator pattern adds new functionality to objects without changing their structure. It's like adding toppings to a pizza - each topping adds something new without changing the base pizza.

```csharp
// Component interface
public interface INotification
{
    string Send(string message);
    decimal GetCost();
}

// Base implementation
public class EmailNotification : INotification
{
    private string emailAddress;
    
    public EmailNotification(string emailAddress)
    {
        this.emailAddress = emailAddress;
    }
    
    public virtual string Send(string message)
    {
        return $"📧 Email sent to {emailAddress}: {message}";
    }
    
    public virtual decimal GetCost()
    {
        return 0.01m; // $0.01 per email
    }
}

// Base decorator
public abstract class NotificationDecorator : INotification
{
    protected INotification notification;
    
    public NotificationDecorator(INotification notification)
    {
        this.notification = notification;
    }
    
    public virtual string Send(string message)
    {
        return notification.Send(message);
    }
    
    public virtual decimal GetCost()
    {
        return notification.GetCost();
    }
}

// Concrete decorators
public class SMSDecorator : NotificationDecorator
{
    private string phoneNumber;
    
    public SMSDecorator(INotification notification, string phoneNumber) 
        : base(notification)
    {
        this.phoneNumber = phoneNumber;
    }
    
    public override string Send(string message)
    {
        var result = base.Send(message);
        result += $"\n📱 SMS sent to {phoneNumber}: {message}";
        return result;
    }
    
    public override decimal GetCost()
    {
        return base.GetCost() + 0.05m; // Additional $0.05 for SMS
    }
}

public class PushNotificationDecorator : NotificationDecorator
{
    private string deviceId;
    
    public PushNotificationDecorator(INotification notification, string deviceId) 
        : base(notification)
    {
        this.deviceId = deviceId;
    }
    
    public override string Send(string message)
    {
        var result = base.Send(message);
        result += $"\n🔔 Push notification sent to device {deviceId}: {message}";
        return result;
    }
    
    public override decimal GetCost()
    {
        return base.GetCost() + 0.02m; // Additional $0.02 for push notification
    }
}

public class SlackDecorator : NotificationDecorator
{
    private string slackChannel;
    
    public SlackDecorator(INotification notification, string slackChannel) 
        : base(notification)
    {
        this.slackChannel = slackChannel;
    }
    
    public override string Send(string message)
    {
        var result = base.Send(message);
        result += $"\n💬 Slack message sent to #{slackChannel}: {message}";
        return result;
    }
    
    public override decimal GetCost()
    {
        return base.GetCost() + 0.03m; // Additional $0.03 for Slack
    }
}

public class EncryptionDecorator : NotificationDecorator
{
    public EncryptionDecorator(INotification notification) : base(notification) { }
    
    public override string Send(string message)
    {
        var encryptedMessage = EncryptMessage(message);
        var result = base.Send(encryptedMessage);
        return "🔐 Encryption applied\n" + result;
    }
    
    public override decimal GetCost()
    {
        return base.GetCost() + 0.01m; // Additional $0.01 for encryption
    }
    
    private string EncryptMessage(string message)
    {
        // Simple encryption simulation
        var bytes = System.Text.Encoding.UTF8.GetBytes(message);
        var encrypted = Convert.ToBase64String(bytes);
        return $"ENCRYPTED[{encrypted[..Math.Min(20, encrypted.Length)]}...]";
    }
}

public class PriorityDecorator : NotificationDecorator
{
    private string priority;
    
    public PriorityDecorator(INotification notification, string priority) 
        : base(notification)
    {
        this.priority = priority.ToUpper();
    }
    
    public override string Send(string message)
    {
        var priorityMessage = $"[{priority}] {message}";
        var result = base.Send(priorityMessage);
        return $"⚡ Priority level: {priority}\n" + result;
    }
    
    public override decimal GetCost()
    {
        var multiplier = priority switch
        {
            "HIGH" => 2.0m,
            "MEDIUM" => 1.5m,
            "LOW" => 1.0m,
            _ => 1.0m
        };
        return base.GetCost() * multiplier;
    }
}

// Usage example
public class DecoratorDemo
{
    public static void RunDemo()
    {
        Console.WriteLine("🎨 Decorator Pattern - Notification System\n");
        
        // Start with basic email notification
        INotification notification = new EmailNotification("user@example.com");
        
        Console.WriteLine("📧 Basic email notification:");
        Console.WriteLine(notification.Send("Welcome to our service!"));
        Console.WriteLine($"Cost: ${notification.GetCost():F3}\n");
        
        // Add SMS capability
        notification = new SMSDecorator(notification, "+1-555-0123");
        Console.WriteLine("📧📱 Email + SMS notification:");
        Console.WriteLine(notification.Send("Your order has been shipped!"));
        Console.WriteLine($"Cost: ${notification.GetCost():F3}\n");
        
        // Add push notification
        notification = new PushNotificationDecorator(notification, "device_abc123");
        Console.WriteLine("📧📱🔔 Email + SMS + Push notification:");
        Console.WriteLine(notification.Send("Flash sale starting in 1 hour!"));
        Console.WriteLine($"Cost: ${notification.GetCost():F3}\n");
        
        // Add Slack integration
        notification = new SlackDecorator(notification, "general");
        Console.WriteLine("📧📱🔔💬 Email + SMS + Push + Slack notification:");
        Console.WriteLine(notification.Send("System maintenance scheduled"));
        Console.WriteLine($"Cost: ${notification.GetCost():F3}\n");
        
        // Add encryption
        notification = new EncryptionDecorator(notification);
        Console.WriteLine("🔐📧📱🔔💬 Encrypted multi-channel notification:");
        Console.WriteLine(notification.Send("Sensitive security alert"));
        Console.WriteLine($"Cost: ${notification.GetCost():F3}\n");
        
        // Add high priority
        notification = new PriorityDecorator(notification, "HIGH");
        Console.WriteLine("⚡🔐📧📱🔔💬 High priority encrypted multi-channel notification:");
        Console.WriteLine(notification.Send("URGENT: Security breach detected"));
        Console.WriteLine($"Cost: ${notification.GetCost():F3}\n");
        
        // Demonstrate different combinations
        Console.WriteLine("🔄 Different decoration combinations:\n");
        
        // Simple encrypted email
        var simpleEncrypted = new EncryptionDecorator(
            new EmailNotification("admin@company.com")
        );
        Console.WriteLine("Simple encrypted email:");
        Console.WriteLine(simpleEncrypted.Send("Confidential report"));
        Console.WriteLine($"Cost: ${simpleEncrypted.GetCost():F3}\n");
        
        // High priority SMS only
        var urgentSMS = new PriorityDecorator(
            new SMSDecorator(
                new EmailNotification("manager@company.com"), 
                "+1-555-9999"
            ), 
            "HIGH"
        );
        Console.WriteLine("High priority SMS:");
        Console.WriteLine(urgentSMS.Send("Server is down!"));
        Console.WriteLine($"Cost: ${urgentSMS.GetCost():F3}");
    }
}
```

## Real-World Pattern Combinations 🌍

Here's how these patterns work together in a real application:

```csharp
// Combining Observer + Strategy + Factory patterns
public class OrderProcessingSystem
{
    private IPaymentStrategy paymentStrategy;
    private List<IOrderObserver> observers = new List<IOrderObserver>();
    private INotificationFactory notificationFactory;
    
    public OrderProcessingSystem()
    {
        notificationFactory = new NotificationFactory();
    }
    
    public void AddObserver(IOrderObserver observer) => observers.Add(observer);
    
    public void SetPaymentStrategy(IPaymentStrategy strategy) => paymentStrategy = strategy;
    
    public void ProcessOrder(Order order)
    {
        // Use Strategy pattern for payment processing
        var paymentResult = paymentStrategy.ProcessPayment(order.Total, "MERCHANT_123");
        
        if (paymentResult.IsSuccessful)
        {
            order.Status = "Paid";
            
            // Use Factory pattern to create appropriate notifications
            var notification = notificationFactory.CreateNotification(order.Customer.PreferredContactMethod);
            
            // Use Decorator pattern to enhance notifications
            notification = new EncryptionDecorator(notification);
            if (order.Total > 100)
                notification = new PriorityDecorator(notification, "HIGH");
            
            // Use Observer pattern to notify all interested parties
            NotifyObservers(order, notification);
        }
    }
    
    private void NotifyObservers(Order order, INotification notification)
    {
        foreach (var observer in observers)
        {
            observer.OnOrderProcessed(order, notification);
        }
    }
}

public interface IOrderObserver
{
    void OnOrderProcessed(Order order, INotification notification);
}

public class InventoryObserver : IOrderObserver
{
    public void OnOrderProcessed(Order order, INotification notification)
    {
        Console.WriteLine($"📦 Inventory: Reducing stock for order {order.Id}");
        // Update inventory logic here
    }
}

public class CustomerServiceObserver : IOrderObserver
{
    public void OnOrderProcessed(Order order, INotification notification)
    {
        var message = $"Order {order.Id} confirmed. Thank you for your purchase!";
        Console.WriteLine($"📞 Customer Service: {notification.Send(message)}");
    }
}

public class Order
{
    public string Id { get; set; }
    public decimal Total { get; set; }
    public string Status { get; set; }
    public Customer Customer { get; set; }
}

public class Customer
{
    public string Name { get; set; }
    public string Email { get; set; }
    public string Phone { get; set; }
    public string PreferredContactMethod { get; set; }
}

public interface INotificationFactory
{
    INotification CreateNotification(string type);
}

public class NotificationFactory : INotificationFactory
{
    public INotification CreateNotification(string type)
    {
        return type.ToLower() switch
        {
            "email" => new EmailNotification("customer@email.com"),
            "sms" => new SMSDecorator(new EmailNotification("customer@email.com"), "+1-555-0123"),
            "both" => new PushNotificationDecorator(
                new SMSDecorator(new EmailNotification("customer@email.com"), "+1-555-0123"), 
                "device123"),
            _ => new EmailNotification("customer@email.com")
        };
    }
}
```

## Key Takeaways 🎯

1. **Design patterns solve specific problems** - Don't use them just because you can. Use them when you actually have the problem they solve.

2. **SOLID principles guide good patterns** - Well-designed patterns naturally follow SOLID principles:
   - **SRP**: Each class has one clear responsibility
   - **OCP**: Easy to extend without modifying existing code
   - **LSP**: Substitutable implementations work correctly
   - **ISP**: Focused interfaces for specific needs
   - **DIP**: Depend on abstractions, not concrete classes

3. **Patterns work together** - Real applications often combine multiple patterns for robust solutions.

4. **Start simple, refactor to patterns** - Begin with straightforward code and introduce patterns as complexity grows.

5. **Understand the intent** - Knowing why a pattern exists is more valuable than memorizing its structure.

6. **Choose the right tool** - Each pattern solves specific problems. Use the appropriate pattern for your situation.

Remember: Design patterns are tools in your programming toolbox. Like any tool, they're most effective when used for their intended purpose! 🔧✨

## Exercises

1. **Easy**: Implement a simple Factory that creates different types of vehicles (Car, Bike, Truck).
   - *Hint: Create a base Vehicle class and specific implementations*

2. **Medium**: Create an Observer pattern for a simple newsletter subscription system.
   - *Hint: Newsletter is the subject, subscribers are observers*

3. **Hard**: Combine Factory and Strategy patterns to create a payment processing system that can handle different payment methods.
   - *Hint: Factory creates payment processors, Strategy handles different algorithms*

## Chapter Checklist

After completing this chapter, you should be able to:

- [ ] Explain what design patterns are and why they're useful
- [ ] Identify when common patterns should be applied
- [ ] Implement Observer pattern for notifications
- [ ] Implement Strategy pattern for interchangeable algorithms
- [ ] Implement Factory pattern for object creation
- [ ] Implement Decorator pattern for extending functionality
- [ ] Recognize the difference between patterns and specific implementations
- [ ] Understand that patterns are tools, not rules
- [ ] Avoid "pattern fever" - using patterns unnecessarily
- [ ] Combine multiple patterns effectively
- [ ] Choose appropriate patterns for specific problems