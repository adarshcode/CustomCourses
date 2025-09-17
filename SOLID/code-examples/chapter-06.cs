// FILE: chapter-06-basic.cs
// Chapter 6 — Single Responsibility Principle — Basic

using System;
using System.IO;

// BAD EXAMPLE - Violates SRP
public class EmployeeBad
{
    public string Name { get; set; }
    public double Salary { get; set; }
    
    // Responsibility 1: Calculate pay
    public double CalculatePay()
    {
        return Salary / 12; // Monthly pay
    }
    
    // Responsibility 2: Save to database
    public void SaveToDatabase()
    {
        Console.WriteLine($"Saving {Name} to database...");
        // Database logic here
    }
    
    // Responsibility 3: Generate report
    public void PrintPayslip()
    {
        Console.WriteLine("=== PAYSLIP ===");
        Console.WriteLine($"Employee: {Name}");
        Console.WriteLine($"Monthly Pay: ${CalculatePay():F2}");
        Console.WriteLine("===============");
    }
}

// GOOD EXAMPLE - Follows SRP
// Each class has ONE responsibility

// Responsibility 1: Store employee data
public class Employee
{
    public string Name { get; set; }
    public double Salary { get; set; }
    
    public Employee(string name, double salary)
    {
        Name = name;
        Salary = salary;
    }
}

// Responsibility 2: Calculate employee pay
public class PayCalculator
{
    public double CalculateMonthlyPay(Employee employee)
    {
        return employee.Salary / 12;
    }
    
    public double CalculateWeeklyPay(Employee employee)
    {
        return employee.Salary / 52;
    }
}

// Responsibility 3: Save employee data
public class EmployeeRepository
{
    public void Save(Employee employee)
    {
        Console.WriteLine($"Saving {employee.Name} to database...");
        // Database logic here
    }
    
    public Employee Load(string name)
    {
        Console.WriteLine($"Loading {name} from database...");
        // Database logic here
        return new Employee(name, 50000);
    }
}

// Responsibility 4: Generate reports
public class PayslipGenerator
{
    private PayCalculator payCalculator;
    
    public PayslipGenerator()
    {
        payCalculator = new PayCalculator();
    }
    
    public void PrintPayslip(Employee employee)
    {
        double monthlyPay = payCalculator.CalculateMonthlyPay(employee);
        
        Console.WriteLine("=== PAYSLIP ===");
        Console.WriteLine($"Employee: {employee.Name}");
        Console.WriteLine($"Monthly Pay: ${monthlyPay:F2}");
        Console.WriteLine("===============");
    }
}

class Program
{
    static void Main()
    {
        Console.WriteLine("=== SRP Violation Example ===");
        var badEmployee = new EmployeeBad { Name = "John Bad", Salary = 60000 };
        badEmployee.PrintPayslip();
        badEmployee.SaveToDatabase();
        
        Console.WriteLine("\n=== SRP Correct Example ===");
        var employee = new Employee("John Good", 60000);
        var payCalculator = new PayCalculator();
        var repository = new EmployeeRepository();
        var payslipGenerator = new PayslipGenerator();
        
        // Each object does its ONE job
        repository.Save(employee);
        payslipGenerator.PrintPayslip(employee);
        
        Console.WriteLine($"\nWeekly pay: ${payCalculator.CalculateWeeklyPay(employee):F2}");
    }
}