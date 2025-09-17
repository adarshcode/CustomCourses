# Chapter 12: SOLID in Practice - Complete System 🏗️

## Child-Friendly Explanation 🧸
Remember learning to ride a bike? First you learned about balance, then pedaling, then steering, then braking - each skill separately. But to actually ride a bike, you need to use ALL these skills together at the same time! SOLID principles are like this - we've learned each principle separately, but now we're going to build a complete program that uses ALL the principles working together to create something amazing!

## Developer-Level Explanation 👨‍💻
This chapter demonstrates how all five SOLID principles work together in a realistic application. We'll build a complete task management system that showcases how SRP, OCP, LSP, ISP, and DIP combine to create maintainable, extensible, and robust software.

Let's build a real-world e-commerce order processing system that demonstrates all SOLID principles working together in harmony!

## Complete E-Commerce Order System 🛒

### System Requirements
- Process different types of orders (physical goods, digital products, subscriptions)
- Support multiple payment methods (credit card, PayPal, cryptocurrency)
- Send notifications via various channels (email, SMS, push notifications)
- Generate different types of reports (sales, inventory, customer)
- Handle different shipping methods
- Apply various discount strategies

### Core Domain Models

```csharp
// Domain entities - pure data with business rules
public class Order
{
    public string Id { get; private set; }
    public string CustomerId { get; private set; }
    public List<OrderItem> Items { get; private set; }
    public OrderStatus Status { get; private set; }
    public DateTime CreatedAt { get; private set; }
    public decimal TotalAmount => Items.Sum(item => item.TotalPrice);
    
    public Order(string customerId)
    {
        Id = Guid.NewGuid().ToString();
        CustomerId = customerId;
        Items = new List<OrderItem>();
        Status = OrderStatus.Pending;
        CreatedAt = DateTime.UtcNow;
    }
    
    public void AddItem(OrderItem item)
    {
        Items.Add(item);
    }
    
    public void UpdateStatus(OrderStatus newStatus)
    {
        Status = newStatus;
    }
}

public class OrderItem
{
    public string ProductId { get; private set; }
    public string ProductName { get; private set; }
    public decimal UnitPrice { get; private set; }
    public int Quantity { get; private set; }
    public decimal TotalPrice => UnitPrice * Quantity;
    
    public OrderItem(string productId, string productName, decimal unitPrice, int quantity)
    {
        ProductId = productId;
        ProductName = productName;
        UnitPrice = unitPrice;
        Quantity = quantity;
    }
}

public enum OrderStatus
{
    Pending,
    Paid,
    Processing,
    Shipped,
    Delivered,
    Cancelled
}

public class Customer
{
    public string Id { get; private set; }
    public string Name { get; private set; }
    public string Email { get; private set; }
    public string Phone { get; private set; }
    public CustomerType Type { get; private set; }
    
    public Customer(string name, string email, string phone, CustomerType type)
    {
        Id = Guid.NewGuid().ToString();
        Name = name;
        Email = email;
        Phone = phone;
        Type = type;
    }
}

public enum CustomerType
{
    Regular,
    Premium,
    VIP
}
```

## SRP: Single Responsibility Principle 📋

Each class has one clear responsibility:

