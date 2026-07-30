#ifndef AUDITLOG_H
#define AUDITLOG_H

#include <string>
#include <vector>

struct AuditEntry {
    int id;
    std::string action;
    std::string entityType;
    int entityId;
    std::string details;
    std::string timestamp;
};

class AuditLog {
public:
    static AuditEntry log(const std::string& action, const std::string& entityType,
                          int entityId, const std::string& details);
    static std::vector<AuditEntry> getRecent(int count = 10);
    static void printRecent(int count = 10);
    static void printByEntity(const std::string& entityType, int entityId);
    static void clear();

private:
    static std::vector<AuditEntry> load();
    static void save(const std::vector<AuditEntry>& entries);
    static int nextId();
    static std::string now();
};

#endif
