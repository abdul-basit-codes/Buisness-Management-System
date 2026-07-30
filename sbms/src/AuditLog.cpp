#include "AuditLog.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <ctime>
#include <iomanip>
#include <algorithm>

const std::string AUDIT_FILE = "data/audit.txt";

std::string AuditLog::now() {
    std::time_t t = std::time(nullptr);
    std::tm* tm = std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

int AuditLog::nextId() {
    auto entries = load();
    int maxId = 0;
    for (const auto& e : entries) maxId = std::max(maxId, e.id);
    return maxId + 1;
}

AuditEntry AuditLog::log(const std::string& action, const std::string& entityType,
                          int entityId, const std::string& details) {
    auto entries = load();
    AuditEntry entry{nextId(), action, entityType, entityId, details, now()};
    entries.push_back(entry);
    save(entries);
    return entry;
}

std::vector<AuditEntry> AuditLog::load() {
    std::vector<AuditEntry> entries;
    std::ifstream file(AUDIT_FILE);
    if (!file.is_open()) return entries;
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string idStr, action, entityType, eidStr, details, ts;
        std::getline(ss, idStr, '|');
        std::getline(ss, action, '|');
        std::getline(ss, entityType, '|');
        std::getline(ss, eidStr, '|');
        std::getline(ss, details, '|');
        std::getline(ss, ts, '|');
        entries.push_back({std::stoi(idStr), action, entityType, std::stoi(eidStr), details, ts});
    }
    return entries;
}

void AuditLog::save(const std::vector<AuditEntry>& entries) {
    std::ofstream file(AUDIT_FILE, std::ios::trunc);
    for (const auto& e : entries)
        file << e.id << "|" << e.action << "|" << e.entityType << "|" << e.entityId << "|" << e.details << "|" << e.timestamp << "\n";
}

std::vector<AuditEntry> AuditLog::getRecent(int count) {
    auto entries = load();
    if (entries.size() > static_cast<size_t>(count))
        entries.erase(entries.begin(), entries.end() - count);
    return entries;
}

void AuditLog::printRecent(int count) {
    auto entries = getRecent(count);
    std::cout << "\n=== RECENT AUDIT LOG ===\n";
    if (entries.empty()) { std::cout << "No entries.\n"; return; }
    for (const auto& e : entries)
        std::cout << "[" << e.timestamp << "] " << e.action << " " << e.entityType
                  << " #" << e.entityId << " — " << e.details << "\n";
    std::cout << "========================\n";
}

void AuditLog::printByEntity(const std::string& entityType, int entityId) {
    auto entries = load();
    std::cout << "\n=== AUDIT: " << entityType << " #" << entityId << " ===\n";
    bool found = false;
    for (const auto& e : entries) {
        if (e.entityType == entityType && e.entityId == entityId) {
            std::cout << "[" << e.timestamp << "] " << e.action << " — " << e.details << "\n";
            found = true;
        }
    }
    if (!found) std::cout << "No entries.\n";
    std::cout << "============================\n";
}

void AuditLog::clear() {
    std::ofstream file(AUDIT_FILE, std::ios::trunc);
}
