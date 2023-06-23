#include "Database.hxx"

Config* Database::pConfig = nullptr;

Database::Database(Config* config) {}

std::unique_ptr<Database> Database::Create(Config* config)
{
    pConfig = config;

    auto pDatabase = std::unique_ptr<Database>();

    sqlite3* db;

    path dbFile = pConfig->pathData.wstring() + path::preferred_separator + L"Airglow.db";
    auto dbFileS = dbFile.string();
    const char* dbPath = dbFileS.c_str();

    auto dbOpen = sqlite3_open(dbPath, &db);

    if (dbOpen)
    {
        string err = "Database opening failed";
        error(err);
        return 0;
    }

    return pDatabase;
}
