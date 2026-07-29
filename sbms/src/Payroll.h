#ifndef PAYROLL_H
#define PAYROLL_H

#include <string>
#include <vector>
#include "Employee.h"

struct PayrollEntry {
    int employeeId;
    std::string employeeName;
    double baseSalary;
    double bonus;
    double deductions;
    double netPay;
};

class Payroll {
public:
    static PayrollEntry calculatePay(const Employee& emp, double bonusPercent, double taxPercent);
    static std::vector<PayrollEntry> generatePayroll(const std::vector<Employee>& employees,
                                                      double bonusPercent, double taxPercent);
    static void printPayroll(const std::vector<PayrollEntry>& entries);
    static void printPayStub(const PayrollEntry& entry);
};

#endif
