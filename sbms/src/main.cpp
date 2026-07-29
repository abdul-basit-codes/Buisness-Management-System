#include <iostream>
#include <limits>
#include <algorithm>
#include "Customer.h"
#include "Employee.h"
#include "Service.h"
#include "Booking.h"
#include "FileManager.h"
#include "Invoice.h"
#include "Report.h"
#include "Payroll.h"

const std::string APP_NAME = "Small Business Management System";
const std::string APP_VERSION = "1.0.0";

const std::string CUSTOMERS_FILE = "data/customers.txt";
const std::string EMPLOYEES_FILE = "data/employees.txt";
const std::string SERVICES_FILE  = "data/services.txt";
const std::string BOOKINGS_FILE  = "data/bookings.txt";
const std::string INVOICES_FILE  = "data/invoices.txt";

std::string promptNonEmpty(const std::string& label) {
    std::string value;
    while (true) {
        std::cout << label;
        std::getline(std::cin, value);
        if (!value.empty()) break;
        std::cout << "This field cannot be empty. Try again.\n";
    }
    return value;
}

double promptPositiveDouble(const std::string& label) {
    double value;
    while (true) {
        std::cout << label;
        std::cin >> value;
        if (std::cin.fail() || value < 0) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Please enter a valid non-negative number.\n";
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return value;
    }
}

int nextId(const std::vector<Customer>& items) {
    int maxId = 0;
    for (const auto& i : items) maxId = std::max(maxId, i.getId());
    return maxId + 1;
}

int nextId(const std::vector<Employee>& items) {
    int maxId = 0;
    for (const auto& i : items) maxId = std::max(maxId, i.getId());
    return maxId + 1;
}

void payrollMenu() {
    std::vector<Employee> employees = FileManager::loadEmployees(EMPLOYEES_FILE);
    if (employees.empty()) { std::cout << "No employees found. Add employees first.\n"; return; }

    double bonusPct, taxPct;
    std::cout << "\n--- Payroll Processing ---\n";
    std::cout << "Employees on file: " << employees.size() << "\n";
    std::cout << "Enter bonus percentage (e.g. 5 for 5%): ";
    std::cin >> bonusPct;
    std::cout << "Enter tax/deduction percentage (e.g. 10 for 10%): ";
    std::cin >> taxPct;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    auto payroll = Payroll::generatePayroll(employees, bonusPct, taxPct);
    Payroll::printPayroll(payroll);
}

void reportsMenu() {
    std::vector<Booking> bookings = FileManager::loadBookings(BOOKINGS_FILE);
    std::vector<Customer> customers = FileManager::loadCustomers(CUSTOMERS_FILE);
    std::vector<Service> services = FileManager::loadServices(SERVICES_FILE);
    std::vector<Employee> employees = FileManager::loadEmployees(EMPLOYEES_FILE);
    std::vector<Invoice> invoices = FileManager::loadInvoices(INVOICES_FILE);

    int choice = -1;
    while (choice != 0) {
        std::cout << "\n--- Reports ---\n";
        std::cout << "1. Business Overview\n2. Sales Summary\n3. Popular Services\n0. Back\nChoice: ";
        std::cin >> choice;
        if (std::cin.fail()) { std::cin.clear(); std::cin.ignore(10000, '\n'); continue; }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == 1) {
            int completed = 0, pending = 0;
            for (const auto& b : bookings) {
                if (b.isCompleted()) completed++; else pending++;
            }
            std::cout << "\n========= BUSINESS OVERVIEW =========\n";
            std::cout << "Total Customers:     " << customers.size() << "\n";
            std::cout << "Total Employees:     " << employees.size() << "\n";
            std::cout << "Total Services:      " << services.size() << "\n";
            std::cout << "Total Bookings:      " << bookings.size() << "\n";
            std::cout << "  Completed:         " << completed << "\n";
            std::cout << "  Pending:           " << pending << "\n";
            std::cout << "Total Invoices:      " << invoices.size() << "\n";
            std::cout << "======================================\n";
        } else if (choice == 2) {
            auto summary = Report::generateSalesSummary(invoices, services);
            Report::printSalesSummary(summary);
        } else if (choice == 3) {
            auto pop = Report::popularServices(invoices);
            if (pop.empty()) std::cout << "No invoice data yet.\n";
            else Report::printPopularServices(pop, services);
        }
    }
}

