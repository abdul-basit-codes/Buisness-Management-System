#ifndef SEARCH_H
#define SEARCH_H

#include <string>
#include <vector>
#include <functional>

class SearchUtils {
public:
    template <typename T>
    static std::vector<T> filter(const std::vector<T>& items,
                                 std::function<bool(const T&)> predicate) {
        std::vector<T> result;
        for (const auto& item : items)
            if (predicate(item)) result.push_back(item);
        return result;
    }

    template <typename T>
    static T* findById(std::vector<T>& items, int id) {
        for (auto& item : items)
            if (item.getId() == id) return &item;
        return nullptr;
    }

    template <typename T>
    static std::vector<T> searchByName(const std::vector<T>& items,
                                       const std::string& query) {
        std::vector<T> result;
        std::string lowerQuery = toLower(query);
        for (const auto& item : items)
            if (toLower(item.getName()).find(lowerQuery) != std::string::npos)
                result.push_back(item);
        return result;
    }

    template <typename T>
    static std::vector<T> sortBy(std::vector<T> items,
                                 std::function<bool(const T&, const T&)> comparator) {
        std::sort(items.begin(), items.end(), comparator);
        return items;
    }

    template <typename T>
    static double sum(const std::vector<T>& items,
                      std::function<double(const T&)> extractor) {
        double total = 0;
        for (const auto& item : items) total += extractor(item);
        return total;
    }

    template <typename T>
    static T* findMax(std::vector<T>& items,
                      std::function<double(const T&)> evaluator) {
        if (items.empty()) return nullptr;
        T* best = &items[0];
        for (auto& item : items)
            if (evaluator(item) > evaluator(*best)) best = &item;
        return best;
    }

    template <typename T>
    static T* findMin(std::vector<T>& items,
                      std::function<double(const T&)> evaluator) {
        if (items.empty()) return nullptr;
        T* best = &items[0];
        for (auto& item : items)
            if (evaluator(item) < evaluator(*best)) best = &item;
        return best;
    }

private:
    static std::string toLower(const std::string& s) {
        std::string result = s;
        std::transform(result.begin(), result.end(), result.begin(), ::tolower);
        return result;
    }
};

#endif
