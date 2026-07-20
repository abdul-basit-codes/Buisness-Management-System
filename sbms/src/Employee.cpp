#include "Employee.h"
#include <sstream>
#include <iostream>

Employee::Employee() : id(0), name(""), role(""), salary(0.0) {}

Employee::Employee(int id, const std::string& name, const std::string& role, double salary)
    : id(id), name(name), role(role), salary(salary) {}

int Employee::getId() const { return id; }
std::string Employee::getName() const { return name; }
std::string Employee::getRole() const { return role; }
double Employee::getSalary() const { return salary; }

void Employee::setName(const std::string& n) { name = n; }
void Employee::setRole(const std::string& r) { role = r; }
void Employee::setSalary(double s) { salary = s; }

std::string Employee::toCSV() const {
    std::ostringstream oss;
    oss << id << "," << name << "," << role << "," << salary;
    return oss.str();
}

Employee Employee::fromCSV(const std::string& line) {
    std::stringstream ss(line);
    std::string idStr, name, role, salaryStr;
    std::getline(ss, idStr, ',');
    std::getline(ss, name, ',');
    std::getline(ss, role, ',');
    std::getline(ss, salaryStr, ',');
    return Employee(std::stoi(idStr), name, role, std::stod(salaryStr));
}

void Employee::print() const {
    std::cout << "ID: " << id << " | Name: " << name
              << " | Role: " << role << " | Salary: " << salary << "\n";
}
