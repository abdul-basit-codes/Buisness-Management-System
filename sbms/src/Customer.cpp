#include "Customer.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>

const std::string CUST_FILE = "data/customers.txt";

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

std::vector<Customer> Customer::loadAll() {
    std::vector<Customer> customers;
    std::ifstream file(CUST_FILE);
    if (!file.is_open()) return customers;
    std::string line;
    while (std::getline(file, line)) { if (!line.empty()) customers.push_back(fromCSV(line)); }
    return customers;
}

void Customer::saveAll(const std::vector<Customer>& customers) {
    std::ofstream file(CUST_FILE, std::ios::trunc);
    for (const auto& c : customers) file << c.toCSV() << "\n";
}

void Customer::print() const {
    std::cout << "ID: " << id << " | Name: " << name
              << " | Phone: " << phone << " | Address: " << address << "\n";
}
