#ifndef INVOICE_H
#define INVOICE_H

#include <string>
#include <vector>
#include "Customer.h"
#include "Service.h"

class Invoice {
public:
    Invoice();
    Invoice(int id, int customerId, const std::vector<int>& serviceIds,
            const std::vector<int>& quantities, double taxRate);

    int getId() const;
    int getCustomerId() const;
    std::vector<int> getServiceIds() const;
    std::vector<int> getQuantities() const;
    double getTaxRate() const;
    double getSubtotal(const std::vector<Service>& services) const;
    double getTaxAmount(const std::vector<Service>& services) const;
    double getTotal(const std::vector<Service>& services) const;
    std::string getDate() const;

    void addService(int serviceId, int quantity);
    void setTaxRate(double rate);

    std::string toCSV() const;
    static Invoice fromCSV(const std::string& line);
    void print(const std::vector<Service>& services) const;

private:
    int id;
    int customerId;
    std::vector<int> serviceIds;
    std::vector<int> quantities;
    double taxRate;
    std::string date;
};

#endif