```csharp
// SRP: Only responsible for order-related business logic
public class OrderService
{
    private readonly IOrderRepository orderRepository;
    private readonly IPaymentProcessor paymentProcessor;
    private readonly INotificationService notificationService;
    private readonly IDiscountCalculator discountCalculator;
    
    public OrderService(
        IOrderRepository orderRepository,
        IPaymentProcessor paymentProcessor,
        INotificationService notificationService,
        IDiscountCalculator discountCalculator)
    {
        this.orderRepository = orderRepository;
        this.paymentProcessor = paymentProcessor;
        this.notificationService = notificationService;
        this.discountCalculator = discountCalculator;
    }
    
    public async Task<OrderResult> ProcessOrderAsync(Order order, PaymentInfo paymentInfo)
    {
        try
        {
            // Apply discounts
            var discount = discountCalculator.CalculateDiscount(order);
            var finalAmount = order.TotalAmount - discount;
            
            // Process payment
            var paymentResult = await paymentProcessor.ProcessPaymentAsync(finalAmount, paymentInfo);
            
            if (paymentResult.IsSuccessful)
            {
                order.UpdateStatus(OrderStatus.Paid);
                await orderRepository.SaveAsync(order);
                
                // Send confirmation
                await notificationService.SendOrderConfirmationAsync(order);
                
                return OrderResult.Success(order, paymentResult);
            }
            
            return OrderResult.Failure("Payment failed: " + paymentResult.ErrorMessage);
        }
        catch (Exception ex)
        {
            return OrderResult.Failure($"Order processing failed: {ex.Message}");
        }
    }
}

// SRP: Only responsible for discount calculations
public class DiscountCalculator : IDiscountCalculator
{
    private readonly List<IDiscountStrategy> discountStrategies;
    
    public DiscountCalculator(List<IDiscountStrategy> strategies)
    {
        this.discountStrategies = strategies;
    }
    
    public decimal CalculateDiscount(Order order)
    {
        decimal totalDiscount = 0;
        
        foreach (var strategy in discountStrategies)
        {
            if (strategy.IsApplicable(order))
            {
                totalDiscount += strategy.CalculateDiscount(order);
            }
        }
        
        return totalDiscount;
    }
}

// SRP: Only responsible for inventory management
public class InventoryService
{
    private readonly IInventoryRepository inventoryRepository;
    private readonly INotificationService notificationService;
    
    public InventoryService(IInventoryRepository inventoryRepository, INotificationService notificationService)
    {
        this.inventoryRepository = inventoryRepository;
        this.notificationService = notificationService;
    }
    
    public async Task<bool> ReserveItemsAsync(Order order)
    {
        foreach (var item in order.Items)
        {
            var available = await inventoryRepository.GetAvailableQuantityAsync(item.ProductId);
            
            if (available < item.Quantity)
            {
                await notificationService.SendLowStockAlertAsync(item.ProductId, available);
                return false;
            }
            
            await inventoryRepository.ReserveAsync(item.ProductId, item.Quantity);
        }
        
        return true;
    }
}
```

## OCP: Open/Closed Principle 🔓

System is open for extension, closed for modification:

```csharp
// OCP: Can add new payment methods without modifying existing code
public interface IPaymentProcessor
{
    Task<PaymentResult> ProcessPaymentAsync(decimal amount, PaymentInfo paymentInfo);
    string GetPaymentMethodName();
}

public class CreditCardProcessor : IPaymentProcessor
{
    public async Task<PaymentResult> ProcessPaymentAsync(decimal amount, PaymentInfo paymentInfo)
    {
        // Simulate credit card processing
        await Task.Delay(1000); // Simulate API call
        
        if (paymentInfo.CreditCard.IsValid())
        {
            var transactionId = $"CC_{Guid.NewGuid().ToString()[..8]}";
            return PaymentResult.Success(transactionId, $"Charged ${amount:F2} to card ending in {paymentInfo.CreditCard.LastFourDigits}");
        }
        
        return PaymentResult.Failure("Invalid credit card information");
    }
    
    public string GetPaymentMethodName() => "Credit Card";
}

public class PayPalProcessor : IPaymentProcessor
{
    public async Task<PaymentResult> ProcessPaymentAsync(decimal amount, PaymentInfo paymentInfo)
    {
        // Simulate PayPal processing
        await Task.Delay(800);
        
        var transactionId = $"PP_{Guid.NewGuid().ToString()[..8]}";
        return PaymentResult.Success(transactionId, $"PayPal payment of ${amount:F2} processed for {paymentInfo.PayPalEmail}");
    }
    
    public string GetPaymentMethodName() => "PayPal";
}

// NEW: Adding cryptocurrency without modifying existing code
public class CryptocurrencyProcessor : IPaymentProcessor
{
    public async Task<PaymentResult> ProcessPaymentAsync(decimal amount, PaymentInfo paymentInfo)
    {
        // Simulate crypto processing
        await Task.Delay(2000); // Crypto takes longer
        
        var transactionId = $"CRYPTO_{Guid.NewGuid().ToString()[..8]}";
        return PaymentResult.Success(transactionId, $"Cryptocurrency payment of ${amount:F2} processed to wallet {paymentInfo.CryptoWallet}");
    }
    
    public string GetPaymentMethodName() => "Cryptocurrency";
}

// OCP: Can add new discount strategies without modifying existing code
public interface IDiscountStrategy
{
    bool IsApplicable(Order order);
    decimal CalculateDiscount(Order order);
    string GetDescription();
}

public class VolumeDiscountStrategy : IDiscountStrategy
{
    public bool IsApplicable(Order order) => order.Items.Sum(i => i.Quantity) >= 10;
    
    public decimal CalculateDiscount(Order order)
    {
        var totalItems = order.Items.Sum(i => i.Quantity);
        return totalItems >= 20 ? order.TotalAmount * 0.15m : order.TotalAmount * 0.10m;
    }
    
    public string GetDescription() => "Volume discount: 10% for 10+ items, 15% for 20+ items";
}

public class CustomerLoyaltyDiscountStrategy : IDiscountStrategy
{
    private readonly ICustomerRepository customerRepository;
    
    public CustomerLoyaltyDiscountStrategy(ICustomerRepository customerRepository)
    {
        this.customerRepository = customerRepository;
    }
    
    public bool IsApplicable(Order order)
    {
        var customer = customerRepository.GetByIdAsync(order.CustomerId).Result;
        return customer.Type == CustomerType.Premium || customer.Type == CustomerType.VIP;
    }
    
    public decimal CalculateDiscount(Order order)
    {
        var customer = customerRepository.GetByIdAsync(order.CustomerId).Result;
        return customer.Type switch
        {
            CustomerType.Premium => order.TotalAmount * 0.05m,
            CustomerType.VIP => order.TotalAmount * 0.10m,
            _ => 0m
        };
    }
    
    public string GetDescription() => "Customer loyalty: 5% Premium, 10% VIP";
}

// NEW: Adding seasonal discount without modifying existing code
public class SeasonalDiscountStrategy : IDiscountStrategy
{
    public bool IsApplicable(Order order) => IsHolidaySeason();
    
    public decimal CalculateDiscount(Order order) => order.TotalAmount * 0.08m;
    
    public string GetDescription() => "Holiday season special: 8% off";
    
    private bool IsHolidaySeason()
    {
        var now = DateTime.Now;
        return (now.Month == 11 && now.Day >= 20) || (now.Month == 12 && now.Day <= 31);
    }
}
```

