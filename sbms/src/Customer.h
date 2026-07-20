#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>

class Customer {
public:
    Customer();
    Customer(int id, const std::string& name, const std::string& phone, const std::string& address);

    int getId() const;
    std::string getName() const;
    std::string getPhone() const;
    std::string getAddress() const;

    void setName(const std::string& name);
    void setPhone(const std::string& phone);
    void setAddress(const std::string& address);

    std::string toCSV() const;
    static Customer fromCSV(const std::string& line);

    void print() const;

private:
    int id;
    std::string name;
    std::string phone;
    std::string address;
};

#endif
