# Chapter 13: KISS, YAGNI, DRY - The Pragmatic Trio 🎯

## Child-Friendly Explanation 🧸
Imagine you're building a treehouse. KISS says "keep it simple" - don't make it so complicated that you can't even use it! YAGNI says "don't build extra rooms until you actually need them" - maybe you think you'll want a library, but build the main room first. DRY says "don't do the same work twice" - if you need to hammer nails, don't make a new hammer for each nail, just use the same hammer over and over!

## Developer-Level Explanation 👨‍💻
KISS, YAGNI, and DRY are three pragmatic principles that complement SOLID and help create maintainable, efficient code. They guide us toward practical solutions that solve real problems without unnecessary complexity.

Let's explore each principle with real-world examples and see how they work together!

## KISS - Keep It Simple, Stupid 🎨

The KISS principle advocates for simplicity. Choose the simplest solution that works. Avoid unnecessary complexity, prefer readable code over clever code.

### Problem: Over-Engineering a Simple Calculator

```csharp
// ❌ KISS Violation - Over-complicated calculator
public abstract class CalculationOperation
{
    public abstract double Execute(params double[] operands);
    public abstract bool ValidateOperands(params double[] operands);
    public abstract string GetOperationName();
}

public class AdditionOperation : CalculationOperation
{
    public override double Execute(params double[] operands)
    {
        if (!ValidateOperands(operands))
            throw new InvalidOperationException("Invalid operands for addition");
        
        return operands.Sum();
    }
    
    public override bool ValidateOperands(params double[] operands)
    {
        return operands != null && operands.Length >= 2 && 
               operands.All(o => !double.IsNaN(o) && !double.IsInfinity(o));
    }
    
    public override string GetOperationName() => "Addition";
}

public class SubtractionOperation : CalculationOperation
{
    public override double Execute(params double[] operands)
    {
        if (!ValidateOperands(operands))
            throw new InvalidOperationException("Invalid operands for subtraction");
        
        double result = operands[0];
        for (int i = 1; i < operands.Length; i++)
            result -= operands[i];
        
        return result;
    }
    
    public override bool ValidateOperands(params double[] operands)
    {
        return operands != null && operands.Length >= 2 && 
               operands.All(o => !double.IsNaN(o) && !double.IsInfinity(o));
    }
    
    public override string GetOperationName() => "Subtraction";
}

public class CalculatorFactory
{
    private readonly Dictionary<string, Func<CalculationOperation>> operations;
    
    public CalculatorFactory()
    {
        operations = new Dictionary<string, Func<CalculationOperation>>
        {
            ["add"] = () => new AdditionOperation(),
            ["subtract"] = () => new SubtractionOperation(),
            // ... more operations
        };
    }
    
    public CalculationOperation CreateOperation(string operationType)
    {
        if (operations.ContainsKey(operationType.ToLower()))
            return operations[operationType.ToLower()]();
        
        throw new NotSupportedException($"Operation {operationType} not supported");
    }
}

public class AdvancedCalculator
{
    private readonly CalculatorFactory factory;
    private readonly List<string> operationHistory;
    private readonly ILogger logger;
    
    public AdvancedCalculator(CalculatorFactory factory, ILogger logger)
    {
        this.factory = factory;
        this.logger = logger;
        this.operationHistory = new List<string>();
    }
    
    public double Calculate(string operation, params double[] operands)
    {
        var op = factory.CreateOperation(operation);
        var result = op.Execute(operands);
        
        var historyEntry = $"{op.GetOperationName()}: {string.Join(", ", operands)} = {result}";
        operationHistory.Add(historyEntry);
        logger.Log($"Calculation performed: {historyEntry}");
        
        return result;
    }
}

// ✅ KISS Solution - Simple and straightforward
public class SimpleCalculator
{
    public double Add(double a, double b) => a + b;
    public double Subtract(double a, double b) => a - b;
    public double Multiply(double a, double b) => a * b;
    public double Divide(double a, double b) => b != 0 ? a / b : throw new DivideByZeroException();
}

// Usage comparison
public class KissDemo
{
    public static void RunDemo()
    {
        Console.WriteLine("🎨 KISS Principle - Keep It Simple!");
        Console.WriteLine("===================================\n");
        
        // Complex solution
        Console.WriteLine("❌ Over-engineered calculator:");
        var factory = new CalculatorFactory();
        var logger = new ConsoleLogger();
        var complexCalculator = new AdvancedCalculator(factory, logger);
        
        var result1 = complexCalculator.Calculate("add", 5, 3);
        Console.WriteLine($"Complex result: {result1}");
        
        // Simple solution
        Console.WriteLine("\n✅ Simple calculator:");
        var simpleCalculator = new SimpleCalculator();
        var result2 = simpleCalculator.Add(5, 3);
        Console.WriteLine($"Simple result: {result2}");
        
        Console.WriteLine("\n💡 Both produce the same result, but simple is better!");
        Console.WriteLine("   - Easier to understand");
        Console.WriteLine("   - Faster to write");
        Console.WriteLine("   - Less likely to have bugs");
        Console.WriteLine("   - Easier to maintain");
    }
}
```

