#include "Database.hxx"

Database::Database(Config* config) {}

std::unique_ptr<Database> Database::Create(Config* config)
{
    auto pDatabase = std::unique_ptr<Database>();

    sqlite3* db;

    auto test = sqlite3_open("Airglow.db", &db);

    return pDatabase;
}
