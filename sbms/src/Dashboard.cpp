#include "Dashboard.h"
#include "Customer.h"
#include <iostream>
#include <algorithm>
#include <numeric>
#include <iomanip>
#include <fstream>
#include <sstream>

const std::string REVENUE_FILE = "data/revenue_history.txt";

DashboardStats Dashboard::compute(const std::vector<Service>& services) {
    auto invoices = Invoice::loadAll();
    auto customers = Customer::loadAll();
    DashboardStats stats = {0, 0, 0.0, 0.0, 0, "", 0, 0.0};

    stats.totalCustomers = customers.size();
    stats.totalInvoices = invoices.size();

    if (invoices.empty()) return stats;

    std::vector<int> serviceCounts(services.size() + 1, 0);
    double total = 0.0;
    double totalDisc = 0.0;

    for (const auto& inv : invoices) {
        double invTotal = 0.0;
        for (size_t i = 0; i < inv.getServiceIds().size(); ++i) {
            int sid = inv.getServiceIds()[i];
            int qty = inv.getQuantities()[i];
            for (const auto& s : services) {
                if (s.getId() == sid) {
                    invTotal += s.getPrice() * qty;
                    if (sid < static_cast<int>(serviceCounts.size())) serviceCounts[sid] += qty;
                    break;
                }
            }
        }
        double tax = invTotal * inv.getTaxRate();
        total += invTotal + tax;

        if (inv.getDiscountPercent() > 0.0) {
            double discAmt = (invTotal + tax) * inv.getDiscountPercent() / 100.0;
            totalDisc += discAmt;
        }
    }

    stats.totalRevenue = total - totalDisc;
    stats.totalDiscounts = totalDisc;
    stats.avgInvoiceValue = stats.totalRevenue / invoices.size();

    auto maxIt = std::max_element(serviceCounts.begin(), serviceCounts.end());
    if (maxIt != serviceCounts.end() && *maxIt > 0) {
        stats.popularServiceId = std::distance(serviceCounts.begin(), maxIt);
        stats.popularServiceCount = *maxIt;
        for (const auto& s : services) {
            if (s.getId() == stats.popularServiceId) {
                stats.popularServiceName = s.getName();
                break;
            }
        }
    }

    return stats;
}

void Dashboard::printSummary(const std::vector<Service>& services) {
    auto stats = compute(services);
    std::cout << "\n========== DASHBOARD ==========\n";
    std::cout << "Customers: " << stats.totalCustomers << "\n";
    std::cout << "Invoices:  " << stats.totalInvoices << "\n";
    std::cout << "Revenue:   $" << std::fixed << std::setprecision(2) << stats.totalRevenue << "\n";
    std::cout << "Discounts: $" << stats.totalDiscounts << "\n";
    std::cout << "Avg Invoice: $" << stats.avgInvoiceValue << "\n";
    if (stats.popularServiceCount > 0)
        std::cout << "Top Service: " << stats.popularServiceName
                  << " (sold " << stats.popularServiceCount << ")\n";
    std::cout << "==============================\n";
}

void Dashboard::printRevenueTrend() {
    auto history = loadRevenueHistory();
    std::cout << "\n=== REVENUE TREND (last 7) ===\n";
    if (history.empty()) { std::cout << "No data.\n"; return; }
    for (size_t i = 0; i < history.size(); ++i) {
        int barLen = static_cast<int>(history[i] / 10);
        std::cout << "  $" << std::setw(8) << std::fixed << std::setprecision(2) << history[i] << " ";
        for (int b = 0; b < barLen && b < 50; ++b) std::cout << "#";
        std::cout << "\n";
    }
    std::cout << "==============================\n";
}

void Dashboard::printServiceBreakdown(const std::vector<Service>& services) {
    auto invoices = Invoice::loadAll();
    std::cout << "\n=== SERVICE BREAKDOWN ===\n";
    if (invoices.empty()) { std::cout << "No invoices.\n"; return; }

    std::vector<int> counts(services.size() + 1, 0);
    std::vector<double> revenues(services.size() + 1, 0.0);

    for (const auto& inv : invoices) {
        for (size_t i = 0; i < inv.getServiceIds().size(); ++i) {
            int sid = inv.getServiceIds()[i];
            int qty = inv.getQuantities()[i];
            for (const auto& s : services) {
                if (s.getId() == sid) {
                    counts[sid] += qty;
                    revenues[sid] += s.getPrice() * qty;
                    break;
                }
            }
        }
    }

    for (const auto& s : services) {
        if (counts[s.getId()] > 0)
            std::cout << "  " << s.getName() << ": " << counts[s.getId()] << " sold, $"
                      << std::fixed << std::setprecision(2) << revenues[s.getId()] << "\n";
    }
    std::cout << "========================\n";
}

std::vector<double> Dashboard::loadRevenueHistory() {
    std::vector<double> history;
    std::ifstream file(REVENUE_FILE);
    if (!file.is_open()) {
        auto invoices = Invoice::loadAll();
        double total = 0.0;
        for (const auto& inv : invoices) total += inv.getDiscountedTotal() > 0.0 ? inv.getDiscountedTotal() : 0.0;
        if (total > 0.0) history.push_back(total);
        return history;
    }
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        history.push_back(std::stod(line));
    }
    return history;
}
