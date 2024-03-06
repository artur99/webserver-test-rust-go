#include "Database.h"
#include <cassert>
#include <iostream>
#include <string>

void Database::initConnections()
{
    char defaultHost[100] = "localhost";
    char connectionString[200];
    char const *dbHost = std::getenv("DB_HOST");
    if(dbHost)
    {
        strncpy(defaultHost, dbHost, sizeof(defaultHost) - 1);
    }

    snprintf(
        connectionString,
        sizeof(connectionString),
        "user=postgres password=postgres host=%s port=5432 dbname=postgres target_session_attrs=read-write",
        defaultHost
    );

    for (uint32_t i = 0; i < mNumConnections; ++i)
    {
        mConnections.emplace_back(connectionString);
        assert(mConnections.back().is_open());
    }
}

void Database::reset()
{
    pqxx::work w(getConnection());

    w.exec("DROP TABLE IF EXISTS data");
    w.exec(R"(CREATE TABLE data (
            id SERIAL PRIMARY KEY,
            name VARCHAR(100) NOT NULL,
            value VARCHAR(100) NOT NULL
        ))");

    w.commit();
}

void Database::insertEntry(std::string_view name, std::string_view value)
{
    pqxx::work w(getConnection());

    w.exec_params("INSERT INTO data(name, value) VALUES ($1, $2)", name.data(), value.data());

    w.commit();
}

void Database::insertEntries(std::vector<Entry> const &entries)
{
    pqxx::work w(getConnection());

    for (auto const &entry : entries)
    {
        w.exec_params("INSERT INTO data(name, value) VALUES ($1, $2)", entry.name.data(), entry.value.data());
    }

    w.commit();
}

std::vector<Database::Entry> Database::getEntries(uint32_t n)
{
    pqxx::work w(getConnection());

    pqxx::result rows = w.exec("SELECT name, value FROM data");
    assert(rows.size() == n);

    std::vector<Entry> result(rows.size());
    for (pqxx::result::size_type i = 0; i < rows.size(); ++i)
    {
        std::tie(result[i].name, result[i].value) = rows[i].as<std::string, std::string>();
    }

    return result;
}