### KISS in Web Development

```csharp
// ❌ KISS Violation - Over-complicated user authentication
public abstract class AuthenticationStrategy
{
    public abstract Task<AuthenticationResult> AuthenticateAsync(IAuthenticationContext context);
    protected abstract bool ValidateCredentials(ICredentials credentials);
    protected abstract IUserProfile CreateUserProfile(IUserData userData);
}

public class DatabaseAuthenticationStrategy : AuthenticationStrategy
{
    private readonly IUserRepository userRepository;
    private readonly IPasswordHasher passwordHasher;
    private readonly IAuthenticationLogger logger;
    private readonly IUserProfileFactory profileFactory;
    
    // ... complex implementation with multiple layers
}

public class AuthenticationServiceManager
{
    private readonly Dictionary<AuthenticationType, AuthenticationStrategy> strategies;
    private readonly IAuthenticationEventBus eventBus;
    private readonly ISecurityAuditor auditor;
    
    // ... complex orchestration logic
}

// ✅ KISS Solution - Simple and effective
public class UserAuthService
{
    private readonly IUserRepository userRepository;
    
    public UserAuthService(IUserRepository userRepository)
    {
        this.userRepository = userRepository;
    }
    
    public async Task<User> LoginAsync(string email, string password)
    {
        var user = await userRepository.GetByEmailAsync(email);
        
        if (user == null || !VerifyPassword(password, user.PasswordHash))
            return null;
        
        return user;
    }
    
    private bool VerifyPassword(string password, string hash)
    {
        // Simple password verification
        return BCrypt.Net.BCrypt.Verify(password, hash);
    }
}

// Usage
public class AuthDemo
{
    public static async Task RunDemo()
    {
        Console.WriteLine("🔐 Simple Authentication Example:");
        
        var userRepo = new MockUserRepository();
        var authService = new UserAuthService(userRepo);
        
        var user = await authService.LoginAsync("user@example.com", "password123");
        
        if (user != null)
            Console.WriteLine($"✅ Login successful! Welcome, {user.Name}");
        else
            Console.WriteLine("❌ Login failed");
    }
}
```

## YAGNI - You Aren't Gonna Need It 🔮

YAGNI principle tells us not to implement functionality until we actually need it. Don't build for hypothetical future requirements.

### Problem: Building for Imaginary Future Needs