void billingMenu() {
    std::vector<Booking> bookings = FileManager::loadBookings(BOOKINGS_FILE);
    std::vector<Customer> customers = FileManager::loadCustomers(CUSTOMERS_FILE);
    std::vector<Service> services = FileManager::loadServices(SERVICES_FILE);
    std::vector<Invoice> invoices = FileManager::loadInvoices(INVOICES_FILE);

    int choice = -1;
    while (choice != 0) {
        std::cout << "\n--- Billing & Invoicing ---\n";
        std::cout << "1. Generate Invoice from Booking\n2. View All Invoices\n3. View Sales Report\n0. Back\nChoice: ";
        std::cin >> choice;
        if (std::cin.fail()) { std::cin.clear(); std::cin.ignore(10000, '\n'); continue; }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == 1) {
            int bookingId;
            std::cout << "Enter Booking ID: ";
            std::cin >> bookingId;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            const Booking* booking = nullptr;
            for (const auto& b : bookings) {
                if (b.getId() == bookingId) { booking = &b; break; }
            }
            if (!booking) { std::cout << "Booking not found.\n"; continue; }

            Invoice inv(nextId(invoices), booking->getCustomerId(), {booking->getServiceId()}, {1}, 0.05);
            invoices.push_back(inv);
            FileManager::saveInvoices(INVOICES_FILE, invoices);
            inv.print(services);
        } else if (choice == 2) {
            if (invoices.empty()) std::cout << "No invoices yet.\n";
            for (const auto& inv : invoices) inv.print(services);
        } else if (choice == 3) {
            Report::printDailyReport(invoices, services);
        }
    }
}

int nextId(const std::vector<Booking>& items) {
    int maxId = 0;
    for (const auto& i : items) maxId = std::max(maxId, i.getId());
    return maxId + 1;
}

int nextId(const std::vector<Invoice>& items) {
    int maxId = 0;
    for (const auto& i : items) maxId = std::max(maxId, i.getId());
    return maxId + 1;
}

void bookingMenu() {
    std::vector<Booking> bookings = FileManager::loadBookings(BOOKINGS_FILE);
    std::vector<Customer> customers = FileManager::loadCustomers(CUSTOMERS_FILE);
    std::vector<Service> services = FileManager::loadServices(SERVICES_FILE);
    std::vector<Employee> employees = FileManager::loadEmployees(EMPLOYEES_FILE);

    int choice = -1;
    while (choice != 0) {
        std::cout << "\n--- Booking Management ---\n";
        std::cout << "1. Create Booking\n2. View All\n3. Mark Completed\n4. Cancel Booking\n0. Back\nChoice: ";
        std::cin >> choice;
        if (std::cin.fail()) { std::cin.clear(); std::cin.ignore(10000, '\n'); continue; }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == 1) {
            if (customers.empty() || services.empty() || employees.empty()) {
                std::cout << "Need at least one customer, service, and employee before booking.\n";
                continue;
            }
            int custId, servId, empId;
            std::string date;
            std::cout << "Customer ID: "; std::cin >> custId;
            std::cout << "Service ID: "; std::cin >> servId;
            std::cout << "Employee ID: "; std::cin >> empId;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            auto exists = [](int id, auto& vec) {
                for (const auto& item : vec) if (item.getId() == id) return true;
                return false;
            };
            if (!exists(custId, customers) || !exists(servId, services) || !exists(empId, employees)) {
                std::cout << "One or more IDs do not exist. Booking not created.\n";
                continue;
            }

            date = promptNonEmpty("Date (YYYY-MM-DD): ");
            Booking b(nextId(bookings), custId, servId, empId, date, false);
            bookings.push_back(b);
            FileManager::saveBookings(BOOKINGS_FILE, bookings);
            std::cout << "Booking created with ID " << b.getId() << "\n";
        } else if (choice == 2) {
            if (bookings.empty()) std::cout << "No bookings yet.\n";
            for (const auto& b : bookings) b.print();
        } else if (choice == 3) {
            int id; std::cout << "Enter booking ID to mark completed: "; std::cin >> id;
            bool found = false;
            for (auto& b : bookings) {
                if (b.getId() == id) {
                    found = true;
                    b.setCompleted(true);
                    FileManager::saveBookings(BOOKINGS_FILE, bookings);
                    std::cout << "Marked completed.\n";
                    break;
                }
            }
            if (!found) std::cout << "Booking not found.\n";
        } else if (choice == 4) {
            int id; std::cout << "Enter booking ID to cancel: "; std::cin >> id;
            auto it = std::remove_if(bookings.begin(), bookings.end(),
                [id](const Booking& b) { return b.getId() == id; });
            if (it != bookings.end()) {
                bookings.erase(it, bookings.end());
                FileManager::saveBookings(BOOKINGS_FILE, bookings);
                std::cout << "Cancelled.\n";
            } else {
                std::cout << "Booking not found.\n";
            }
        }
    }
}

