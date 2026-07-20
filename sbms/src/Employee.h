#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <string>

class Employee {
public:
    Employee();
    Employee(int id, const std::string& name, const std::string& role, double salary);

    int getId() const;
    std::string getName() const;
    std::string getRole() const;
    double getSalary() const;

    void setName(const std::string& name);
    void setRole(const std::string& role);
    void setSalary(double salary);

    std::string toCSV() const;
    static Employee fromCSV(const std::string& line);

    void print() const;

private:
    int id;
    std::string name;
    std::string role;
    double salary;
};

#endif
