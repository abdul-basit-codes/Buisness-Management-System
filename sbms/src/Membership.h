#ifndef MEMBERSHIP_H
#define MEMBERSHIP_H

#include <string>
#include <vector>
#include <optional>
#include "Customer.h"

struct Membership {
    int id;
    int customerId;
    std::string tier;
    int points;
    std::string since;
    std::string expires;
};

class MembershipManager {
public:
    static const int TIER_THRESHOLDS[4];
    static const std::string TIER_NAMES[4];

    static Membership enroll(int customerId);
    static std::vector<Membership> getAll();
    static std::optional<Membership> findByCustomer(int customerId);
    static bool addPoints(int customerId, int points);
    static bool redeemPoints(int customerId, int points);
    static std::string tierForPoints(int points);
    static bool isExpired(const Membership& m);
    static void list(const std::vector<Customer>& customers);
    static bool cancel(int customerId);

private:
    static std::vector<Membership> load();
    static void save(const std::vector<Membership>& memberships);
    static int nextId();
    static std::string today();
    static std::string addOneYear(const std::string& date);
};

#endif