```csharp
// ❌ YAGNI Violation - Building for hypothetical requirements
public class BlogPost
{
    public string Id { get; set; }
    public string Title { get; set; }
    public string Content { get; set; }
    public string Author { get; set; }
    public DateTime CreatedAt { get; set; }
    public DateTime PublishedAt { get; set; }
    
    // YAGNI violations - features "we might need"
    public List<string> Tags { get; set; } = new List<string>();
    public Dictionary<string, object> Metadata { get; set; } = new Dictionary<string, object>();
    public int Version { get; set; } = 1;
    public string OriginalLanguage { get; set; } = "en";
    public Dictionary<string, string> Translations { get; set; } = new Dictionary<string, string>();
    public bool IsFeatured { get; set; }
    public bool IsSponsored { get; set; }
    public decimal SponsoredAmount { get; set; }
    public DateTime? ScheduledPublishTime { get; set; }
    public List<string> RelatedPostIds { get; set; } = new List<string>();
    public int ReadingTimeMinutes { get; set; }
    public string Excerpt { get; set; }
    public string FeaturedImageUrl { get; set; }
    public List<string> CategoryIds { get; set; } = new List<string>();
    public bool AllowComments { get; set; } = true;
    public bool RequireModeration { get; set; } = false;
    public string CustomCSSClass { get; set; }
    public Dictionary<string, string> SEOMetadata { get; set; } = new Dictionary<string, string>();
}

public class BlogService
{
    // Complex service trying to handle all possible scenarios
    public async Task<BlogPost> CreatePostAsync(CreatePostRequest request)
    {
        var post = new BlogPost
        {
            Id = Guid.NewGuid().ToString(),
            Title = request.Title,
            Content = request.Content,
            Author = request.Author,
            CreatedAt = DateTime.UtcNow
        };
        
        // Handling all the "might need" features
        if (request.Tags?.Any() == true)
            post.Tags = request.Tags;
        
        if (request.ScheduledPublishTime.HasValue)
            post.ScheduledPublishTime = request.ScheduledPublishTime;
        
        // Complex publishing logic for different scenarios
        if (request.IsDraft)
        {
            // Save as draft
        }
        else if (request.ScheduledPublishTime.HasValue)
        {
            // Schedule for later
        }
        else
        {
            post.PublishedAt = DateTime.UtcNow;
            // Immediate publish with complex workflow
        }
        
        // Calculating reading time (might need?)
        post.ReadingTimeMinutes = CalculateReadingTime(post.Content);
        
        // Auto-generating excerpt (might need?)
        if (string.IsNullOrEmpty(request.Excerpt))
            post.Excerpt = GenerateExcerpt(post.Content);
        
        // Complex save logic handling all scenarios
        await SavePostWithAllFeatures(post);
        
        return post;
    }
    
    private int CalculateReadingTime(string content)
    {
        // Complex reading time calculation we might not need
        var wordCount = content.Split(' ').Length;
        return Math.Max(1, wordCount / 200); // Average reading speed
    }
    
    private string GenerateExcerpt(string content)
    {
        // Complex excerpt generation we might not need
        if (content.Length <= 150) return content;
        return content.Substring(0, 147) + "...";
    }
    
    private async Task SavePostWithAllFeatures(BlogPost post)
    {
        // Complex save logic handling versioning, translations, etc.
        // All features we might never use
    }
}

// ✅ YAGNI Solution - Build only what we need NOW
public class SimpleBlogPost
{
    public string Id { get; set; }
    public string Title { get; set; }
    public string Content { get; set; }
    public string Author { get; set; }
    public DateTime CreatedAt { get; set; }
    public DateTime? PublishedAt { get; set; }
    public bool IsDraft { get; set; }
}

public class SimpleBlogService
{
    private readonly IBlogRepository repository;
    
    public SimpleBlogService(IBlogRepository repository)
    {
        this.repository = repository;
    }
    
    public async Task<SimpleBlogPost> CreatePostAsync(string title, string content, string author)
    {
        var post = new SimpleBlogPost
        {
            Id = Guid.NewGuid().ToString(),
            Title = title,
            Content = content,
            Author = author,
            CreatedAt = DateTime.UtcNow,
            IsDraft = true // Start as draft by default
        };
        
        await repository.SaveAsync(post);
        return post;
    }
    
    public async Task PublishPostAsync(string postId)
    {
        var post = await repository.GetByIdAsync(postId);
        if (post != null && post.IsDraft)
        {
            post.IsDraft = false;
            post.PublishedAt = DateTime.UtcNow;
            await repository.SaveAsync(post);
        }
    }
    
    public async Task<List<SimpleBlogPost>> GetPublishedPostsAsync()
    {
        return await repository.GetPublishedPostsAsync();
    }
}

// Usage
public class YagniDemo
{
    public static async Task RunDemo()
    {
        Console.WriteLine("🔮 YAGNI Principle - You Aren't Gonna Need It!");
        Console.WriteLine("================================================\n");
        
        var repository = new MockBlogRepository();
        var blogService = new SimpleBlogService(repository);
        
        // Create a simple blog post
        var post = await blogService.CreatePostAsync(
            "My First Post", 
            "This is the content of my first blog post.", 
            "John Doe"
        );
        
        Console.WriteLine($"📝 Created draft post: {post.Title}");
        
        // Publish the post
        await blogService.PublishPostAsync(post.Id);
        Console.WriteLine($"📤 Published post: {post.Title}");
        
        // Get published posts
        var publishedPosts = await blogService.GetPublishedPostsAsync();
        Console.WriteLine($"📚 Found {publishedPosts.Count} published posts");
        
        Console.WriteLine("\n💡 YAGNI Benefits:");
        Console.WriteLine("   - Faster to implement");
        Console.WriteLine("   - Less code to maintain");
        Console.WriteLine("   - Easier to understand");
        Console.WriteLine("   - Can add features when actually needed");
    }
}
```

