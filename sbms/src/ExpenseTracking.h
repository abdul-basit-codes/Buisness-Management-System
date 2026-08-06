#ifndef EXPENSE_TRACKING_H
#define EXPENSE_TRACKING_H

#include <string>
#include <vector>

struct Expense {
    int id;
    std::string description;
    double amount;
    std::string category;
    std::string date;
};

class ExpenseManager {
public:
    static Expense add(const std::string& description, double amount, const std::string& category);
    static std::vector<Expense> getAll();
    static double totalExpenses();
    static double totalByCategory(const std::string& category);
    static void list();
    static bool remove(int id);

private:
    static std::vector<Expense> load();
    static void save(const std::vector<Expense>& expenses);
    static int nextId();
    static std::string today();
};

#endif
