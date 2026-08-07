#include "Membership.h"
#include "AuditLog.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <ctime>

const std::string MEMBERSHIPS_FILE = "data/memberships.txt";

const int MembershipManager::TIER_THRESHOLDS[4] = {0, 200, 500, 1000};
const std::string MembershipManager::TIER_NAMES[4] = {"Bronze", "Silver", "Gold", "Platinum"};

Membership MembershipManager::enroll(int customerId) {
    auto memberships = load();
    for (const auto& m : memberships)
        if (m.customerId == customerId) {
            std::cout << "Customer " << customerId << " is already enrolled.\n";
            return m;
        }
    Membership m{nextId(), customerId, TIER_NAMES[0], 0, today(), addOneYear(today())};
    memberships.push_back(m);
    save(memberships);
    AuditLog::log("CREATE", "Membership", m.id, "Enrolled customer " + std::to_string(customerId) + " as " + m.tier);
    std::cout << "Enrolled customer " << customerId << " as " << m.tier << ".\n";
    return m;
}

std::string MembershipManager::tierForPoints(int points) {
    std::string tier = TIER_NAMES[0];
    for (int i = 1; i < 4; ++i)
        if (points >= TIER_THRESHOLDS[i]) tier = TIER_NAMES[i];
    return tier;
}

bool MembershipManager::addPoints(int customerId, int points) {
    auto memberships = load();
    for (auto& m : memberships) {
        if (m.customerId == customerId) {
            m.points += points;
            std::string oldTier = m.tier;
            m.tier = tierForPoints(m.points);
            save(memberships);
            if (m.tier != oldTier)
                std::cout << "Customer " << customerId << " upgraded to " << m.tier << "!\n";
            return true;
        }
    }
    return false;
}

bool MembershipManager::redeemPoints(int customerId, int points) {
    auto memberships = load();
    for (auto& m : memberships) {
        if (m.customerId == customerId && m.points >= points) {
            m.points -= points;
            m.tier = tierForPoints(m.points);
            save(memberships);
            AuditLog::log("REDEEM", "Membership", m.id, "Redeemed " + std::to_string(points) + " points for customer " + std::to_string(customerId));
            std::cout << "Redeemed " << points << " points. Balance: " << m.points << "\n";
            return true;
        }
    }
    std::cout << "Redemption failed: customer not found or insufficient points.\n";
    return false;
}

bool MembershipManager::isExpired(const Membership& m) {
    return m.expires < today();
}

std::vector<Membership> MembershipManager::load() {
    std::vector<Membership> memberships;
    std::ifstream file(MEMBERSHIPS_FILE);
    if (!file.is_open()) return memberships;
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string idStr, custStr, tier, pointsStr, since, expires;
        std::getline(ss, idStr, '|');
        std::getline(ss, custStr, '|');
        std::getline(ss, tier, '|');
        std::getline(ss, pointsStr, '|');
        std::getline(ss, since, '|');
        std::getline(ss, expires, '|');
        memberships.push_back({std::stoi(idStr), std::stoi(custStr), tier, std::stoi(pointsStr), since, expires});
    }
    return memberships;
}

void MembershipManager::save(const std::vector<Membership>& memberships) {
    std::ofstream file(MEMBERSHIPS_FILE, std::ios::trunc);
    for (const auto& m : memberships)
        file << m.id << "|" << m.customerId << "|" << m.tier << "|"
             << m.points << "|" << m.since << "|" << m.expires << "\n";
}

int MembershipManager::nextId() {
    auto memberships = load();
    int maxId = 0;
    for (const auto& m : memberships) maxId = std::max(maxId, m.id);
    return maxId + 1;
}

std::vector<Membership> MembershipManager::getAll() { return load(); }

std::optional<Membership> MembershipManager::findByCustomer(int customerId) {
    auto memberships = load();
    for (const auto& m : memberships)
        if (m.customerId == customerId) return m;
    return std::nullopt;
}

void MembershipManager::list(const std::vector<Customer>& customers) {
    auto memberships = load();
    std::cout << "\n=== MEMBERSHIPS ===\n";
    if (memberships.empty()) { std::cout << "No memberships.\n"; return; }
    for (const auto& m : memberships) {
        std::string name = "Customer #" + std::to_string(m.customerId);
        for (const auto& c : customers)
            if (c.getId() == m.customerId) { name = c.getName(); break; }
        std::cout << "  [" << m.id << "] " << name << " — " << m.tier
                  << " (" << m.points << " pts)"
                  << (isExpired(m) ? " [EXPIRED]" : "")
                  << "\n    Since: " << m.since << "  Expires: " << m.expires << "\n";
    }
    std::cout << "===================\n";
}

bool MembershipManager::cancel(int customerId) {
    auto memberships = load();
    auto it = std::remove_if(memberships.begin(), memberships.end(),
        [customerId](const Membership& m) { return m.customerId == customerId; });
    if (it != memberships.end()) {
        memberships.erase(it, memberships.end());
        save(memberships);
        AuditLog::log("CANCEL", "Membership", customerId, "Cancelled membership for customer " + std::to_string(customerId));
        return true;
    }
    return false;
}

std::string MembershipManager::today() {
    std::time_t t = std::time(nullptr);
    std::tm* tm = std::localtime(&t);
    char buf[16];
    std::snprintf(buf, sizeof(buf), "%04d-%02d-%02d", tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday);
    return buf;
}

std::string MembershipManager::addOneYear(const std::string& date) {
    int year = std::stoi(date.substr(0, 4));
    std::string rest = date.substr(4);
    return std::to_string(year + 1) + rest;
}
