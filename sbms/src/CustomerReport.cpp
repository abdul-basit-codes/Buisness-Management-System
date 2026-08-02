#include "CustomerReport.h"
#include "Customer.h"
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <map>

std::vector<std::pair<int, double>> CustomerReport::topCustomersBySpend(
    const std::vector<Invoice>& invoices, const std::vector<Service>& services, int limit) {

    std::map<int, double> spend;
    for (const auto& inv : invoices) {
        double total = 0.0;
        for (size_t i = 0; i < inv.getServiceIds().size(); ++i) {
            int sid = inv.getServiceIds()[i];
            int qty = inv.getQuantities()[i];
            for (const auto& s : services) {
                if (s.getId() == sid) { total += s.getPrice() * qty; break; }
            }
        }
        total += total * inv.getTaxRate();
        if (inv.getDiscountPercent() > 0.0)
            total -= total * inv.getDiscountPercent() / 100.0;
        spend[inv.getCustomerId()] += total;
    }

    std::vector<std::pair<int, double>> ranked(spend.begin(), spend.end());
    std::sort(ranked.begin(), ranked.end(),
              [](const auto& a, const auto& b) { return a.second > b.second; });
    if (limit > 0 && static_cast<int>(ranked.size()) > limit)
        ranked.resize(limit);
    return ranked;
}

int CustomerReport::repeatCustomerCount(const std::vector<Invoice>& invoices) {
    std::map<int, int> counts;
    for (const auto& inv : invoices) counts[inv.getCustomerId()]++;
    int repeats = 0;
    for (const auto& kv : counts)
        if (kv.second > 1) repeats++;
    return repeats;
}

double CustomerReport::averageSpendPerCustomer(const std::vector<Invoice>& invoices,
                                               const std::vector<Service>& services) {
    auto customers = Customer::loadAll();
    if (customers.empty()) return 0.0;
    auto ranked = topCustomersBySpend(invoices, services, 0);
    double total = 0.0;
    for (const auto& kv : ranked) total += kv.second;
    return total / customers.size();
}

void CustomerReport::printTopCustomers(const std::vector<Invoice>& invoices,
                                       const std::vector<Service>& services, int limit) {
    auto ranked = topCustomersBySpend(invoices, services, limit);
    auto customers = Customer::loadAll();

    std::cout << "\n=== TOP CUSTOMERS BY SPEND ===\n";
    if (ranked.empty()) { std::cout << "No invoices yet.\n"; return; }
    for (const auto& kv : ranked) {
        std::string name = "ID " + std::to_string(kv.first);
        for (const auto& c : customers) {
            if (c.getId() == kv.first) { name = c.getName(); break; }
        }
        std::cout << "  " << name << " — $" << std::fixed << std::setprecision(2) << kv.second << "\n";
    }
    std::cout << "==============================\n";
}

void CustomerReport::printLoyaltySummary(const std::vector<Invoice>& invoices,
                                         const std::vector<Service>& services) {
    auto customers = Customer::loadAll();
    int repeat = repeatCustomerCount(invoices);

    std::cout << "\n=== CUSTOMER LOYALTY SUMMARY ===\n";
    std::cout << "Total customers:     " << customers.size() << "\n";
    std::cout << "Repeat customers:    " << repeat << "\n";
    std::cout << "Avg spend/customer:  $" << std::fixed << std::setprecision(2)
              << averageSpendPerCustomer(invoices, services) << "\n";
    std::cout << "===============================\n";
}
