#include "Inventory.h"
#include <sstream>
#include <iostream>

InventoryItem::InventoryItem()
    : id(0), quantity(0), unitPrice(0.0), lowStockThreshold(10) {}

InventoryItem::InventoryItem(int id, const std::string& name, const std::string& category,
                             int qty, double price, int threshold)
    : id(id), name(name), category(category), quantity(qty),
      unitPrice(price), lowStockThreshold(threshold) {}

int InventoryItem::getId() const { return id; }
std::string InventoryItem::getName() const { return name; }
std::string InventoryItem::getCategory() const { return category; }
int InventoryItem::getQuantity() const { return quantity; }
double InventoryItem::getUnitPrice() const { return unitPrice; }
int InventoryItem::getLowStockThreshold() const { return lowStockThreshold; }
double InventoryItem::getTotalValue() const { return quantity * unitPrice; }
bool InventoryItem::isLowStock() const { return quantity <= lowStockThreshold; }

void InventoryItem::setQuantity(int qty) { quantity = qty; }
void InventoryItem::setUnitPrice(double price) { unitPrice = price; }
void InventoryItem::setLowStockThreshold(int threshold) { lowStockThreshold = threshold; }

void InventoryItem::addStock(int qty) {
    if (qty > 0) quantity += qty;
}

bool InventoryItem::removeStock(int qty) {
    if (qty <= 0 || qty > quantity) return false;
    quantity -= qty;
    return true;
}

std::string InventoryItem::toString() const {
    std::ostringstream ss;
    ss << id << "|" << name << "|" << category << "|"
       << quantity << "|" << unitPrice << "|" << lowStockThreshold;
    return ss.str();
}

InventoryItem InventoryItem::fromString(const std::string& line) {
    std::istringstream ss(line);
    std::string token;
    InventoryItem item;

    std::getline(ss, token, '|'); item.id = std::stoi(token);
    std::getline(ss, item.name, '|');
    std::getline(ss, item.category, '|');
    std::getline(ss, token, '|'); item.quantity = std::stoi(token);
    std::getline(ss, token, '|'); item.unitPrice = std::stod(token);
    std::getline(ss, token, '|'); item.lowStockThreshold = std::stoi(token);

    return item;
}
