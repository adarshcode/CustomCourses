// FILE: chapter-09-isp.cpp
// Chapter 9 — Interface Segregation Principle — Beginner/Intermediate

#include <iostream>
#include <vector>
#include <memory>
#include <stdexcept>

// BAD EXAMPLE - Violates ISP (Fat Interface)
class BadPrinter {
public:
    virtual ~BadPrinter() = default;
    virtual void print(const std::string& document) = 0;
    virtual void scan(const std::string& document) = 0;    // Not all printers can scan
    virtual void fax(const std::string& document) = 0;     // Not all printers can fax
    virtual void email(const std::string& document) = 0;   // Not all printers can email
};

class SimplePrinter : public BadPrinter {
public:
    void print(const std::string& document) override {
        std::cout << "Printing: " << document << std::endl;
    }
    
    // Forced to implement methods it doesn't support - ISP violation!
    void scan(const std::string& document) override {
        throw std::runtime_error("Simple printer cannot scan");
    }
    
    void fax(const std::string& document) override {
        throw std::runtime_error("Simple printer cannot fax");
    }
    
    void email(const std::string& document) override {
        throw std::runtime_error("Simple printer cannot email");
    }
};

// GOOD EXAMPLE - Follows ISP (Segregated Interfaces)

// Separate interfaces for different capabilities
class Printable {
public:
    virtual ~Printable() = default;
    virtual void print(const std::string& document) = 0;
};

class Scannable {
public:
    virtual ~Scannable() = default;
    virtual void scan(const std::string& document) = 0;
};

class Faxable {
public:
    virtual ~Faxable() = default;
    virtual void fax(const std::string& document) = 0;
};

class Emailable {
public:
    virtual ~Emailable() = default;
    virtual void email(const std::string& document) = 0;
};

// Simple printer only implements what it can do
class GoodSimplePrinter : public Printable {
public:
    void print(const std::string& document) override {
        std::cout << "Simple printer printing: " << document << std::endl;
    }
};

// Multifunction printer implements multiple interfaces
class MultiFunctionPrinter : public Printable, public Scannable, public Faxable, public Emailable {
public:
    void print(const std::string& document) override {
        std::cout << "MFP printing: " << document << std::endl;
    }
    
    void scan(const std::string& document) override {
        std::cout << "MFP scanning: " << document << std::endl;
    }
    
    void fax(const std::string& document) override {
        std::cout << "MFP faxing: " << document << std::endl;
    }
    
    void email(const std::string& document) override {
        std::cout << "MFP emailing: " << document << std::endl;
    }
};

// Scanner that only scans and emails
class NetworkScanner : public Scannable, public Emailable {
public:
    void scan(const std::string& document) override {
        std::cout << "Network scanner scanning: " << document << std::endl;
    }
    
    void email(const std::string& document) override {
        std::cout << "Network scanner emailing: " << document << std::endl;
    }
};

// Office class that uses different printer capabilities
class Office {
private:
    std::vector<std::unique_ptr<Printable>> printers;
    std::vector<std::unique_ptr<Scannable>> scanners;

public:
    void addPrinter(std::unique_ptr<Printable> printer) {
        printers.push_back(std::move(printer));
    }
    
    void addScanner(std::unique_ptr<Scannable> scanner) {
        scanners.push_back(std::move(scanner));
    }
    
    void printDocuments(const std::string& document) {
        std::cout << "\nPrinting '" << document << "' on all available printers:" << std::endl;
        for (const auto& printer : printers) {
            printer->print(document);
        }
    }
    
    void scanDocuments(const std::string& document) {
        std::cout << "\nScanning '" << document << "' on all available scanners:" << std::endl;
        for (const auto& scanner : scanners) {
            scanner->scan(document);
        }
    }
};

int main() {
    std::cout << "=== ISP Violation Demo ===" << std::endl;
    
    SimplePrinter badPrinter;
    badPrinter.print("Report.pdf");  // Works fine
    
    try {
        badPrinter.scan("Invoice.pdf");  // Will throw exception - ISP violation!
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }

    std::cout << "\n=== ISP Compliant Demo ===" << std::endl;
    
    // Create different devices
    auto simplePrinter = std::make_unique<GoodSimplePrinter>();
    auto mfp1 = std::make_unique<MultiFunctionPrinter>();
    auto mfp2 = std::make_unique<MultiFunctionPrinter>();
    auto networkScanner = std::make_unique<NetworkScanner>();
    
    // Demonstrate ISP benefits
    Office office;
    
    // Add devices based on their capabilities
    office.addPrinter(std::move(simplePrinter));  // Can only print
    office.addPrinter(std::move(mfp1));          // Can print (among other things)
    
    office.addScanner(std::move(mfp2));          // Can scan (among other things)
    office.addScanner(std::move(networkScanner)); // Can scan (among other things)
    
    // Use devices through segregated interfaces
    office.printDocuments("Monthly Report");
    office.scanDocuments("Invoice");
    
    std::cout << "\n=== ISP Benefits ===" << std::endl;
    std::cout << "✓ Simple printer only implements print - no unused methods" << std::endl;
    std::cout << "✓ Office can use any printer without knowing its other capabilities" << std::endl;
    std::cout << "✓ Easy to add new device types with different capability combinations" << std::endl;
    std::cout << "✓ No risk of calling unsupported methods" << std::endl;
    std::cout << "✓ Each interface is focused and cohesive" << std::endl;
    
    // Demonstrate flexibility with references
    std::cout << "\n=== Flexibility Demo ===" << std::endl;
    MultiFunctionPrinter flexibleMFP;
    
    // Same MFP object used through different interfaces
    Printable& justPrinter = flexibleMFP;   // MFP used only as printer
    Scannable& justScanner = flexibleMFP;   // Same MFP used only as scanner
    
    justPrinter.print("Using MFP as just a printer");
    justScanner.scan("Using MFP as just a scanner");
    
    return 0;
}