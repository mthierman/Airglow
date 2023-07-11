#include "Database.hxx"

Config* Database::pConfig{nullptr};

Database::Database(Config* config) {}

std::unique_ptr<Database> Database::Create(Config* config)
{
    auto database = std::unique_ptr<Database>(new Database(config));

    database->pConfig = config;

    auto dbFile{(pConfig->paths.db).string()};
    auto dbPath{dbFile.c_str()};
    sqlite3* db{nullptr};
    char* errMsg{0};
    string sql{"CREATE TABLE CONFIG("
               "X INT NOT NULL,"
               "Y INT NOT NULL,"
               "WIDTH INT NOT NULL,"
               "HEIGHT INT NOT NULL,"
               "MENU INT NOT NULL,"
               "SPLIT INT NOT NULL,"
               "MAXIMIZED INT NOT NULL,"
               "FULLSCREEN INT NOT NULL,"
               "TOPMOST INT NOT NULL,"
               "MAIN TEXT NOT NULL,"
               "SIDE TEXT NOT NULL);"};

    auto dbOpen = sqlite3_open(dbPath, &db);
    if (dbOpen != SQLITE_OK)
    {
        dberror("Database opening failed");
        return nullptr;
    }

    if (!std::filesystem::exists(pConfig->paths.db))
    {
        auto debExec = sqlite3_exec(db, sql.c_str(), nullptr, 0, &errMsg);
        if (debExec != SQLITE_OK)
        {
            dberror(errMsg);
            sqlite3_free(errMsg);
        }
    }

    if (!std::filesystem::exists(pConfig->paths.db))
        return nullptr;

    sqlite3_close(db);

    return database;
}
