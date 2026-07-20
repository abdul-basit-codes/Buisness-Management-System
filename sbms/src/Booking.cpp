#include "Booking.h"
#include <sstream>
#include <iostream>

Booking::Booking()
    : id(0), customerId(0), serviceId(0), employeeId(0), date(""), completed(false) {}

Booking::Booking(int id, int customerId, int serviceId, int employeeId,
                   const std::string& date, bool completed)
    : id(id), customerId(customerId), serviceId(serviceId),
      employeeId(employeeId), date(date), completed(completed) {}

int Booking::getId() const { return id; }
int Booking::getCustomerId() const { return customerId; }
int Booking::getServiceId() const { return serviceId; }
int Booking::getEmployeeId() const { return employeeId; }
std::string Booking::getDate() const { return date; }
bool Booking::isCompleted() const { return completed; }

void Booking::setCompleted(bool c) { completed = c; }

std::string Booking::toCSV() const {
    std::ostringstream oss;
    oss << id << "," << customerId << "," << serviceId << "," << employeeId
        << "," << date << "," << (completed ? 1 : 0);
    return oss.str();
}

Booking Booking::fromCSV(const std::string& line) {
    std::stringstream ss(line);
    std::string idStr, custStr, servStr, empStr, date, doneStr;
    std::getline(ss, idStr, ',');
    std::getline(ss, custStr, ',');
    std::getline(ss, servStr, ',');
    std::getline(ss, empStr, ',');
    std::getline(ss, date, ',');
    std::getline(ss, doneStr, ',');
    return Booking(std::stoi(idStr), std::stoi(custStr), std::stoi(servStr),
                    std::stoi(empStr), date, doneStr == "1");
}

void Booking::print() const {
    std::cout << "Booking#" << id << " | Customer:" << customerId
              << " | Service:" << serviceId << " | Employee:" << employeeId
              << " | Date:" << date << " | " << (completed ? "Completed" : "Pending") << "\n";
}