int nextId(const std::vector<Service>& items) {
    int maxId = 0;
    for (const auto& i : items) maxId = std::max(maxId, i.getId());
    return maxId + 1;
}

void serviceMenu() {
    std::vector<Service> services = FileManager::loadServices(SERVICES_FILE);
    int choice = -1;
    while (choice != 0) {
        std::cout << "\n--- Service Management ---\n";
        std::cout << "1. Add Service\n2. View All\n3. Edit Service\n4. Delete Service\n0. Back\nChoice: ";
        std::cin >> choice;
        if (std::cin.fail()) { std::cin.clear(); std::cin.ignore(10000, '\n'); continue; }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == 1) {
            std::string name = promptNonEmpty("Name: ");
            double price = promptPositiveDouble("Price: ");
            double durationD = promptPositiveDouble("Duration (minutes): ");
            int duration = static_cast<int>(durationD);
            Service s(nextId(services), name, price, duration);
            services.push_back(s);
            FileManager::saveServices(SERVICES_FILE, services);
            std::cout << "Service added with ID " << s.getId() << "\n";
        } else if (choice == 2) {
            if (services.empty()) std::cout << "No services yet.\n";
            for (const auto& s : services) s.print();
        } else if (choice == 3) {
            int id; std::cout << "Enter service ID to edit: "; std::cin >> id;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            bool found = false;
            for (auto& s : services) {
                if (s.getId() == id) {
                    found = true;
                    std::string name = promptNonEmpty("New Name: ");
                    double price = promptPositiveDouble("New Price: ");
                    double durationD = promptPositiveDouble("New Duration (minutes): ");
                    int duration = static_cast<int>(durationD);
                    s.setName(name); s.setPrice(price); s.setDuration(duration);
                    FileManager::saveServices(SERVICES_FILE, services);
                    std::cout << "Updated.\n";
                    break;
                }
            }
            if (!found) std::cout << "Service not found.\n";
        } else if (choice == 4) {
            int id; std::cout << "Enter service ID to delete: "; std::cin >> id;
            auto it = std::remove_if(services.begin(), services.end(),
                [id](const Service& s) { return s.getId() == id; });
            if (it != services.end()) {
                services.erase(it, services.end());
                FileManager::saveServices(SERVICES_FILE, services);
                std::cout << "Deleted.\n";
            } else {
                std::cout << "Service not found.\n";
            }
        }
    }
}

void employeeMenu() {
    std::vector<Employee> employees = FileManager::loadEmployees(EMPLOYEES_FILE);
    int choice = -1;
    while (choice != 0) {
        std::cout << "\n--- Employee Management ---\n";
        std::cout << "1. Add Employee\n2. View All\n3. Edit Employee\n4. Delete Employee\n0. Back\nChoice: ";
        std::cin >> choice;
        if (std::cin.fail()) { std::cin.clear(); std::cin.ignore(10000, '\n'); continue; }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == 1) {
            std::string name = promptNonEmpty("Name: ");
            std::string role = promptNonEmpty("Role: ");
            double salary = promptPositiveDouble("Salary: ");
            Employee e(nextId(employees), name, role, salary);
            employees.push_back(e);
            FileManager::saveEmployees(EMPLOYEES_FILE, employees);
            std::cout << "Employee added with ID " << e.getId() << "\n";
        } else if (choice == 2) {
            if (employees.empty()) std::cout << "No employees yet.\n";
            for (const auto& e : employees) e.print();
        } else if (choice == 3) {
            int id; std::cout << "Enter employee ID to edit: "; std::cin >> id;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            bool found = false;
            for (auto& e : employees) {
                if (e.getId() == id) {
                    found = true;
                    std::string name = promptNonEmpty("New Name: ");
                    std::string role = promptNonEmpty("New Role: ");
                    double salary = promptPositiveDouble("New Salary: ");
                    e.setName(name); e.setRole(role); e.setSalary(salary);
                    FileManager::saveEmployees(EMPLOYEES_FILE, employees);
                    std::cout << "Updated.\n";
                    break;
                }
            }
            if (!found) std::cout << "Employee not found.\n";
        } else if (choice == 4) {
            int id; std::cout << "Enter employee ID to delete: "; std::cin >> id;
            auto it = std::remove_if(employees.begin(), employees.end(),
                [id](const Employee& e) { return e.getId() == id; });
            if (it != employees.end()) {
                employees.erase(it, employees.end());
                FileManager::saveEmployees(EMPLOYEES_FILE, employees);
                std::cout << "Deleted.\n";
            } else {
                std::cout << "Employee not found.\n";
            }
        }
    }
}

