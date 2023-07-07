#include "Database.hxx"

Config* Database::pConfig{nullptr};

Database::Database(Config* config) {}

std::unique_ptr<Database> Database::Create(Config* config)
{
    pConfig = config;

    std::string dbFile = (pConfig->pathData / "Airglow.sqlite").string();

    auto pDatabase = std::unique_ptr<Database>(new Database(pConfig));

    sqlite3* db;
    std::string sql = "CREATE TABLE CONFIG("
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
                      "SIDE TEXT NOT NULL);";
    char* messageError;

    const char* dbPath = dbFile.c_str();

    auto dbOpen = sqlite3_open(dbPath, &db);

    if (dbOpen != SQLITE_OK)
    {
        error("Database opening failed");
        return 0;
    }

    auto debExec = sqlite3_exec(db, sql.c_str(), nullptr, 0, &messageError);

    if (!messageError)
    {
        error("Database execution failed: " + string(messageError));
        return 0;
    }

    sqlite3_free(messageError);

    sqlite3_close(db);

    return pDatabase;
}