### YAGNI in API Design

```csharp
// ❌ YAGNI Violation - Over-designed API for future scenarios
public class UserController : ControllerBase
{
    [HttpGet]
    public async Task<IActionResult> GetUsers(
        [FromQuery] int page = 1,
        [FromQuery] int pageSize = 10,
        [FromQuery] string sortBy = "name",
        [FromQuery] string sortDirection = "asc",
        [FromQuery] string filterBy = null,
        [FromQuery] string filterValue = null,
        [FromQuery] string groupBy = null,
        [FromQuery] bool includeMetadata = false,
        [FromQuery] string expand = null,
        [FromQuery] string fields = null,
        [FromQuery] string format = "json") // Might support XML, CSV later
    {
        // Complex implementation handling all possible scenarios
        // that we might never actually need
    }
    
    [HttpPost]
    public async Task<IActionResult> CreateUser(
        [FromBody] CreateUserRequest request,
        [FromQuery] bool sendWelcomeEmail = true,
        [FromQuery] bool validateInRealTime = false,
        [FromQuery] string profileImageProcessing = "standard", // future: "ai-enhanced"
        [FromQuery] bool enableAuditTrail = false)
    {
        // Complex creation with many optional features
    }
}

// ✅ YAGNI Solution - Simple API that meets current needs
public class SimpleUserController : ControllerBase
{
    private readonly IUserService userService;
    
    public SimpleUserController(IUserService userService)
    {
        this.userService = userService;
    }
    
    [HttpGet]
    public async Task<IActionResult> GetUsers()
    {
        var users = await userService.GetAllUsersAsync();
        return Ok(users);
    }
    
    [HttpGet("{id}")]
    public async Task<IActionResult> GetUser(string id)
    {
        var user = await userService.GetUserByIdAsync(id);
        return user != null ? Ok(user) : NotFound();
    }
    
    [HttpPost]
    public async Task<IActionResult> CreateUser([FromBody] CreateUserRequest request)
    {
        var user = await userService.CreateUserAsync(request.Name, request.Email);
        return CreatedAtAction(nameof(GetUser), new { id = user.Id }, user);
    }
    
    [HttpPut("{id}")]
    public async Task<IActionResult> UpdateUser(string id, [FromBody] UpdateUserRequest request)
    {
        var updated = await userService.UpdateUserAsync(id, request.Name, request.Email);
        return updated ? Ok() : NotFound();
    }
    
    [HttpDelete("{id}")]
    public async Task<IActionResult> DeleteUser(string id)
    {
        var deleted = await userService.DeleteUserAsync(id);
        return deleted ? NoContent() : NotFound();
    }
}

public class CreateUserRequest
{
    public string Name { get; set; }
    public string Email { get; set; }
}

public class UpdateUserRequest
{
    public string Name { get; set; }
    public string Email { get; set; }
}
```

## DRY - Don't Repeat Yourself 🔁

The DRY principle states that every piece of knowledge should have a single, authoritative representation. Avoid code duplication, but don't abstract prematurely.

### Problem: Code Duplication

