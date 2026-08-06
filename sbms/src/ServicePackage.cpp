#include "ServicePackage.h"
#include "AuditLog.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <iomanip>

const std::string PKG_FILE = "data/packages.txt";

ServicePackage PackageManager::create(const std::string& name,
                                      const std::vector<int>& serviceIds,
                                      double discountPercent) {
    auto packages = load();
    ServicePackage pkg{nextId(), name, serviceIds, discountPercent, true};
    packages.push_back(pkg);
    save(packages);
    AuditLog::log("CREATE", "Package", pkg.id, name + " with " + std::to_string(discountPercent) + "% bundle discount");
    return pkg;
}

std::vector<ServicePackage> PackageManager::load() {
    std::vector<ServicePackage> packages;
    std::ifstream file(PKG_FILE);
    if (!file.is_open()) return packages;
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string idStr, name, idsStr, discStr, activeStr;
        std::getline(ss, idStr, '|');
        std::getline(ss, name, '|');
        std::getline(ss, idsStr, '|');
        std::getline(ss, discStr, '|');
        std::getline(ss, activeStr, '|');

        std::vector<int> ids;
        std::stringstream is(idsStr);
        std::string token;
        while (std::getline(is, token, ';'))
            if (!token.empty()) ids.push_back(std::stoi(token));

        packages.push_back({std::stoi(idStr), name, ids, std::stod(discStr), activeStr == "1"});
    }
    return packages;
}

void PackageManager::save(const std::vector<ServicePackage>& packages) {
    std::ofstream file(PKG_FILE, std::ios::trunc);
    for (const auto& p : packages) {
        file << p.id << "|" << p.name << "|";
        for (size_t i = 0; i < p.serviceIds.size(); ++i) {
            if (i > 0) file << ";";
            file << p.serviceIds[i];
        }
        file << "|" << p.discountPercent << "|" << (p.active ? "1" : "0") << "\n";
    }
}

int PackageManager::nextId() {
    auto packages = load();
    int maxId = 0;
    for (const auto& p : packages) maxId = std::max(maxId, p.id);
    return maxId + 1;
}

std::vector<ServicePackage> PackageManager::getAll() { return load(); }

double PackageManager::packagePrice(const ServicePackage& pkg,
                                    const std::vector<Service>& services) {
    double full = 0.0;
    for (int sid : pkg.serviceIds) {
        for (const auto& s : services)
            if (s.getId() == sid) { full += s.getPrice(); break; }
    }
    return full * (1.0 - pkg.discountPercent / 100.0);
}

double PackageManager::savingsAmount(const ServicePackage& pkg,
                                     const std::vector<Service>& services) {
    double full = 0.0;
    for (int sid : pkg.serviceIds) {
        for (const auto& s : services)
            if (s.getId() == sid) { full += s.getPrice(); break; }
    }
    return full * pkg.discountPercent / 100.0;
}

void PackageManager::list(const std::vector<Service>& services) {
    auto packages = load();
    std::cout << "\n=== SERVICE PACKAGES ===\n";
    if (packages.empty()) { std::cout << "No packages.\n"; return; }
    for (const auto& p : packages) {
        if (!p.active) continue;
        std::cout << "  [" << p.id << "] " << p.name << " — "
                  << p.discountPercent << "% off ($"
                  << std::fixed << std::setprecision(2) << packagePrice(p, services)
                  << ")\n";
        std::cout << "    Includes: ";
        bool first = true;
        for (int sid : p.serviceIds) {
            for (const auto& s : services) {
                if (s.getId() == sid) {
                    if (!first) std::cout << ", ";
                    std::cout << s.getName();
                    first = false;
                    break;
                }
            }
        }
        std::cout << "\n";
    }
    std::cout << "=======================\n";
}

bool PackageManager::deactivate(int id) {
    auto packages = load();
    for (auto& p : packages) {
        if (p.id == id) { p.active = false; save(packages); return true; }
    }
    return false;
}
