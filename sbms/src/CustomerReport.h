#ifndef CUSTOMERREPORT_H
#define CUSTOMERREPORT_H

#include <vector>
#include <utility>
#include <string>
#include "Invoice.h"
#include "Service.h"

class CustomerReport {
public:
    static std::vector<std::pair<int, double>> topCustomersBySpend(
        const std::vector<Invoice>& invoices, const std::vector<Service>& services, int limit = 5);
    static int repeatCustomerCount(const std::vector<Invoice>& invoices);
    static double averageSpendPerCustomer(const std::vector<Invoice>& invoices,
                                          const std::vector<Service>& services);
    static void printTopCustomers(const std::vector<Invoice>& invoices,
                                  const std::vector<Service>& services, int limit = 5);
    static void printLoyaltySummary(const std::vector<Invoice>& invoices,
                                    const std::vector<Service>& services);
};

#endif
