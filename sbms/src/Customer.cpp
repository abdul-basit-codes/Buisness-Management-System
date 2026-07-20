#include "Customer.h"
#include <sstream>
#include <iostream>

Customer::Customer() : id(0), name(""), phone(""), address("") {}

Customer::Customer(int id, const std::string& name, const std::string& phone, const std::string& address)
    : id(id), name(name), phone(phone), address(address) {}

int Customer::getId() const { return id; }
std::string Customer::getName() const { return name; }
std::string Customer::getPhone() const { return phone; }
std::string Customer::getAddress() const { return address; }

void Customer::setName(const std::string& n) { name = n; }
void Customer::setPhone(const std::string& p) { phone = p; }
void Customer::setAddress(const std::string& a) { address = a; }

std::string Customer::toCSV() const {
    std::ostringstream oss;
    oss << id << "," << name << "," << phone << "," << address;
    return oss.str();
}

Customer Customer::fromCSV(const std::string& line) {
    std::stringstream ss(line);
    std::string idStr, name, phone, address;
    std::getline(ss, idStr, ',');
    std::getline(ss, name, ',');
    std::getline(ss, phone, ',');
    std::getline(ss, address, ',');
    return Customer(std::stoi(idStr), name, phone, address);
}

void Customer::print() const {
    std::cout << "ID: " << id << " | Name: " << name
              << " | Phone: " << phone << " | Address: " << address << "\n";
}
