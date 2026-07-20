#ifndef BOOKING_H
#define BOOKING_H

#include <string>

class Booking {
public:
    Booking();
    Booking(int id, int customerId, int serviceId, int employeeId,
             const std::string& date, bool completed);

    int getId() const;
    int getCustomerId() const;
    int getServiceId() const;
    int getEmployeeId() const;
    std::string getDate() const;
    bool isCompleted() const;

    void setCompleted(bool completed);

    std::string toCSV() const;
    static Booking fromCSV(const std::string& line);

    void print() const;

private:
    int id;
    int customerId;
    int serviceId;
    int employeeId;
    std::string date;
    bool completed;
};

#endif