## LSP: Liskov Substitution Principle ⚖️

All implementations can be substituted for their interfaces:

```csharp
// LSP: All notification services follow the same contract
public interface INotificationService
{
    Task SendOrderConfirmationAsync(Order order);
    Task SendShippingNotificationAsync(Order order, string trackingNumber);
    Task SendLowStockAlertAsync(string productId, int currentStock);
}

public class EmailNotificationService : INotificationService
{
    private readonly IEmailSender emailSender;
    private readonly ICustomerRepository customerRepository;
    
    public EmailNotificationService(IEmailSender emailSender, ICustomerRepository customerRepository)
    {
        this.emailSender = emailSender;
        this.customerRepository = customerRepository;
    }
    
    public async Task SendOrderConfirmationAsync(Order order)
    {
        var customer = await customerRepository.GetByIdAsync(order.CustomerId);
        var subject = $"Order Confirmation - #{order.Id}";
        var body = $"Dear {customer.Name}, your order for ${order.TotalAmount:F2} has been confirmed.";
        
        await emailSender.SendAsync(customer.Email, subject, body);
        Console.WriteLine($"📧 Email sent to {customer.Email}: {subject}");
    }
    
    public async Task SendShippingNotificationAsync(Order order, string trackingNumber)
    {
        var customer = await customerRepository.GetByIdAsync(order.CustomerId);
        var subject = $"Your order #{order.Id} has shipped!";
        var body = $"Dear {customer.Name}, your order has shipped. Tracking: {trackingNumber}";
        
        await emailSender.SendAsync(customer.Email, subject, body);
        Console.WriteLine($"📧 Shipping email sent to {customer.Email}");
    }
    
    public async Task SendLowStockAlertAsync(string productId, int currentStock)
    {
        var subject = "Low Stock Alert";
        var body = $"Product {productId} is low in stock. Current quantity: {currentStock}";
        
        await emailSender.SendAsync("inventory@company.com", subject, body);
        Console.WriteLine($"📧 Low stock alert sent for product {productId}");
    }
}

public class SMSNotificationService : INotificationService
{
    private readonly ISMSSender smsSender;
    private readonly ICustomerRepository customerRepository;
    
    public SMSNotificationService(ISMSSender smsSender, ICustomerRepository customerRepository)
    {
        this.smsSender = smsSender;
        this.customerRepository = customerRepository;
    }
    
    public async Task SendOrderConfirmationAsync(Order order)
    {
        var customer = await customerRepository.GetByIdAsync(order.CustomerId);
        var message = $"Order #{order.Id} confirmed! Total: ${order.TotalAmount:F2}. Thank you!";
        
        await smsSender.SendAsync(customer.Phone, message);
        Console.WriteLine($"📱 SMS sent to {customer.Phone}: Order confirmed");
    }
    
    public async Task SendShippingNotificationAsync(Order order, string trackingNumber)
    {
        var customer = await customerRepository.GetByIdAsync(order.CustomerId);
        var message = $"Order #{order.Id} shipped! Track: {trackingNumber}";
        
        await smsSender.SendAsync(customer.Phone, message);
        Console.WriteLine($"📱 SMS sent to {customer.Phone}: Shipment notification");
    }
    
    public async Task SendLowStockAlertAsync(string productId, int currentStock)
    {
        var message = $"ALERT: Product {productId} low stock: {currentStock} remaining";
        
        await smsSender.SendAsync("+1-555-INVENTORY", message);
        Console.WriteLine($"📱 Low stock SMS alert sent");
    }
}

// LSP: Multi-channel service that combines multiple notification methods
public class MultiChannelNotificationService : INotificationService
{
    private readonly List<INotificationService> notificationServices;
    
    public MultiChannelNotificationService(params INotificationService[] services)
    {
        this.notificationServices = services.ToList();
    }
    
    public async Task SendOrderConfirmationAsync(Order order)
    {
        var tasks = notificationServices.Select(service => service.SendOrderConfirmationAsync(order));
        await Task.WhenAll(tasks);
    }
    
    public async Task SendShippingNotificationAsync(Order order, string trackingNumber)
    {
        var tasks = notificationServices.Select(service => service.SendShippingNotificationAsync(order, trackingNumber));
        await Task.WhenAll(tasks);
    }
    
    public async Task SendLowStockAlertAsync(string productId, int currentStock)
    {
        var tasks = notificationServices.Select(service => service.SendLowStockAlertAsync(productId, currentStock));
        await Task.WhenAll(tasks);
    }
}

// Demonstration that all implementations are substitutable
public class NotificationTester
{
    public static async Task TestAllNotificationServices(Order testOrder)
    {
        var services = new List<INotificationService>
        {
            new EmailNotificationService(new MockEmailSender(), new MockCustomerRepository()),
            new SMSNotificationService(new MockSMSSender(), new MockCustomerRepository()),
            new MultiChannelNotificationService(
                new EmailNotificationService(new MockEmailSender(), new MockCustomerRepository()),
                new SMSNotificationService(new MockSMSSender(), new MockCustomerRepository())
            )
        };
        
        foreach (var service in services)
        {
            Console.WriteLine($"\n🔄 Testing {service.GetType().Name}:");
            await service.SendOrderConfirmationAsync(testOrder);
            await service.SendShippingNotificationAsync(testOrder, "TRACK123456");
        }
    }
}
```