void customerMenu() {
    std::vector<Customer> customers = FileManager::loadCustomers(CUSTOMERS_FILE);
    int choice = -1;
    while (choice != 0) {
        std::cout << "\n--- Customer Management ---\n";
        std::cout << "1. Add Customer\n2. View All\n3. Edit Customer\n4. Delete Customer\n0. Back\nChoice: ";
        std::cin >> choice;
        if (std::cin.fail()) { std::cin.clear(); std::cin.ignore(10000, '\n'); continue; }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == 1) {
            std::string name = promptNonEmpty("Name: ");
            std::string phone = promptNonEmpty("Phone: ");
            std::string address = promptNonEmpty("Address: ");
            Customer c(nextId(customers), name, phone, address);
            customers.push_back(c);
            FileManager::saveCustomers(CUSTOMERS_FILE, customers);
            std::cout << "Customer added with ID " << c.getId() << "\n";
        } else if (choice == 2) {
            if (customers.empty()) std::cout << "No customers yet.\n";
            for (const auto& c : customers) c.print();
        } else if (choice == 3) {
            int id; std::cout << "Enter customer ID to edit: "; std::cin >> id;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            bool found = false;
            for (auto& c : customers) {
                if (c.getId() == id) {
                    found = true;
                    std::string name = promptNonEmpty("New Name: ");
                    std::string phone = promptNonEmpty("New Phone: ");
                    std::string address = promptNonEmpty("New Address: ");
                    c.setName(name); c.setPhone(phone); c.setAddress(address);
                    FileManager::saveCustomers(CUSTOMERS_FILE, customers);
                    std::cout << "Updated.\n";
                    break;
                }
            }
            if (!found) std::cout << "Customer not found.\n";
        } else if (choice == 4) {
            int id; std::cout << "Enter customer ID to delete: "; std::cin >> id;
            auto it = std::remove_if(customers.begin(), customers.end(),
                [id](const Customer& c) { return c.getId() == id; });
            if (it != customers.end()) {
                customers.erase(it, customers.end());
                FileManager::saveCustomers(CUSTOMERS_FILE, customers);
                std::cout << "Deleted.\n";
            } else {
                std::cout << "Customer not found.\n";
            }
        }
    }
}

void showMainMenu() {
    std::cout << "\n===== Small Business Management System =====\n";
    std::cout << "1. Customer Management\n";
    std::cout << "2. Employee Management\n";
    std::cout << "3. Service Management\n";
    std::cout << "4. Booking Management\n";
    std::cout << "5. Payroll Processing\n";
    std::cout << "6. Billing & Invoicing\n";
    std::cout << "7. Reports\n";
    std::cout << "8. Daily Report (Quick)\n";
    std::cout << "0. Exit\n";
    std::cout << "Choice: ";
}

int main() {
    std::cout << APP_NAME << " v" << APP_VERSION << "\n";
    int choice = -1;
    while (choice != 0) {
        showMainMenu();
        std::cin >> choice;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Invalid input.\n";
            continue;
        }
        switch (choice) {
            case 1: customerMenu(); break;
            case 2: employeeMenu(); break;
            case 3: serviceMenu(); break;
            case 4: bookingMenu(); break;
            case 5: payrollMenu(); break;
            case 6: billingMenu(); break;
            case 7: reportsMenu(); break;
            case 8: {
                auto invoices = FileManager::loadInvoices(INVOICES_FILE);
                auto services = FileManager::loadServices(SERVICES_FILE);
                Report::printDailyReport(invoices, services);
                break;
            }
            case 0: std::cout << "Goodbye!\n"; break;
            default: std::cout << "Invalid choice.\n";
        }
    }
    return 0;
}
