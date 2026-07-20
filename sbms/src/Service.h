#ifndef SERVICE_H
#define SERVICE_H

#include <string>

class Service {
public:
    Service();
    Service(int id, const std::string& name, double price, int durationMinutes);

    int getId() const;
    std::string getName() const;
    double getPrice() const;
    int getDuration() const;

    void setName(const std::string& name);
    void setPrice(double price);
    void setDuration(int durationMinutes);

    std::string toCSV() const;
    static Service fromCSV(const std::string& line);

    void print() const;

private:
    int id;
    std::string name;
    double price;
    int durationMinutes;
};

#endif
