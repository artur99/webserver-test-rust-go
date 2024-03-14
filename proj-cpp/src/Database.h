#pragma once

#include <pqxx/pqxx>
#include <string_view>

class Database
{
public:
    struct Entry
    {
        std::string name;
        std::string value;
    };

public:
    Database(uint32_t numConnections) noexcept
    {
        initConnections(numConnections);
    }

    void reset(uint32_t threadId) noexcept;
    void insertEntry(uint32_t threadId, std::string_view name, std::string_view value) noexcept;
    void insertEntries(uint32_t threadId, std::vector<Entry> const &entries) noexcept;
    std::vector<Entry> getEntries(uint32_t threadId, uint32_t n) noexcept;

private:
    void initConnections(uint32_t numConnections) noexcept;

private:
    std::vector<pqxx::connection> mConnections;
};
