#ifndef INVENTORY_H
#define INVENTORY_H

#include <string>
#include <vector>

class InventoryItem {
private:
    int id;
    std::string name;
    std::string category;
    int quantity;
    double unitPrice;
    int lowStockThreshold;

public:
    InventoryItem();
    InventoryItem(int id, const std::string& name, const std::string& category,
                  int qty, double price, int threshold = 10);

    int getId() const;
    std::string getName() const;
    std::string getCategory() const;
    int getQuantity() const;
    double getUnitPrice() const;
    int getLowStockThreshold() const;
    double getTotalValue() const;
    bool isLowStock() const;

    void setQuantity(int qty);
    void setUnitPrice(double price);
    void setLowStockThreshold(int threshold);

    void addStock(int qty);
    bool removeStock(int qty);

    std::string toString() const;
    static InventoryItem fromString(const std::string& line);
};

#endif