## ISP: Interface Segregation Principle 🎯

Interfaces are focused and clients only depend on what they need:

```csharp
// ISP: Segregated interfaces - clients only depend on what they need

// Order reading operations
public interface IOrderReader
{
    Task<Order> GetByIdAsync(string orderId);
    Task<List<Order>> GetByCustomerIdAsync(string customerId);
    Task<List<Order>> GetByStatusAsync(OrderStatus status);
}

// Order writing operations
public interface IOrderWriter
{
    Task SaveAsync(Order order);
    Task UpdateStatusAsync(string orderId, OrderStatus status);
    Task DeleteAsync(string orderId);
}

// Complete order repository (implements both when needed)
public interface IOrderRepository : IOrderReader, IOrderWriter
{
}

// Analytics only needs reading
public interface IOrderAnalytics
{
    Task<decimal> GetTotalSalesAsync(DateTime from, DateTime to);
    Task<Dictionary<string, int>> GetTopProductsAsync(int count);
    Task<List<Order>> GetHighValueOrdersAsync(decimal minimumAmount);
}

// Reporting interfaces are segregated by concern
public interface IReportGenerator
{
    Task<string> GenerateReportAsync();
    string GetReportFormat();
}

public interface ISalesReportData
{
    Task<decimal> GetTotalSalesAsync(DateTime from, DateTime to);
    Task<int> GetOrderCountAsync(DateTime from, DateTime to);
    Task<decimal> GetAverageOrderValueAsync(DateTime from, DateTime to);
}

public interface IInventoryReportData
{
    Task<List<ProductStock>> GetLowStockProductsAsync();
    Task<List<ProductStock>> GetOutOfStockProductsAsync();
    Task<Dictionary<string, int>> GetTopSellingProductsAsync(int days);
}

// Concrete implementations focus on their specific responsibilities
public class SalesReportGenerator : IReportGenerator
{
    private readonly ISalesReportData salesData;
    
    public SalesReportGenerator(ISalesReportData salesData)
    {
        this.salesData = salesData;
    }
    
    public async Task<string> GenerateReportAsync()
    {
        var endDate = DateTime.UtcNow;
        var startDate = endDate.AddDays(-30);
        
        var totalSales = await salesData.GetTotalSalesAsync(startDate, endDate);
        var orderCount = await salesData.GetOrderCountAsync(startDate, endDate);
        var averageValue = await salesData.GetAverageOrderValueAsync(startDate, endDate);
        
        return $"📊 Sales Report (Last 30 Days)\n" +
               $"Total Sales: ${totalSales:F2}\n" +
               $"Total Orders: {orderCount}\n" +
               $"Average Order Value: ${averageValue:F2}";
    }
    
    public string GetReportFormat() => "Sales Summary";
}

public class InventoryReportGenerator : IReportGenerator
{
    private readonly IInventoryReportData inventoryData;
    
    public InventoryReportGenerator(IInventoryReportData inventoryData)
    {
        this.inventoryData = inventoryData;
    }
    
    public async Task<string> GenerateReportAsync()
    {
        var lowStock = await inventoryData.GetLowStockProductsAsync();
        var outOfStock = await inventoryData.GetOutOfStockProductsAsync();
        var topSelling = await inventoryData.GetTopSellingProductsAsync(7);
        
        var report = "📦 Inventory Report\n\n";
        
        report += "⚠️ Low Stock Items:\n";
        foreach (var item in lowStock)
            report += $"  - {item.ProductName}: {item.Quantity} remaining\n";
        
        report += "\n❌ Out of Stock Items:\n";
        foreach (var item in outOfStock)
            report += $"  - {item.ProductName}\n";
        
        report += "\n🔥 Top Selling (Last 7 Days):\n";
        foreach (var (product, quantity) in topSelling)
            report += $"  - {product}: {quantity} sold\n";
        
        return report;
    }
    
    public string GetReportFormat() => "Inventory Status";
}

// Payment processing is segregated by different concerns
public interface IPaymentValidator
{
    bool ValidatePaymentInfo(PaymentInfo paymentInfo);
    List<string> GetValidationErrors(PaymentInfo paymentInfo);
}

public interface IPaymentProcessor
{
    Task<PaymentResult> ProcessPaymentAsync(decimal amount, PaymentInfo paymentInfo);
    string GetPaymentMethodName();
}

public interface IPaymentLogger
{
    Task LogPaymentAttemptAsync(string orderId, decimal amount, string method);
    Task LogPaymentResultAsync(string orderId, PaymentResult result);
}

// Fraud detection only needs specific payment information
public interface IFraudDetector
{
    Task<bool> IsSuspiciousAsync(string customerId, decimal amount);
    Task<FraudRiskLevel> AssessRiskAsync(PaymentInfo paymentInfo);
}

public class PaymentService
{
    private readonly IPaymentValidator validator;
    private readonly IPaymentProcessor processor;
    private readonly IPaymentLogger logger;
    private readonly IFraudDetector fraudDetector;
    
    public PaymentService(
        IPaymentValidator validator,
        IPaymentProcessor processor,
        IPaymentLogger logger,
        IFraudDetector fraudDetector)
    {
        this.validator = validator;
        this.processor = processor;
        this.logger = logger;
        this.fraudDetector = fraudDetector;
    }
    
    public async Task<PaymentResult> ProcessOrderPaymentAsync(Order order, PaymentInfo paymentInfo)
    {
        // Each interface is used for its specific purpose
        
        // Validation
        if (!validator.ValidatePaymentInfo(paymentInfo))
        {
            var errors = validator.GetValidationErrors(paymentInfo);
            return PaymentResult.Failure($"Payment validation failed: {string.Join(", ", errors)}");
        }
        
        // Fraud detection
        var isSuspicious = await fraudDetector.IsSuspiciousAsync(order.CustomerId, order.TotalAmount);
        if (isSuspicious)
        {
            await logger.LogPaymentAttemptAsync(order.Id, order.TotalAmount, "FRAUD_DETECTED");
            return PaymentResult.Failure("Payment flagged for manual review");
        }
        
        // Processing
        await logger.LogPaymentAttemptAsync(order.Id, order.TotalAmount, processor.GetPaymentMethodName());
        var result = await processor.ProcessPaymentAsync(order.TotalAmount, paymentInfo);
        await logger.LogPaymentResultAsync(order.Id, result);
        
        return result;
    }
}
```

