#include "Service.h"
#include <sstream>
#include <iostream>

Service::Service() : id(0), name(""), price(0.0), durationMinutes(0) {}

Service::Service(int id, const std::string& name, double price, int durationMinutes)
    : id(id), name(name), price(price), durationMinutes(durationMinutes) {}

int Service::getId() const { return id; }
std::string Service::getName() const { return name; }
double Service::getPrice() const { return price; }
int Service::getDuration() const { return durationMinutes; }

void Service::setName(const std::string& n) { name = n; }
void Service::setPrice(double p) { price = p; }
void Service::setDuration(int d) { durationMinutes = d; }

std::string Service::toCSV() const {
    std::ostringstream oss;
    oss << id << "," << name << "," << price << "," << durationMinutes;
    return oss.str();
}

Service Service::fromCSV(const std::string& line) {
    std::stringstream ss(line);
    std::string idStr, name, priceStr, durStr;
    std::getline(ss, idStr, ',');
    std::getline(ss, name, ',');
    std::getline(ss, priceStr, ',');
    std::getline(ss, durStr, ',');
    return Service(std::stoi(idStr), name, std::stod(priceStr), std::stoi(durStr));
}

void Service::print() const {
    std::cout << "ID: " << id << " | Name: " << name
              << " | Price: " << price << " | Duration: " << durationMinutes << " min\n";
}
