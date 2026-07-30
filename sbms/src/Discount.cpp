#include "Discount.h"
#include "AuditLog.h"
#include "Invoice.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <iomanip>

const std::string DISC_FILE = "data/discounts.txt";

Discount DiscountManager::create(const std::string& code, double percent, const std::string& description) {
    auto discounts = load();
    Discount d{nextId(), code, percent, description, true};
    discounts.push_back(d);
    save(discounts);
    AuditLog::log("CREATE", "Discount", d.id, "Code: " + code + ", " + std::to_string(percent) + "% off");
    return d;
}

std::vector<Discount> DiscountManager::load() {
    std::vector<Discount> discounts;
    std::ifstream file(DISC_FILE);
    if (!file.is_open()) return discounts;
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string idStr, code, percentStr, desc, activeStr;
        std::getline(ss, idStr, '|');
        std::getline(ss, code, '|');
        std::getline(ss, percentStr, '|');
        std::getline(ss, desc, '|');
        std::getline(ss, activeStr, '|');
        discounts.push_back({std::stoi(idStr), code, std::stod(percentStr), desc, activeStr == "1"});
    }
    return discounts;
}

void DiscountManager::save(const std::vector<Discount>& discounts) {
    std::ofstream file(DISC_FILE, std::ios::trunc);
    for (const auto& d : discounts)
        file << d.id << "|" << d.code << "|" << d.percent << "|" << d.description << "|" << (d.active ? "1" : "0") << "\n";
}

int DiscountManager::nextId() {
    auto discounts = load();
    int maxId = 0;
    for (const auto& d : discounts) maxId = std::max(maxId, d.id);
    return maxId + 1;
}

std::vector<Discount> DiscountManager::getAll() { return load(); }

Discount* DiscountManager::findByCode(const std::string& code) {
    auto discounts = load();
    for (auto& d : discounts)
        if (d.code == code && d.active) return new Discount(d);
    return nullptr;
}

bool DiscountManager::applyToInvoice(int invoiceId, const std::string& code) {
    auto d = findByCode(code);
    if (!d) return false;
    auto invoices = Invoice::loadAll();
    for (auto& inv : invoices) {
        if (inv.id == invoiceId) {
            inv.discountPercent = d->percent;
            inv.discountedTotal = inv.total - (inv.total * d->percent / 100.0);
            Invoice::saveAll(invoices);
            AuditLog::log("APPLY_DISCOUNT", "Invoice", invoiceId, "Code " + code + " (" + std::to_string(d->percent) + "%)");
            delete d;
            return true;
        }
    }
    delete d;
    return false;
}

void DiscountManager::list() {
    auto discounts = load();
    std::cout << "\n=== DISCOUNTS ===\n";
    if (discounts.empty()) { std::cout << "No discounts configured.\n"; return; }
    for (const auto& d : discounts) {
        std::cout << "  [" << d.id << "] " << d.code << " — " << d.percent << "%"
                  << (d.active ? "" : " (INACTIVE)") << "\n    " << d.description << "\n";
    }
    std::cout << "=================\n";
}

bool DiscountManager::deactivate(int id) {
    auto discounts = load();
    for (auto& d : discounts) {
        if (d.id == id) { d.active = false; save(discounts); return true; }
    }
    return false;
}
