#include "Payroll.h"
#include <iostream>
#include <iomanip>

PayrollEntry Payroll::calculatePay(const Employee& emp, double bonusPercent, double taxPercent) {
    PayrollEntry entry;
    entry.employeeId = emp.getId();
    entry.employeeName = emp.getName();
    entry.baseSalary = emp.getSalary();
    entry.bonus = entry.baseSalary * (bonusPercent / 100.0);
    entry.deductions = entry.baseSalary * (taxPercent / 100.0);
    entry.netPay = entry.baseSalary + entry.bonus - entry.deductions;
    return entry;
}

std::vector<PayrollEntry> Payroll::generatePayroll(const std::vector<Employee>& employees,
                                                    double bonusPercent, double taxPercent) {
    std::vector<PayrollEntry> entries;
    for (const auto& emp : employees) {
        entries.push_back(calculatePay(emp, bonusPercent, taxPercent));
    }
    return entries;
}

void Payroll::printPayroll(const std::vector<PayrollEntry>& entries) {
    std::cout << "\n========== PAYROLL SUMMARY ==========\n";
    double totalGross = 0, totalNet = 0, totalDed = 0, totalBonus = 0;
    for (const auto& e : entries) {
        totalGross += e.baseSalary;
        totalNet += e.netPay;
        totalDed += e.deductions;
        totalBonus += e.bonus;
        std::cout << e.employeeName << " (ID: " << e.employeeId << "): "
                  << "Gross $" << e.baseSalary
                  << " | Bonus $" << e.bonus
                  << " | Deductions $" << e.deductions
                  << " | Net $" << e.netPay << "\n";
    }
    std::cout << "---------------------------------------\n";
    std::cout << "Total Gross: $" << totalGross << "\n";
    std::cout << "Total Bonus: $" << totalBonus << "\n";
    std::cout << "Total Deductions: $" << totalDed << "\n";
    std::cout << "Total Net Payroll: $" << totalNet << "\n";
    std::cout << "=======================================\n";
}

void Payroll::printPayStub(const PayrollEntry& entry) {
    std::cout << "\n========== PAY STUB ==========\n";
    std::cout << "Employee: " << entry.employeeName << " (ID: " << entry.employeeId << ")\n";
    std::cout << "Base Salary:  $" << entry.baseSalary << "\n";
    std::cout << "Bonus:        $" << entry.bonus << "\n";
    std::cout << "Deductions:  -$" << entry.deductions << "\n";
    std::cout << "-----------------------------\n";
    std::cout << "Net Pay:      $" << entry.netPay << "\n";
    std::cout << "==============================\n";
}