## DIP: Dependency Inversion Principle 🔄

High-level modules depend on abstractions, not concrete implementations:

```csharp
// DIP: High-level OrderManagementSystem depends on abstractions
public class OrderManagementSystem
{
    // All dependencies are abstractions (interfaces)
    private readonly IOrderRepository orderRepository;
    private readonly IPaymentProcessor paymentProcessor;
    private readonly INotificationService notificationService;
    private readonly IInventoryService inventoryService;
    private readonly IShippingService shippingService;
    private readonly IDiscountCalculator discountCalculator;
    
    // Constructor injection - depends on abstractions, not concretions
    public OrderManagementSystem(
        IOrderRepository orderRepository,
        IPaymentProcessor paymentProcessor,
        INotificationService notificationService,
        IInventoryService inventoryService,
        IShippingService shippingService,
        IDiscountCalculator discountCalculator)
    {
        this.orderRepository = orderRepository;
        this.paymentProcessor = paymentProcessor;
        this.notificationService = notificationService;
        this.inventoryService = inventoryService;
        this.shippingService = shippingService;
        this.discountCalculator = discountCalculator;
    }
    
    public async Task<OrderResult> ProcessCompleteOrderAsync(Order order, PaymentInfo paymentInfo)
    {
        try
        {
            Console.WriteLine($"🔄 Processing order {order.Id} for customer {order.CustomerId}");
            
            // Step 1: Check inventory availability
            var inventoryAvailable = await inventoryService.CheckAvailabilityAsync(order);
            if (!inventoryAvailable)
            {
                return OrderResult.Failure("Insufficient inventory for order");
            }
            
            // Step 2: Reserve inventory
            await inventoryService.ReserveItemsAsync(order);
            Console.WriteLine("📦 Inventory reserved");
            
            // Step 3: Calculate discounts
            var discount = discountCalculator.CalculateDiscount(order);
            var finalAmount = order.TotalAmount - discount;
            Console.WriteLine($"💰 Discount applied: ${discount:F2}, Final amount: ${finalAmount:F2}");
            
            // Step 4: Process payment
            var paymentResult = await paymentProcessor.ProcessPaymentAsync(finalAmount, paymentInfo);
            if (!paymentResult.IsSuccessful)
            {
                await inventoryService.ReleaseReservationAsync(order);
                return OrderResult.Failure($"Payment failed: {paymentResult.ErrorMessage}");
            }
            Console.WriteLine($"💳 Payment processed: {paymentResult.Message}");
            
            // Step 5: Update order status
            order.UpdateStatus(OrderStatus.Paid);
            await orderRepository.SaveAsync(order);
            
            // Step 6: Send confirmation
            await notificationService.SendOrderConfirmationAsync(order);
            
            // Step 7: Initiate shipping
            var shippingResult = await shippingService.CreateShipmentAsync(order);
            if (shippingResult.IsSuccessful)
            {
                order.UpdateStatus(OrderStatus.Processing);
                await orderRepository.SaveAsync(order);
                await notificationService.SendShippingNotificationAsync(order, shippingResult.TrackingNumber);
            }
            
            return OrderResult.Success(order, paymentResult);
        }
        catch (Exception ex)
        {
            Console.WriteLine($"❌ Order processing failed: {ex.Message}");
            await inventoryService.ReleaseReservationAsync(order);
            return OrderResult.Failure($"Order processing failed: {ex.Message}");
        }
    }
}

// Dependency Injection Container setup
public class DIContainer
{
    public static IServiceCollection ConfigureServices()
    {
        var services = new ServiceCollection();
        
        // Register abstractions with their implementations
        // This is where we decide which concrete classes to use
        
        // Data layer
        services.AddScoped<IOrderRepository, FileOrderRepository>(); // Could be DatabaseOrderRepository
        services.AddScoped<ICustomerRepository, FileCustomerRepository>();
        services.AddScoped<IInventoryRepository, FileInventoryRepository>();
        
        // Business services
        services.AddScoped<IInventoryService, InventoryService>();
        services.AddScoped<IShippingService, StandardShippingService>(); // Could be ExpressShippingService
        
        // Payment processing
        services.AddScoped<IPaymentProcessor, CreditCardProcessor>(); // Could be PayPalProcessor
        services.AddScoped<IPaymentValidator, StandardPaymentValidator>();
        services.AddScoped<IPaymentLogger, FilePaymentLogger>();
        services.AddScoped<IFraudDetector, BasicFraudDetector>();
        
        // Notifications
        services.AddScoped<INotificationService>(provider =>
            new MultiChannelNotificationService(
                new EmailNotificationService(new SMTPEmailSender(), provider.GetService<ICustomerRepository>()),
                new SMSNotificationService(new TwilioSMSSender(), provider.GetService<ICustomerRepository>())
            ));
        
        // Discount strategies
        services.AddScoped<IDiscountCalculator>(provider =>
            new DiscountCalculator(new List<IDiscountStrategy>
            {
                new VolumeDiscountStrategy(),
                new CustomerLoyaltyDiscountStrategy(provider.GetService<ICustomerRepository>()),
                new SeasonalDiscountStrategy()
            }));
        
        // High-level orchestrator
        services.AddScoped<OrderManagementSystem>();
        
        return services;
    }
}

// Application entry point demonstrating DIP
public class Program
{
    public static async Task Main(string[] args)
    {
        // Configure dependency injection
        var services = DIContainer.ConfigureServices();
        var serviceProvider = services.BuildServiceProvider();
        
        // Get the high-level service (which depends only on abstractions)
        var orderSystem = serviceProvider.GetService<OrderManagementSystem>();
        
        // Create test data
        var customer = new Customer("John Doe", "john@email.com", "+1-555-1234", CustomerType.Premium);
        var order = new Order(customer.Id);
        order.AddItem(new OrderItem("LAPTOP001", "Gaming Laptop", 1299.99m, 1));
        order.AddItem(new OrderItem("MOUSE001", "Gaming Mouse", 79.99m, 2));
        
        var paymentInfo = new PaymentInfo
        {
            CreditCard = new CreditCard("1234-5678-9012-3456", "12/25", "123", "John Doe")
        };
        
        // Process the order
        Console.WriteLine("🛒 E-Commerce Order Processing System");
        Console.WriteLine("=====================================\n");
        
        var result = await orderSystem.ProcessCompleteOrderAsync(order, paymentInfo);
        
        if (result.IsSuccessful)
        {
            Console.WriteLine($"\n✅ Order {order.Id} processed successfully!");
            Console.WriteLine($"📄 Order Status: {order.Status}");
            Console.WriteLine($"💰 Total Amount: ${order.TotalAmount:F2}");
        }
        else
        {
            Console.WriteLine($"\n❌ Order processing failed: {result.ErrorMessage}");
        }
        
        // Demonstrate that we can easily swap implementations
        Console.WriteLine("\n🔄 Demonstrating different payment processors:");
        await DemonstratePaymentProcessorSwapping(serviceProvider);
    }
    
    private static async Task DemonstratePaymentProcessorSwapping(IServiceProvider serviceProvider)
    {
        var paymentProcessors = new List<IPaymentProcessor>
        {
            new CreditCardProcessor(),
            new PayPalProcessor(),
            new CryptocurrencyProcessor()
        };
        
        var paymentInfo = new PaymentInfo
        {
            CreditCard = new CreditCard("1234-5678-9012-3456", "12/25", "123", "Jane Doe"),
            PayPalEmail = "jane@email.com",
            CryptoWallet = "1A1zP1eP5QGefi2DMPTfTL5SLmv7DivfNa"
        };
        
        foreach (var processor in paymentProcessors)
        {
            Console.WriteLine($"\n💳 Testing {processor.GetPaymentMethodName()}:");
            var result = await processor.ProcessPaymentAsync(99.99m, paymentInfo);
            Console.WriteLine($"  Result: {(result.IsSuccessful ? "✅" : "❌")} {result.Message}");
        }
    }
}
```

