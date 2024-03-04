#pragma once

#include <cassert>
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
    Database()
        : mConnection("user=postgres password=postgres host=localhost port=5432 dbname=postgres "
                      "target_session_attrs=read-write")
    {
        assert(mConnection.is_open());
    }

    void reset();
    void insertEntry(std::string_view name, std::string_view value);
    void insertEntries(std::vector<Entry> const &entries);
    std::vector<Entry> getEntries(uint32_t n);

private:
    pqxx::connection mConnection;
};