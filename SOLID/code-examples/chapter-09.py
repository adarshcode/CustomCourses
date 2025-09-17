# FILE: chapter-09-isp.py
# Chapter 9 — Interface Segregation Principle — Beginner/Intermediate

from abc import ABC, abstractmethod
from typing import List

# BAD EXAMPLE - Violates ISP (Fat Interface)
class BadPrinter(ABC):
    @abstractmethod
    def print_document(self, document: str) -> None:
        pass
    
    @abstractmethod
    def scan_document(self, document: str) -> None:  # Not all printers can scan
        pass
    
    @abstractmethod
    def fax_document(self, document: str) -> None:   # Not all printers can fax
        pass
    
    @abstractmethod
    def email_document(self, document: str) -> None: # Not all printers can email
        pass

class SimplePrinter(BadPrinter):
    def print_document(self, document: str) -> None:
        print(f"Printing: {document}")
    
    # Forced to implement methods it doesn't support - ISP violation!
    def scan_document(self, document: str) -> None:
        raise NotImplementedError("Simple printer cannot scan")
    
    def fax_document(self, document: str) -> None:
        raise NotImplementedError("Simple printer cannot fax")
    
    def email_document(self, document: str) -> None:
        raise NotImplementedError("Simple printer cannot email")

# GOOD EXAMPLE - Follows ISP (Segregated Interfaces)

# Separate interfaces for different capabilities
class Printable(ABC):
    @abstractmethod
    def print_document(self, document: str) -> None:
        pass

class Scannable(ABC):
    @abstractmethod
    def scan_document(self, document: str) -> None:
        pass

class Faxable(ABC):
    @abstractmethod
    def fax_document(self, document: str) -> None:
        pass

class Emailable(ABC):
    @abstractmethod
    def email_document(self, document: str) -> None:
        pass

# Simple printer only implements what it can do
class GoodSimplePrinter(Printable):
    def print_document(self, document: str) -> None:
        print(f"Simple printer printing: {document}")

# Multifunction printer implements multiple interfaces
class MultiFunctionPrinter(Printable, Scannable, Faxable, Emailable):
    def print_document(self, document: str) -> None:
        print(f"MFP printing: {document}")
    
    def scan_document(self, document: str) -> None:
        print(f"MFP scanning: {document}")
    
    def fax_document(self, document: str) -> None:
        print(f"MFP faxing: {document}")
    
    def email_document(self, document: str) -> None:
        print(f"MFP emailing: {document}")

# Scanner that only scans and emails
class NetworkScanner(Scannable, Emailable):
    def scan_document(self, document: str) -> None:
        print(f"Network scanner scanning: {document}")
    
    def email_document(self, document: str) -> None:
        print(f"Network scanner emailing: {document}")

# Office class that uses different printer capabilities
class Office:
    def __init__(self):
        self.printers: List[Printable] = []
        self.scanners: List[Scannable] = []
    
    def add_printer(self, printer: Printable) -> None:
        self.printers.append(printer)
    
    def add_scanner(self, scanner: Scannable) -> None:
        self.scanners.append(scanner)
    
    def print_documents(self, document: str) -> None:
        print(f"\nPrinting '{document}' on all available printers:")
        for printer in self.printers:
            printer.print_document(document)
    
    def scan_documents(self, document: str) -> None:
        print(f"\nScanning '{document}' on all available scanners:")
        for scanner in self.scanners:
            scanner.scan_document(document)

def main():
    print("=== ISP Violation Demo ===")
    
    bad_printer = SimplePrinter()
    bad_printer.print_document("Report.pdf")  # Works fine
    
    try:
        bad_printer.scan_document("Invoice.pdf")  # Will raise exception - ISP violation!
    except NotImplementedError as e:
        print(f"Error: {e}")

    print("\n=== ISP Compliant Demo ===")
    
    # Demonstrate ISP benefits
    office = Office()
    
    # Create different devices
    simple_printer = GoodSimplePrinter()
    mfp = MultiFunctionPrinter()
    network_scanner = NetworkScanner()
    
    # Add devices based on their capabilities
    office.add_printer(simple_printer)  # Can only print
    office.add_printer(mfp)            # Can print (among other things)
    
    office.add_scanner(mfp)            # Can scan (among other things)
    office.add_scanner(network_scanner) # Can scan (among other things)
    
    # Use devices through segregated interfaces
    office.print_documents("Monthly Report")
    office.scan_documents("Invoice")
    
    print("\n=== ISP Benefits ===")
    print("✓ Simple printer only implements print - no unused methods")
    print("✓ Office can use any printer without knowing its other capabilities")
    print("✓ Easy to add new device types with different capability combinations")
    print("✓ No risk of calling unsupported methods")
    print("✓ Each interface is focused and cohesive")
    
    # Demonstrate flexibility - can treat MFP as different device types
    print("\n=== Flexibility Demo ===")
    just_printer: Printable = mfp  # MFP used only as printer
    just_scanner: Scannable = mfp  # Same MFP used only as scanner
    
    just_printer.print_document("Using MFP as just a printer")
    just_scanner.scan_document("Using MFP as just a scanner")

if __name__ == "__main__":
    main()