## Complete System Integration 🔧

Here's how all the pieces work together:

```csharp
// Supporting classes and interfaces
public class OrderResult
{
    public bool IsSuccessful { get; private set; }
    public string ErrorMessage { get; private set; }
    public Order Order { get; private set; }
    public PaymentResult PaymentResult { get; private set; }
    
    private OrderResult(bool isSuccessful, string errorMessage, Order order, PaymentResult paymentResult)
    {
        IsSuccessful = isSuccessful;
        ErrorMessage = errorMessage;
        Order = order;
        PaymentResult = paymentResult;
    }
    
    public static OrderResult Success(Order order, PaymentResult paymentResult) =>
        new OrderResult(true, null, order, paymentResult);
    
    public static OrderResult Failure(string errorMessage) =>
        new OrderResult(false, errorMessage, null, null);
}

public class PaymentResult
{
    public bool IsSuccessful { get; private set; }
    public string TransactionId { get; private set; }
    public string Message { get; private set; }
    public string ErrorMessage { get; private set; }
    
    private PaymentResult(bool isSuccessful, string transactionId, string message, string errorMessage)
    {
        IsSuccessful = isSuccessful;
        TransactionId = transactionId;
        Message = message;
        ErrorMessage = errorMessage;
    }
    
    public static PaymentResult Success(string transactionId, string message) =>
        new PaymentResult(true, transactionId, message, null);
    
    public static PaymentResult Failure(string errorMessage) =>
        new PaymentResult(false, null, null, errorMessage);
}

public class PaymentInfo
{
    public CreditCard CreditCard { get; set; }
    public string PayPalEmail { get; set; }
    public string CryptoWallet { get; set; }
}

public class CreditCard
{
    public string Number { get; private set; }
    public string ExpiryDate { get; private set; }
    public string CVV { get; private set; }
    public string HolderName { get; private set; }
    public string LastFourDigits => Number.Substring(Number.Length - 4);
    
    public CreditCard(string number, string expiryDate, string cvv, string holderName)
    {
        Number = number;
        ExpiryDate = expiryDate;
        CVV = cvv;
        HolderName = holderName;
    }
    
    public bool IsValid()
    {
        // Simple validation logic
        return !string.IsNullOrEmpty(Number) && Number.Length == 19 && 
               !string.IsNullOrEmpty(CVV) && CVV.Length == 3;
    }
}

// Mock implementations for demonstration
public class MockEmailSender : IEmailSender
{
    public Task SendAsync(string to, string subject, string body) => Task.CompletedTask;
}

public class MockSMSSender : ISMSSender
{
    public Task SendAsync(string phoneNumber, string message) => Task.CompletedTask;
}

public class MockCustomerRepository : ICustomerRepository
{
    public Task<Customer> GetByIdAsync(string customerId) =>
        Task.FromResult(new Customer("Mock Customer", "mock@email.com", "+1-555-0000", CustomerType.Regular));
}
```

