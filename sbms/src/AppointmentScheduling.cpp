#include "AppointmentScheduling.h"
#include "AuditLog.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

const std::string APPOINTMENTS_FILE = "data/appointments.txt";

Appointment AppointmentManager::schedule(int customerId, int serviceId, int employeeId,
                                         const std::string& datetime) {
    auto appointments = load();
    Appointment a{nextId(), customerId, serviceId, employeeId, datetime, "Scheduled"};
    appointments.push_back(a);
    save(appointments);
    AuditLog::log("CREATE", "Appointment", a.id,
                  "Booked customer " + std::to_string(customerId) + " with employee " +
                  std::to_string(employeeId) + " for service " + std::to_string(serviceId) +
                  " at " + datetime);
    std::cout << "Appointment scheduled with ID " << a.id << "\n";
    return a;
}

std::vector<Appointment> AppointmentManager::load() {
    std::vector<Appointment> appointments;
    std::ifstream file(APPOINTMENTS_FILE);
    if (!file.is_open()) return appointments;
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string idStr, custStr, servStr, empStr, datetime, status;
        std::getline(ss, idStr, '|');
        std::getline(ss, custStr, '|');
        std::getline(ss, servStr, '|');
        std::getline(ss, empStr, '|');
        std::getline(ss, datetime, '|');
        std::getline(ss, status, '|');
        appointments.push_back({std::stoi(idStr), std::stoi(custStr), std::stoi(servStr),
                                std::stoi(empStr), datetime, status});
    }
    return appointments;
}

void AppointmentManager::save(const std::vector<Appointment>& appointments) {
    std::ofstream file(APPOINTMENTS_FILE, std::ios::trunc);
    for (const auto& a : appointments)
        file << a.id << "|" << a.customerId << "|" << a.serviceId << "|"
             << a.employeeId << "|" << a.datetime << "|" << a.status << "\n";
}

int AppointmentManager::nextId() {
    auto appointments = load();
    int maxId = 0;
    for (const auto& a : appointments) maxId = std::max(maxId, a.id);
    return maxId + 1;
}

std::vector<Appointment> AppointmentManager::getAll() { return load(); }

std::vector<Appointment> AppointmentManager::byEmployee(int employeeId) {
    auto appointments = load();
    std::vector<Appointment> result;
    for (const auto& a : appointments)
        if (a.employeeId == employeeId) result.push_back(a);
    return result;
}

std::vector<Appointment> AppointmentManager::byCustomer(int customerId) {
    auto appointments = load();
    std::vector<Appointment> result;
    for (const auto& a : appointments)
        if (a.customerId == customerId) result.push_back(a);
    return result;
}

std::vector<Appointment> AppointmentManager::upcoming() {
    auto appointments = load();
    std::vector<Appointment> result;
    for (const auto& a : appointments)
        if (a.status == "Scheduled" || a.status == "Confirmed")
            result.push_back(a);
    std::sort(result.begin(), result.end(),
        [](const Appointment& x, const Appointment& y) { return x.datetime < y.datetime; });
    return result;
}

bool AppointmentManager::setStatus(int id, const std::string& status) {
    auto appointments = load();
    for (auto& a : appointments) {
        if (a.id == id) {
            a.status = status;
            save(appointments);
            AuditLog::log("UPDATE", "Appointment", id, "Status changed to " + status);
            std::cout << "Appointment " << id << " set to " << status << "\n";
            return true;
        }
    }
    return false;
}

bool AppointmentManager::cancel(int id) {
    auto appointments = load();
    auto it = std::remove_if(appointments.begin(), appointments.end(),
        [id](const Appointment& a) { return a.id == id; });
    if (it != appointments.end()) {
        appointments.erase(it, appointments.end());
        save(appointments);
        AuditLog::log("CANCEL", "Appointment", id, "Appointment cancelled");
        return true;
    }
    return false;
}

void AppointmentManager::list(const std::vector<Customer>& customers,
                              const std::vector<Service>& services,
                              const std::vector<Employee>& employees) {
    auto appointments = load();
    std::cout << "\n=== APPOINTMENTS ===\n";
    if (appointments.empty()) { std::cout << "No appointments.\n"; return; }
    for (const auto& a : appointments) {
        std::string cust = "C#" + std::to_string(a.customerId);
        std::string serv = "S#" + std::to_string(a.serviceId);
        std::string emp = "E#" + std::to_string(a.employeeId);
        for (const auto& c : customers) if (c.getId() == a.customerId) { cust = c.getName(); break; }
        for (const auto& s : services) if (s.getId() == a.serviceId) { serv = s.getName(); break; }
        for (const auto& e : employees) if (e.getId() == a.employeeId) { emp = e.getName(); break; }
        std::cout << "  [" << a.id << "] " << a.datetime << " — " << cust
                  << " | " << serv << " | " << emp
                  << " [" << a.status << "]\n";
    }
    std::cout << "====================\n";
}
