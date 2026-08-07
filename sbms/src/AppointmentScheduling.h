#ifndef APPOINTMENT_SCHEDULING_H
#define APPOINTMENT_SCHEDULING_H

#include <string>
#include <vector>
#include <optional>
#include "Customer.h"
#include "Service.h"
#include "Employee.h"

struct Appointment {
    int id;
    int customerId;
    int serviceId;
    int employeeId;
    std::string datetime;
    std::string status;
};

class AppointmentManager {
public:
    static Appointment schedule(int customerId, int serviceId, int employeeId,
                                const std::string& datetime);
    static std::vector<Appointment> getAll();
    static std::vector<Appointment> byEmployee(int employeeId);
    static std::vector<Appointment> byCustomer(int customerId);
    static std::vector<Appointment> upcoming();
    static bool setStatus(int id, const std::string& status);
    static bool cancel(int id);
    static void list(const std::vector<Customer>& customers,
                     const std::vector<Service>& services,
                     const std::vector<Employee>& employees);

private:
    static std::vector<Appointment> load();
    static void save(const std::vector<Appointment>& appointments);
    static int nextId();
};

#endif