```csharp
// ❌ DRY Violation - Repeated validation and processing logic
public class CustomerService
{
    public async Task<Customer> CreateCustomerAsync(string name, string email, string phone)
    {
        // Validation logic repeated
        if (string.IsNullOrWhiteSpace(name))
            throw new ArgumentException("Name is required");
        if (string.IsNullOrWhiteSpace(email))
            throw new ArgumentException("Email is required");
        if (!IsValidEmail(email))
            throw new ArgumentException("Invalid email format");
        if (string.IsNullOrWhiteSpace(phone))
            throw new ArgumentException("Phone is required");
        if (!IsValidPhone(phone))
            throw new ArgumentException("Invalid phone format");
        
        // Processing logic repeated
        var normalizedName = name.Trim().ToTitleCase();
        var normalizedEmail = email.Trim().ToLower();
        var normalizedPhone = NormalizePhoneNumber(phone);
        
        var customer = new Customer
        {
            Id = Guid.NewGuid().ToString(),
            Name = normalizedName,
            Email = normalizedEmail,
            Phone = normalizedPhone,
            CreatedAt = DateTime.UtcNow
        };
        
        await SaveCustomerAsync(customer);
        await SendWelcomeEmailAsync(customer);
        
        return customer;
    }
    
    public async Task<Supplier> CreateSupplierAsync(string name, string email, string phone)
    {
        // Same validation logic repeated!
        if (string.IsNullOrWhiteSpace(name))
            throw new ArgumentException("Name is required");
        if (string.IsNullOrWhiteSpace(email))
            throw new ArgumentException("Email is required");
        if (!IsValidEmail(email))
            throw new ArgumentException("Invalid email format");
        if (string.IsNullOrWhiteSpace(phone))
            throw new ArgumentException("Phone is required");
        if (!IsValidPhone(phone))
            throw new ArgumentException("Invalid phone format");
        
        // Same processing logic repeated!
        var normalizedName = name.Trim().ToTitleCase();
        var normalizedEmail = email.Trim().ToLower();
        var normalizedPhone = NormalizePhoneNumber(phone);
        
        var supplier = new Supplier
        {
            Id = Guid.NewGuid().ToString(),
            Name = normalizedName,
            Email = normalizedEmail,
            Phone = normalizedPhone,
            CreatedAt = DateTime.UtcNow
        };
        
        await SaveSupplierAsync(supplier);
        await SendWelcomeEmailAsync(supplier); // Similar logic
        
        return supplier;
    }
    
    private bool IsValidEmail(string email)
    {
        // Email validation logic
        return email.Contains("@") && email.Contains(".");
    }
    
    private bool IsValidPhone(string phone)
    {
        // Phone validation logic
        return phone.Length >= 10;
    }
    
    private string NormalizePhoneNumber(string phone)
    {
        // Phone normalization logic
        return phone.Replace("-", "").Replace(" ", "").Replace("(", "").Replace(")", "");
    }
}

// ✅ DRY Solution - Extract common functionality
public class ContactInfo
{
    public string Name { get; set; }
    public string Email { get; set; }
    public string Phone { get; set; }
    
    public ContactInfo(string name, string email, string phone)
    {
        Name = name;
        Email = email;
        Phone = phone;
    }
}

public class ContactValidator
{
    public void ValidateContactInfo(ContactInfo contact)
    {
        if (string.IsNullOrWhiteSpace(contact.Name))
            throw new ArgumentException("Name is required");
        if (string.IsNullOrWhiteSpace(contact.Email))
            throw new ArgumentException("Email is required");
        if (!IsValidEmail(contact.Email))
            throw new ArgumentException("Invalid email format");
        if (string.IsNullOrWhiteSpace(contact.Phone))
            throw new ArgumentException("Phone is required");
        if (!IsValidPhone(contact.Phone))
            throw new ArgumentException("Invalid phone format");
    }
    
    private bool IsValidEmail(string email)
    {
        return email.Contains("@") && email.Contains(".");
    }
    
    private bool IsValidPhone(string phone)
    {
        return phone.Length >= 10;
    }
}

public class ContactNormalizer
{
    public ContactInfo NormalizeContactInfo(ContactInfo contact)
    {
        return new ContactInfo(
            NormalizeName(contact.Name),
            NormalizeEmail(contact.Email),
            NormalizePhone(contact.Phone)
        );
    }
    
    private string NormalizeName(string name)
    {
        return name.Trim().ToTitleCase();
    }
    
    private string NormalizeEmail(string email)
    {
        return email.Trim().ToLower();
    }
    
    private string NormalizePhone(string phone)
    {
        return phone.Replace("-", "").Replace(" ", "").Replace("(", "").Replace(")", "");
    }
}

public class ImprovedCustomerService
{
    private readonly ContactValidator validator;
    private readonly ContactNormalizer normalizer;
    private readonly ICustomerRepository customerRepository;
    private readonly IEmailService emailService;
    
    public ImprovedCustomerService(
        ContactValidator validator,
        ContactNormalizer normalizer,
        ICustomerRepository customerRepository,
        IEmailService emailService)
    {
        this.validator = validator;
        this.normalizer = normalizer;
        this.customerRepository = customerRepository;
        this.emailService = emailService;
    }
    
    public async Task<Customer> CreateCustomerAsync(string name, string email, string phone)
    {
        var contactInfo = new ContactInfo(name, email, phone);
        
        // Reuse validation logic
        validator.ValidateContactInfo(contactInfo);
        
        // Reuse normalization logic
        var normalizedContact = normalizer.NormalizeContactInfo(contactInfo);
        
        var customer = new Customer
        {
            Id = Guid.NewGuid().ToString(),
            Name = normalizedContact.Name,
            Email = normalizedContact.Email,
            Phone = normalizedContact.Phone,
            CreatedAt = DateTime.UtcNow
        };
        
        await customerRepository.SaveAsync(customer);
        await emailService.SendWelcomeEmailAsync(customer.Email, customer.Name);
        
        return customer;
    }
}

public class ImprovedSupplierService
{
    private readonly ContactValidator validator;
    private readonly ContactNormalizer normalizer;
    private readonly ISupplierRepository supplierRepository;
    private readonly IEmailService emailService;
    
    public ImprovedSupplierService(
        ContactValidator validator,
        ContactNormalizer normalizer,
        ISupplierRepository supplierRepository,
        IEmailService emailService)
    {
        this.validator = validator;
        this.normalizer = normalizer;
        this.supplierRepository = supplierRepository;
        this.emailService = emailService;
    }
    
    public async Task<Supplier> CreateSupplierAsync(string name, string email, string phone)
    {
        var contactInfo = new ContactInfo(name, email, phone);
        
        // Reuse the same validation logic
        validator.ValidateContactInfo(contactInfo);
        
        // Reuse the same normalization logic
        var normalizedContact = normalizer.NormalizeContactInfo(contactInfo);
        
        var supplier = new Supplier
        {
            Id = Guid.NewGuid().ToString(),
            Name = normalizedContact.Name,
            Email = normalizedContact.Email,
            Phone = normalizedContact.Phone,
            CreatedAt = DateTime.UtcNow
        };
        
        await supplierRepository.SaveAsync(supplier);
        await emailService.SendWelcomeEmailAsync(supplier.Email, supplier.Name);
        
        return supplier;
    }
}

// Usage
public class DryDemo
{
    public static async Task RunDemo()
    {
        Console.WriteLine("🔁 DRY Principle - Don't Repeat Yourself!");
        Console.WriteLine("=========================================\n");
        
        var validator = new ContactValidator();
        var normalizer = new ContactNormalizer();
        var customerRepo = new MockCustomerRepository();
        var supplierRepo = new MockSupplierRepository();
        var emailService = new MockEmailService();
        
        var customerService = new ImprovedCustomerService(validator, normalizer, customerRepo, emailService);
        var supplierService = new ImprovedSupplierService(validator, normalizer, supplierRepo, emailService);
        
        // Create customer and supplier using shared validation/normalization
        var customer = await customerService.CreateCustomerAsync("john doe", "JOHN@EXAMPLE.COM", "555-123-4567");
        var supplier = await supplierService.CreateSupplierAsync("acme corp", "CONTACT@ACME.COM", "(555) 987-6543");
        
        Console.WriteLine($"✅ Created customer: {customer.Name} | {customer.Email} | {customer.Phone}");
        Console.WriteLine($"✅ Created supplier: {supplier.Name} | {supplier.Email} | {supplier.Phone}");
        
        Console.WriteLine("\n💡 DRY Benefits:");
        Console.WriteLine("   - Single source of truth for validation rules");
        Console.WriteLine("   - Changes to logic only need to be made in one place");
        Console.WriteLine("   - Reduced chance of inconsistencies");
        Console.WriteLine("   - Less code to maintain");
    }
}
```