## Key Takeaways 🎯

1. **SRP in Action**: Each class has a single, well-defined responsibility:
   - `OrderService` manages order processing
   - `DiscountCalculator` handles discount logic
   - `InventoryService` manages inventory operations

2. **OCP in Action**: System is easily extensible:
   - New payment methods can be added without changing existing code
   - New discount strategies can be plugged in
   - New notification channels can be integrated

3. **LSP in Action**: All implementations are substitutable:
   - Any `IPaymentProcessor` works with the payment system
   - Any `INotificationService` works with the notification system
   - System behavior remains correct regardless of implementation

4. **ISP in Action**: Interfaces are focused and segregated:
   - Reporting classes only implement what they need
   - Payment processing is split into focused interfaces
   - Clients depend only on relevant interfaces

5. **DIP in Action**: High-level modules depend on abstractions:
   - `OrderManagementSystem` depends on interfaces, not concrete classes
   - Dependencies are injected, making the system flexible and testable
   - Easy to swap implementations without changing high-level logic

## Benefits Achieved ✨

- **Maintainability**: Each class has clear responsibilities
- **Extensibility**: New features can be added without modifying existing code
- **Testability**: All dependencies are injected and can be mocked
- **Flexibility**: Different implementations can be swapped easily
- **Reusability**: Components can be reused in different contexts
- **Reliability**: Well-defined contracts ensure predictable behavior

Remember: SOLID principles work together to create software that is robust, maintainable, and adaptable to changing requirements! 🌟

## Exercises

1. **Easy**: Add a new notification channel (WhatsApp) without modifying existing code.
2. **Medium**: Implement a new discount strategy (first-time customer discount) that integrates seamlessly.
3. **Hard**: Add a user authentication and authorization system while maintaining all SOLID principles.

## Chapter Checklist

After completing this chapter, you should be able to:

- [ ] Apply all five SOLID principles together in a complete system
- [ ] Design architectures that are both flexible and maintainable
- [ ] Identify how each SOLID principle contributes to the overall design
- [ ] Create systems that can be extended without modification
- [ ] Use dependency injection to achieve loose coupling
- [ ] Design interfaces that promote the SOLID principles
- [ ] Balance between over-engineering and under-engineering
- [ ] Recognize when SOLID principles conflict and how to resolve them
- [ ] Build systems that are testable and maintainable