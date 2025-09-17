// FILE: chapter-13.cs
// Chapter 13 — KISS, YAGNI, DRY — C#

using System;
using System.Collections.Generic;
using System.Linq;

// ❌ DRY Violation - Repeated validation logic
class BadOrderProcessor
{
    public void ProcessCustomerOrder(string customerName, string email, decimal amount)
    {
        // Repeated validation
        if (string.IsNullOrEmpty(customerName)) throw new ArgumentException("Name required");
        if (string.IsNullOrEmpty(email)) throw new ArgumentException("Email required");
        if (amount <= 0) throw new ArgumentException("Amount must be positive");
        
        Console.WriteLine($"Processing customer order: {customerName}, {email}, ${amount}");
    }
    
    public void ProcessSupplierOrder(string supplierName, string email, decimal amount)
    {
        // Same validation repeated!
        if (string.IsNullOrEmpty(supplierName)) throw new ArgumentException("Name required");
        if (string.IsNullOrEmpty(email)) throw new ArgumentException("Email required");
        if (amount <= 0) throw new ArgumentException("Amount must be positive");
        
        Console.WriteLine($"Processing supplier order: {supplierName}, {email}, ${amount}");
    }
}

// ✅ DRY Solution - Extract common validation
class OrderValidator
{
    public void ValidateOrder(string name, string email, decimal amount)
    {
        if (string.IsNullOrEmpty(name)) throw new ArgumentException("Name required");
        if (string.IsNullOrEmpty(email)) throw new ArgumentException("Email required");
        if (amount <= 0) throw new ArgumentException("Amount must be positive");
    }
}

// KISS - Simple order processor
class SimpleOrderProcessor
{
    private readonly OrderValidator validator = new OrderValidator();
    
    public void ProcessOrder(string name, string email, decimal amount, string orderType)
    {
        // YAGNI - Only handle current requirements, not hypothetical future ones
        validator.ValidateOrder(name, email, amount);
        Console.WriteLine($"Processing {orderType} order: {name}, {email}, ${amount}");
    }
}

class Program
{
    static void Main()
    {
        Console.WriteLine("🎯 KISS, YAGNI, DRY Principles (C#)");
        Console.WriteLine("===================================\n");
        
        var processor = new SimpleOrderProcessor();
        
        // Process different types of orders using same logic (DRY)
        processor.ProcessOrder("John Customer", "john@example.com", 100.00m, "customer");
        processor.ProcessOrder("ABC Supplier", "abc@supplier.com", 500.00m, "supplier");
        
        Console.WriteLine("\n💡 Principles Applied:");
        Console.WriteLine("   🎨 KISS: Simple, straightforward processing");
        Console.WriteLine("   🔮 YAGNI: Only current features, no speculation");
        Console.WriteLine("   🔁 DRY: Shared validation logic");
    }
}