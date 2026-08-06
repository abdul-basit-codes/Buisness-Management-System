#include "ExpenseTracking.h"
#include "AuditLog.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <ctime>

const std::string EXPENSE_FILE = "data/expenses.txt";

std::string ExpenseManager::today() {
    std::time_t t = std::time(nullptr);
    std::tm* tm = std::localtime(&t);
    char buf[16];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d", tm);
    return buf;
}

int ExpenseManager::nextId() {
    auto expenses = load();
    int maxId = 0;
    for (const auto& e : expenses) maxId = std::max(maxId, e.id);
    return maxId + 1;
}

Expense ExpenseManager::add(const std::string& description, double amount, const std::string& category) {
    auto expenses = load();
    Expense e{nextId(), description, amount, category, today()};
    expenses.push_back(e);
    save(expenses);
    AuditLog::log("CREATE", "Expense", e.id, description + " ($" + std::to_string(amount) + ", " + category + ")");
    return e;
}

std::vector<Expense> ExpenseManager::load() {
    std::vector<Expense> expenses;
    std::ifstream file(EXPENSE_FILE);
    if (!file.is_open()) return expenses;
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string idStr, desc, amtStr, cat, date;
        std::getline(ss, idStr, '|');
        std::getline(ss, desc, '|');
        std::getline(ss, amtStr, '|');
        std::getline(ss, cat, '|');
        std::getline(ss, date, '|');
        expenses.push_back({std::stoi(idStr), desc, std::stod(amtStr), cat, date});
    }
    return expenses;
}

void ExpenseManager::save(const std::vector<Expense>& expenses) {
    std::ofstream file(EXPENSE_FILE, std::ios::trunc);
    for (const auto& e : expenses)
        file << e.id << "|" << e.description << "|" << e.amount << "|" << e.category << "|" << e.date << "\n";
}

std::vector<Expense> ExpenseManager::getAll() { return load(); }

double ExpenseManager::totalExpenses() {
    double total = 0.0;
    for (const auto& e : load()) total += e.amount;
    return total;
}

double ExpenseManager::totalByCategory(const std::string& category) {
    double total = 0.0;
    for (const auto& e : load())
        if (e.category == category) total += e.amount;
    return total;
}

void ExpenseManager::list() {
    auto expenses = load();
    std::cout << "\n=== EXPENSES ===\n";
    if (expenses.empty()) { std::cout << "No expenses recorded.\n"; return; }
    for (const auto& e : expenses)
        std::cout << "  [" << e.id << "] " << e.date << " " << e.description
                  << " ($" << std::fixed << std::setprecision(2) << e.amount
                  << ", " << e.category << ")\n";
    std::cout << "  Total: $" << std::fixed << std::setprecision(2) << totalExpenses() << "\n";
    std::cout << "===============\n";
}

bool ExpenseManager::remove(int id) {
    auto expenses = load();
    for (auto it = expenses.begin(); it != expenses.end(); ++it) {
        if (it->id == id) {
            expenses.erase(it);
            save(expenses);
            return true;
        }
    }
    return false;
}