### When NOT to Apply DRY (Premature Abstraction)

```csharp
// ❌ DRY Gone Wrong - Premature abstraction
public class ReportProcessor<T>
{
    public async Task<string> ProcessReportAsync<TData>(
        TData data, 
        Func<TData, Task<T>> transformer,
        Func<T, string> formatter,
        Action<string> validator = null)
    {
        validator?.Invoke(data.ToString());
        var transformed = await transformer(data);
        return formatter(transformed);
    }
}

// This abstraction tries to handle all reports but makes the code confusing

// ✅ Sometimes duplication is better than bad abstraction
public class SalesReportGenerator
{
    public async Task<string> GenerateSalesReportAsync(SalesData data)
    {
        ValidateSalesData(data);
        var report = await TransformSalesDataAsync(data);
        return FormatSalesReport(report);
    }
    
    private void ValidateSalesData(SalesData data)
    {
        if (data.StartDate > data.EndDate)
            throw new ArgumentException("Start date must be before end date");
        // Sales-specific validation
    }
    
    private async Task<SalesReport> TransformSalesDataAsync(SalesData data)
    {
        // Sales-specific transformation
        return new SalesReport 
        { 
            TotalSales = data.Orders.Sum(o => o.Amount),
            OrderCount = data.Orders.Count
        };
    }
    
    private string FormatSalesReport(SalesReport report)
    {
        return $"Sales Report\nTotal: ${report.TotalSales:F2}\nOrders: {report.OrderCount}";
    }
}

public class InventoryReportGenerator
{
    public async Task<string> GenerateInventoryReportAsync(InventoryData data)
    {
        ValidateInventoryData(data);
        var report = await TransformInventoryDataAsync(data);
        return FormatInventoryReport(report);
    }
    
    private void ValidateInventoryData(InventoryData data)
    {
        if (data.Products == null || !data.Products.Any())
            throw new ArgumentException("No products found");
        // Inventory-specific validation
    }
    
    private async Task<InventoryReport> TransformInventoryDataAsync(InventoryData data)
    {
        // Inventory-specific transformation
        return new InventoryReport
        {
            LowStockItems = data.Products.Where(p => p.Quantity < p.MinimumStock).ToList(),
            TotalProducts = data.Products.Count
        };
    }
    
    private string FormatInventoryReport(InventoryReport report)
    {
        return $"Inventory Report\nTotal Products: {report.TotalProducts}\nLow Stock: {report.LowStockItems.Count}";
    }
}
```

