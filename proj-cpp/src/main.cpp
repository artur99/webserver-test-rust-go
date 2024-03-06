#include <algorithm>
#include <cerrno>
#include <chrono>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include "App.h"
#include "Database.h"

std::vector<Database::Entry> generateData(int n)
{
    std::vector<Database::Entry> result(n);
    srand(time(NULL));
    for (int i = 0; i < n; ++i)
    {
        int K = rand() % 100000;

        result[i].name = "name" + std::to_string(10000000 - i);
        result[i].value = "value" + std::to_string(K) + std::to_string(i);
    }

    return result;
}

int main()
{
    constexpr int32_t PORT = 3004;
    constexpr uint32_t NUM_CONNECTIONS = 10;

    Database db(NUM_CONNECTIONS);
    std::cout << "Listening on http://127.0.0.1:" << PORT << std::endl;

    std::array<std::unique_ptr<std::thread>, NUM_CONNECTIONS> threads;

    for (auto &thread : threads)
    {
        thread = std::make_unique<std::thread>([&] {
            uWS::App()
                .get("/",
                     [](auto *res, auto *req) {
                         res->writeStatus(uWS::HTTP_200_OK);
                         res->writeHeader("Content-Type", "text/plain");
                         res->end("Hello, World!");
                     })
                .get("/reset_and_insert_data",
                     [&db](auto *res, auto *req) {
                         /* Make sure we have the right query */
                         auto query = req->getQuery().data();
                         assert(query[0] == 'n' && query[1] == '=');
                         query += 2;

                         /* Convert query to long */
                         char *endptr = nullptr;
                         long n = strtol(query, &endptr, 10);
                         assert(endptr != query);

                         auto begin = std::chrono::system_clock::now();

                         auto data = generateData(n);

                         db.reset();
                         for (auto const &entry : data)
                         {
                             db.insertEntry(entry.name, entry.value);
                         }
                         auto end = std::chrono::system_clock::now();
                         auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();

                         res->writeStatus(uWS::HTTP_200_OK);
                         res->writeHeader("Content-Type", "text/plain");
                         char message[1024];
                         sprintf(message, "Inserted %ld entries in db in %ld ms.", n, duration);
                         res->end(message);
                     })
                .get("/get_first_values",
                     [&](auto *res, auto *req) {
                         /* Make sure we have the right query */
                         auto query = req->getQuery().data();
                         assert(query[0] == 'n' && query[1] == '=');
                         query += 2;

                         /* Convert query to long */
                         char *endptr = nullptr;
                         long n = strtol(query, &endptr, 10);
                         assert(endptr != query);

                         auto begin = std::chrono::system_clock::now();

                         auto entries = db.getEntries(n);

                         std::sort(entries.begin(), entries.end(),
                                   [](Database::Entry const &lhs, Database::Entry const &rhs) {
                                       return lhs.value < rhs.value;
                                   });

                         auto end = std::chrono::system_clock::now();

                         auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();

                         res->writeStatus(uWS::HTTP_200_OK);
                         res->writeHeader("Content-Type", "text/plain");
                         char message[1024];
                         sprintf(message, "Read %ld entries from db in %ld ms.", n, duration);
                         res->end(message);
                     })
                .listen(PORT, [](auto *token) { (void)token; })
                .run();
        });
    }

    for (auto &thread : threads)
    {
        thread->join();
    }
}