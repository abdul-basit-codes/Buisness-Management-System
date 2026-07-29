#ifndef REPORT_H
#define REPORT_H

#include <string>
#include <vector>
#include "Invoice.h"
#include "Service.h"

struct SalesSummary {
    double totalRevenue;
    double totalTax;
    int invoiceCount;
    int serviceCount;
};

class Report {
public:
    static SalesSummary generateSalesSummary(const std::vector<Invoice>& invoices,
                                             const std::vector<Service>& services);
    static std::vector<std::pair<int, int>> popularServices(const std::vector<Invoice>& invoices);
    static void printSalesSummary(const SalesSummary& summary);
    static void printPopularServices(const std::vector<std::pair<int, int>>& popular,
                                     const std::vector<Service>& services);
    static void printDailyReport(const std::vector<Invoice>& invoices,
                                 const std::vector<Service>& services);
};

#endif