## Balancing the Three Principles ⚖️

```csharp
// Real-world example showing how to balance KISS, YAGNI, and DRY
public class OrderProcessor
{
    private readonly IPaymentService paymentService;
    private readonly IInventoryService inventoryService;
    private readonly INotificationService notificationService;
    
    public OrderProcessor(
        IPaymentService paymentService,
        IInventoryService inventoryService,
        INotificationService notificationService)
    {
        this.paymentService = paymentService;
        this.inventoryService = inventoryService;
        this.notificationService = notificationService;
    }
    
    // KISS: Simple, straightforward method that's easy to understand
    public async Task<OrderResult> ProcessOrderAsync(Order order)
    {
        try
        {
            // Step 1: Validate order (DRY: reusable validation)
            var validationResult = ValidateOrder(order);
            if (!validationResult.IsValid)
                return OrderResult.Failure(validationResult.ErrorMessage);
            
            // Step 2: Check inventory
            var inventoryAvailable = await inventoryService.CheckAvailabilityAsync(order.Items);
            if (!inventoryAvailable)
                return OrderResult.Failure("Insufficient inventory");
            
            // Step 3: Process payment
            var paymentResult = await paymentService.ProcessPaymentAsync(order.PaymentInfo, order.Total);
            if (!paymentResult.IsSuccessful)
                return OrderResult.Failure($"Payment failed: {paymentResult.ErrorMessage}");
            
            // Step 4: Reserve inventory
            await inventoryService.ReserveItemsAsync(order.Items);
            
            // Step 5: Send confirmation (YAGNI: just email for now, can add SMS later if needed)
            await notificationService.SendOrderConfirmationAsync(order.CustomerEmail, order.Id);
            
            return OrderResult.Success(order.Id, paymentResult.TransactionId);
        }
        catch (Exception ex)
        {
            // KISS: Simple error handling
            return OrderResult.Failure($"Order processing failed: {ex.Message}");
        }
    }
    
    // DRY: Reusable validation logic
    private ValidationResult ValidateOrder(Order order)
    {
        if (order == null)
            return ValidationResult.Invalid("Order cannot be null");
        
        if (string.IsNullOrWhiteSpace(order.CustomerEmail))
            return ValidationResult.Invalid("Customer email is required");
        
        if (order.Items == null || !order.Items.Any())
            return ValidationResult.Invalid("Order must contain at least one item");
        
        if (order.Total <= 0)
            return ValidationResult.Invalid("Order total must be greater than zero");
        
        return ValidationResult.Valid();
    }
}

// Supporting classes demonstrating the principles
public class ValidationResult
{
    public bool IsValid { get; private set; }
    public string ErrorMessage { get; private set; }
    
    private ValidationResult(bool isValid, string errorMessage)
    {
        IsValid = isValid;
        ErrorMessage = errorMessage;
    }
    
    public static ValidationResult Valid() => new ValidationResult(true, null);
    public static ValidationResult Invalid(string errorMessage) => new ValidationResult(false, errorMessage);
}

public class OrderResult
{
    public bool IsSuccessful { get; private set; }
    public string OrderId { get; private set; }
    public string TransactionId { get; private set; }
    public string ErrorMessage { get; private set; }
    
    private OrderResult(bool isSuccessful, string orderId, string transactionId, string errorMessage)
    {
        IsSuccessful = isSuccessful;
        OrderId = orderId;
        TransactionId = transactionId;
        ErrorMessage = errorMessage;
    }
    
    public static OrderResult Success(string orderId, string transactionId) =>
        new OrderResult(true, orderId, transactionId, null);
    
    public static OrderResult Failure(string errorMessage) =>
        new OrderResult(false, null, null, errorMessage);
}

// Demo showing balanced approach
public class BalancedDemo
{
    public static async Task RunDemo()
    {
        Console.WriteLine("⚖️ Balancing KISS, YAGNI, and DRY");
        Console.WriteLine("==================================\n");
        
        var paymentService = new MockPaymentService();
        var inventoryService = new MockInventoryService();
        var notificationService = new MockNotificationService();
        
        var processor = new OrderProcessor(paymentService, inventoryService, notificationService);
        
        var order = new Order
        {
            Id = "ORD001",
            CustomerEmail = "customer@example.com",
            Items = new List<OrderItem>
            {
                new OrderItem { ProductId = "P001", Quantity = 2, Price = 25.00m }
            },
            Total = 50.00m,
            PaymentInfo = new PaymentInfo { CardNumber = "1234-5678-9012-3456" }
        };
        
        var result = await processor.ProcessOrderAsync(order);
        
        if (result.IsSuccessful)
        {
            Console.WriteLine($"✅ Order processed successfully!");
            Console.WriteLine($"   Order ID: {result.OrderId}");
            Console.WriteLine($"   Transaction ID: {result.TransactionId}");
        }
        else
        {
            Console.WriteLine($"❌ Order processing failed: {result.ErrorMessage}");
        }
        
        Console.WriteLine("\n💡 This example demonstrates:");
        Console.WriteLine("   🎨 KISS: Simple, readable order processing flow");
        Console.WriteLine("   🔮 YAGNI: Only email notifications (can add SMS when needed)");
        Console.WriteLine("   🔁 DRY: Reusable validation logic");
        Console.WriteLine("   ⚖️ Balance: Right amount of abstraction without over-engineering");
    }
}
```

