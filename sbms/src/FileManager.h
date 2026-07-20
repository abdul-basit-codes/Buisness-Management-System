#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <vector>
#include <string>
#include "Customer.h"
#include "Employee.h"
#include "Service.h"
#include "Booking.h"

namespace FileManager {
    std::vector<Customer> loadCustomers(const std::string& path);
    void saveCustomers(const std::string& path, const std::vector<Customer>& customers);

    std::vector<Employee> loadEmployees(const std::string& path);
    void saveEmployees(const std::string& path, const std::vector<Employee>& employees);

    std::vector<Service> loadServices(const std::string& path);
    void saveServices(const std::string& path, const std::vector<Service>& services);

    std::vector<Booking> loadBookings(const std::string& path);
    void saveBookings(const std::string& path, const std::vector<Booking>& bookings);
}

#endif
