#include "Database.hxx"

Database::Database(Config* config) {}

std::unique_ptr<Database> Database::Create(Config* config)
{
    auto pDatabase = std::unique_ptr<Database>();

    sqlite3* db;

    path dbFilePath =
        (ToWide(config->pathData.string())) + path::preferred_separator + L"Airglow.db";
    auto dbFilePathConvert = ToString(dbFilePath);
    const char* dbPath = dbFilePathConvert.c_str();

    auto dbOpen = sqlite3_open(dbPath, &db);

    return pDatabase;
}