## Key Takeaways 🎯

1. **KISS - Keep It Simple**: 
   - Choose the simplest solution that works
   - Prefer readability over cleverness
   - Simple code is easier to maintain and debug

2. **YAGNI - You Aren't Gonna Need It**:
   - Don't build features for hypothetical future needs
   - Focus on current, well-understood requirements
   - You can always add features later when they're actually needed

3. **DRY - Don't Repeat Yourself**:
   - Avoid code duplication
   - Extract common functionality into reusable components
   - But don't abstract prematurely - sometimes duplication is better

4. **Balancing the Principles**:
   - Sometimes they conflict - use judgment
   - KISS might favor some duplication over complex abstraction
   - YAGNI prevents over-applying DRY too early
   - All three work together to create pragmatic solutions

5. **When to Break the Rules**:
   - Performance requirements might justify complexity
   - Domain complexity can't always be simplified
   - Early-stage development might benefit from some duplication

Remember: These principles guide us toward practical, maintainable solutions that solve real problems without unnecessary complexity! 🌟

## Exercises

1. **Easy**: Refactor a calculator with repeated validation code to follow DRY principle.
2. **Medium**: Simplify an over-engineered configuration system that violates KISS and YAGNI.
3. **Hard**: Find the balance between DRY and KISS in a reporting system with similar but not identical report types.

## Chapter Checklist

After completing this chapter, you should be able to:

- [ ] Apply KISS principle to choose simple solutions over complex ones
- [ ] Use YAGNI to avoid building unnecessary features
- [ ] Apply DRY to eliminate harmful code duplication
- [ ] Recognize when these principles conflict and how to balance them
- [ ] Identify premature optimization and over-engineering
- [ ] Refactor code to be simpler and more maintainable
- [ ] Know when it's okay to break these rules
- [ ] Balance between under-engineering and over-engineering
- [ ] Write pragmatic code that serves real business needs