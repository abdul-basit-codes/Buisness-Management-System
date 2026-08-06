#ifndef SERVICEPACKAGE_H
#define SERVICEPACKAGE_H

#include <string>
#include <vector>
#include <utility>
#include "Service.h"

struct ServicePackage {
    int id;
    std::string name;
    std::vector<int> serviceIds;
    double discountPercent;
    bool active;
};

class PackageManager {
public:
    static ServicePackage create(const std::string& name,
                                 const std::vector<int>& serviceIds,
                                 double discountPercent);
    static std::vector<ServicePackage> getAll();
    static double packagePrice(const ServicePackage& pkg,
                               const std::vector<Service>& services);
    static double savingsAmount(const ServicePackage& pkg,
                                const std::vector<Service>& services);
    static void list(const std::vector<Service>& services);
    static bool deactivate(int id);

private:
    static std::vector<ServicePackage> load();
    static void save(const std::vector<ServicePackage>& packages);
    static int nextId();
};

#endif
