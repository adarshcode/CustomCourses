// FILE: chapter-09-isp.cs
// Chapter 9 — Interface Segregation Principle — Beginner/Intermediate

using System;
using System.Collections.Generic;

namespace ISPExample
{
    // BAD EXAMPLE - Violates ISP (Fat Interface)
    public interface IBadPrinter
    {
        void Print(string document);
        void Scan(string document);     // Not all printers can scan
        void Fax(string document);      // Not all printers can fax
        void Email(string document);    // Not all printers can email
    }

    public class SimplePrinter : IBadPrinter
    {
        public void Print(string document)
        {
            Console.WriteLine($"Printing: {document}");
        }
        
        // Forced to implement methods it doesn't support - ISP violation!
        public void Scan(string document)
        {
            throw new NotSupportedException("Simple printer cannot scan");
        }
        
        public void Fax(string document)
        {
            throw new NotSupportedException("Simple printer cannot fax");
        }
        
        public void Email(string document)
        {
            throw new NotSupportedException("Simple printer cannot email");
        }
    }

    // GOOD EXAMPLE - Follows ISP (Segregated Interfaces)

    // Separate interfaces for different capabilities
    public interface IPrintable
    {
        void Print(string document);
    }

    public interface IScannable
    {
        void Scan(string document);
    }

    public interface IFaxable
    {
        void Fax(string document);
    }

    public interface IEmailable
    {
        void Email(string document);
    }

    // Simple printer only implements what it can do
    public class GoodSimplePrinter : IPrintable
    {
        public void Print(string document)
        {
            Console.WriteLine($"Simple printer printing: {document}");
        }
    }

    // Multifunction printer implements multiple interfaces
    public class MultiFunctionPrinter : IPrintable, IScannable, IFaxable, IEmailable
    {
        public void Print(string document)
        {
            Console.WriteLine($"MFP printing: {document}");
        }
        
        public void Scan(string document)
        {
            Console.WriteLine($"MFP scanning: {document}");
        }
        
        public void Fax(string document)
        {
            Console.WriteLine($"MFP faxing: {document}");
        }
        
        public void Email(string document)
        {
            Console.WriteLine($"MFP emailing: {document}");
        }
    }

    // Scanner that only scans and emails
    public class NetworkScanner : IScannable, IEmailable
    {
        public void Scan(string document)
        {
            Console.WriteLine($"Network scanner scanning: {document}");
        }
        
        public void Email(string document)
        {
            Console.WriteLine($"Network scanner emailing: {document}");
        }
    }

    // Office class that uses different printer capabilities
    public class Office
    {
        private List<IPrintable> printers = new List<IPrintable>();
        private List<IScannable> scanners = new List<IScannable>();
        
        public void AddPrinter(IPrintable printer)
        {
            printers.Add(printer);
        }
        
        public void AddScanner(IScannable scanner)
        {
            scanners.Add(scanner);
        }
        
        public void PrintDocuments(string document)
        {
            Console.WriteLine($"\nPrinting '{document}' on all available printers:");
            foreach (var printer in printers)
            {
                printer.Print(document);
            }
        }
        
        public void ScanDocuments(string document)
        {
            Console.WriteLine($"\nScanning '{document}' on all available scanners:");
            foreach (var scanner in scanners)
            {
                scanner.Scan(document);
            }
        }
    }

    class Program
    {
        static void Main()
        {
            Console.WriteLine("=== ISP Violation Demo ===");
            
            var badPrinter = new SimplePrinter();
            badPrinter.Print("Report.pdf"); // Works fine
            
            try
            {
                badPrinter.Scan("Invoice.pdf"); // Will throw exception - ISP violation!
            }
            catch (NotSupportedException ex)
            {
                Console.WriteLine($"Error: {ex.Message}");
            }

            Console.WriteLine("\n=== ISP Compliant Demo ===");
            
            // Demonstrate ISP benefits
            var office = new Office();
            
            // Create different devices
            var simplePrinter = new GoodSimplePrinter();
            var mfp = new MultiFunctionPrinter();
            var networkScanner = new NetworkScanner();
            
            // Add devices based on their capabilities
            office.AddPrinter(simplePrinter);  // Can only print
            office.AddPrinter(mfp);           // Can print (among other things)
            
            office.AddScanner(mfp);           // Can scan (among other things)
            office.AddScanner(networkScanner); // Can scan (among other things)
            
            // Use devices through segregated interfaces
            office.PrintDocuments("Monthly Report");
            office.ScanDocuments("Invoice");
            
            Console.WriteLine("\n=== ISP Benefits ===");
            Console.WriteLine("✓ Simple printer only implements Print - no unused methods");
            Console.WriteLine("✓ Office can use any printer without knowing its other capabilities");
            Console.WriteLine("✓ Easy to add new device types with different capability combinations");
            Console.WriteLine("✓ No risk of calling unsupported methods");
            Console.WriteLine("✓ Each interface is focused and cohesive");
            
            // Demonstrate flexibility - can treat MFP as different device types
            Console.WriteLine("\n=== Flexibility Demo ===");
            IPrintable justPrinter = mfp;  // MFP used only as printer
            IScannable justScanner = mfp;  // Same MFP used only as scanner
            
            justPrinter.Print("Using MFP as just a printer");
            justScanner.Scan("Using MFP as just a scanner");
        }
    }
}