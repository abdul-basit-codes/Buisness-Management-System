#include "Report.h"
#include <iostream>
#include <algorithm>
#include <map>

SalesSummary Report::generateSalesSummary(const std::vector<Invoice>& invoices,
                                          const std::vector<Service>& services) {
    SalesSummary summary = {0.0, 0.0, 0, 0};
    for (const auto& inv : invoices) {
        summary.totalRevenue += inv.getTotal(services);
        summary.totalTax += inv.getTaxAmount(services);
        summary.invoiceCount++;
        for (int qty : inv.getQuantities()) summary.serviceCount += qty;
    }
    return summary;
}

std::vector<std::pair<int, int>> Report::popularServices(const std::vector<Invoice>& invoices) {
    std::map<int, int> freq;
    for (const auto& inv : invoices) {
        auto sids = inv.getServiceIds();
        for (int sid : sids) freq[sid]++;
    }
    std::vector<std::pair<int, int>> result(freq.begin(), freq.end());
    std::sort(result.begin(), result.end(),
              [](const auto& a, const auto& b) { return a.second > b.second; });
    return result;
}

void Report::printSalesSummary(const SalesSummary& summary) {
    std::cout << "\n=== SALES SUMMARY ===\n";
    std::cout << "Invoices: " << summary.invoiceCount << "\n";
    std::cout << "Services Sold: " << summary.serviceCount << "\n";
    std::cout << "Total Revenue: $" << summary.totalRevenue << "\n";
    std::cout << "Total Tax: $" << summary.totalTax << "\n";
    std::cout << "Net Revenue: $" << (summary.totalRevenue - summary.totalTax) << "\n";
    std::cout << "=====================\n";
}

void Report::printPopularServices(const std::vector<std::pair<int, int>>& popular,
                                   const std::vector<Service>& services) {
    std::cout << "\n=== POPULAR SERVICES ===\n";
    int rank = 1;
    for (const auto& [sid, count] : popular) {
        for (const auto& s : services) {
            if (s.getId() == sid) {
                std::cout << rank << ". " << s.getName() << " (" << count << " times)\n";
                break;
            }
        }
        rank++;
    }
    std::cout << "========================\n";
}

void Report::printDailyReport(const std::vector<Invoice>& invoices,
                               const std::vector<Service>& services) {
    auto summary = generateSalesSummary(invoices, services);
    printSalesSummary(summary);
    auto pop = popularServices(invoices);
    if (!pop.empty()) printPopularServices(pop, services);
}
