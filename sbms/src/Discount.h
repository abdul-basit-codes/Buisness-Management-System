#ifndef DISCOUNT_H
#define DISCOUNT_H

#include <string>
#include <vector>

struct Discount {
    int id;
    std::string code;
    double percent;
    std::string description;
    bool active;
};

class DiscountManager {
public:
    static Discount create(const std::string& code, double percent, const std::string& description);
    static std::vector<Discount> getAll();
    static Discount* findByCode(const std::string& code);
    static bool applyToInvoice(int invoiceId, const std::string& code);
    static void list();
    static bool deactivate(int id);

private:
    static std::vector<Discount> load();
    static void save(const std::vector<Discount>& discounts);
    static int nextId();
};

#endif
