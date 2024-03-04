#pragma once

#include <atomic>
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
    Database(uint32_t numConnections) : mActiveConnection(0), mNumConnections(numConnections)
    {
        initConnections();
    }

    void reset();
    void insertEntry(std::string_view name, std::string_view value);
    void insertEntries(std::vector<Entry> const &entries);
    std::vector<Entry> getEntries(uint32_t n);

private:
    void initConnections();
    pqxx::connection &getConnection()
    {
        uint32_t activeConnection = mActiveConnection;
        mActiveConnection = (mActiveConnection + 1) % mNumConnections;
        return mConnections[activeConnection];
    }

private:
    std::atomic<uint32_t> mActiveConnection;
    uint32_t mNumConnections;
    std::vector<pqxx::connection> mConnections;
};
