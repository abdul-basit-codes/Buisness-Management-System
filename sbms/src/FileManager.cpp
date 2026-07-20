#include "FileManager.h"
#include <fstream>

namespace FileManager {

template <typename T>
static std::vector<T> loadGeneric(const std::string& path) {
    std::vector<T> items;
    std::ifstream file(path);
    if (!file.is_open()) return items;
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        items.push_back(T::fromCSV(line));
    }
    return items;
}

template <typename T>
static void saveGeneric(const std::string& path, const std::vector<T>& items) {
    std::ofstream file(path, std::ios::trunc);
    for (const auto& item : items) {
        file << item.toCSV() << "\n";
    }
}

std::vector<Customer> loadCustomers(const std::string& path) { return loadGeneric<Customer>(path); }
void saveCustomers(const std::string& path, const std::vector<Customer>& customers) { saveGeneric(path, customers); }

std::vector<Employee> loadEmployees(const std::string& path) { return loadGeneric<Employee>(path); }
void saveEmployees(const std::string& path, const std::vector<Employee>& employees) { saveGeneric(path, employees); }

std::vector<Service> loadServices(const std::string& path) { return loadGeneric<Service>(path); }
void saveServices(const std::string& path, const std::vector<Service>& services) { saveGeneric(path, services); }

std::vector<Booking> loadBookings(const std::string& path) { return loadGeneric<Booking>(path); }
void saveBookings(const std::string& path, const std::vector<Booking>& bookings) { saveGeneric(path, bookings); }

}
