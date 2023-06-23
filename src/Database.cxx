#include "Database.hxx"

Config* Database::pConfig = nullptr;

Database::Database(Config* config) {}

std::unique_ptr<Database> Database::Create(Config* config)
{
    pConfig = config;

    auto pDatabase = std::unique_ptr<Database>();

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

    std::string dbFile = pConfig->pathData.string() + "/" + "Airglow.sqlite";
    const char* dbPath = dbFile.c_str();

    auto dbOpen = sqlite3_open(dbPath, &db);

    if (dbOpen)
    {
        string err = "Database opening failed";
        error(err);
        return 0;
    }

    sqlite3_exec(db, sql.c_str(), NULL, 0, &messageError);

    sqlite3_close(db);

    return pDatabase;
}
