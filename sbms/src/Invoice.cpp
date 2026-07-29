#include "Invoice.h"
#include <sstream>
#include <iostream>
#include <ctime>
#include <iomanip>

Invoice::Invoice() : id(0), customerId(0), taxRate(0.0) {
    std::time_t t = std::time(nullptr);
    std::tm* tm = std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(tm, "%Y-%m-%d");
    date = oss.str();
}

Invoice::Invoice(int id, int customerId, const std::vector<int>& serviceIds,
                 const std::vector<int>& quantities, double taxRate)
    : id(id), customerId(customerId), serviceIds(serviceIds),
      quantities(quantities), taxRate(taxRate) {
    std::time_t t = std::time(nullptr);
    std::tm* tm = std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(tm, "%Y-%m-%d");
    date = oss.str();
}

int Invoice::getId() const { return id; }
int Invoice::getCustomerId() const { return customerId; }
std::vector<int> Invoice::getServiceIds() const { return serviceIds; }
std::vector<int> Invoice::getQuantities() const { return quantities; }
double Invoice::getTaxRate() const { return taxRate; }
std::string Invoice::getDate() const { return date; }

double Invoice::getSubtotal(const std::vector<Service>& services) const {
    double total = 0.0;
    for (size_t i = 0; i < serviceIds.size(); ++i) {
        for (const auto& s : services) {
            if (s.getId() == serviceIds[i]) {
                total += s.getPrice() * quantities[i];
                break;
            }
        }
    }
    return total;
}

double Invoice::getTaxAmount(const std::vector<Service>& services) const {
    return getSubtotal(services) * taxRate;
}

double Invoice::getTotal(const std::vector<Service>& services) const {
    return getSubtotal(services) + getTaxAmount(services);
}

void Invoice::addService(int serviceId, int quantity) {
    serviceIds.push_back(serviceId);
    quantities.push_back(quantity);
}

void Invoice::setTaxRate(double rate) { taxRate = rate; }

std::string Invoice::toCSV() const {
    std::ostringstream oss;
    oss << id << "," << customerId << "," << taxRate << "," << date << ",";
    for (size_t i = 0; i < serviceIds.size(); ++i) {
        if (i > 0) oss << ";";
        oss << serviceIds[i] << ":" << quantities[i];
    }
    return oss.str();
}

Invoice Invoice::fromCSV(const std::string& line) {
    std::stringstream ss(line);
    std::string idStr, cidStr, taxStr, dateStr, itemsStr;
    std::getline(ss, idStr, ',');
    std::getline(ss, cidStr, ',');
    std::getline(ss, taxStr, ',');
    std::getline(ss, dateStr, ',');
    std::getline(ss, itemsStr, ',');

    Invoice inv(std::stoi(idStr), std::stoi(cidStr), {}, {}, std::stod(taxStr));
    inv.date = dateStr;

    std::stringstream itemsSs(itemsStr);
    std::string pair;
    while (std::getline(itemsSs, pair, ';')) {
        std::stringstream ps(pair);
        std::string sid, qty;
        std::getline(ps, sid, ':');
        std::getline(ps, qty, ':');
        if (!sid.empty()) inv.addService(std::stoi(sid), std::stoi(qty));
    }
    return inv;
}

void Invoice::print(const std::vector<Service>& services) const {
    std::cout << "\n=== INVOICE #" << id << " ===\n";
    std::cout << "Date: " << date << "\n";
    std::cout << "Customer ID: " << customerId << "\n";
    std::cout << "--- Items ---\n";
    for (size_t i = 0; i < serviceIds.size(); ++i) {
        for (const auto& s : services) {
            if (s.getId() == serviceIds[i]) {
                double lineTotal = s.getPrice() * quantities[i];
                std::cout << "  " << s.getName() << " x" << quantities[i]
                          << " @ $" << s.getPrice() << " = $" << lineTotal << "\n";
                break;
            }
        }
    }
    std::cout << "Subtotal: $" << getSubtotal(services) << "\n";
    std::cout << "Tax (" << (taxRate * 100) << "%): $" << getTaxAmount(services) << "\n";
    std::cout << "Total: $" << getTotal(services) << "\n";
    std::cout << "==================\n";
}
