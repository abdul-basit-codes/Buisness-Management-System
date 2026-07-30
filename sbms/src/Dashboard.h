#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <string>
#include <vector>
#include "Invoice.h"
#include "Service.h"

struct DashboardStats {
    int totalCustomers;
    int totalInvoices;
    double totalRevenue;
    double totalDiscounts;
    int popularServiceId;
    std::string popularServiceName;
    int popularServiceCount;
    double avgInvoiceValue;
};

class Dashboard {
public:
    static DashboardStats compute(const std::vector<Service>& services);
    static void printSummary(const std::vector<Service>& services);
    static void printRevenueTrend();
    static void printServiceBreakdown(const std::vector<Service>& services);

private:
    static std::vector<double> loadRevenueHistory();
};

#endif
