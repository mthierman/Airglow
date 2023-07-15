#include "Config.hxx"

Config::Config() {}

std::unique_ptr<Config> Config::Create()
{
    auto config{std::unique_ptr<Config>(new Config())};

    SetEnvironmentVariableW(to_wide("WEBVIEW2_DEFAULT_BACKGROUND_COLOR").c_str(),
                            to_wide("0").c_str());

    config->Initialize();

    config->Load();

    config->InitializeColors();

    if (!std::filesystem::exists(config->paths.data) ||
        !std::filesystem::exists(config->paths.config))
        return nullptr;

    return config;
}

void Config::Load()
{
    if (!std::filesystem::exists(paths.config))
        Save();

    if (std::filesystem::exists(paths.config) && !std::filesystem::is_empty(paths.config))
    {
        try
        {
            json j{};
            ifstream f(paths.config);
            j = json::parse(f, nullptr, false, true);
            f.close();

            settings.position = j["position"].get<std::vector<int>>();
            settings.menu = j["menu"].get<bool>();
            settings.split = j["split"].get<bool>();
            settings.swapped = j["swapped"].get<bool>();
            settings.maximized = j["maximized"].get<bool>();
            settings.fullscreen = j["fullscreen"].get<bool>();
            settings.topmost = j["topmost"].get<bool>();
            settings.mainUrl = j["mainUrl"].get<string>();
            settings.sideUrl = j["sideUrl"].get<string>();
        }
        catch (const std::exception& e)
        {
            return;
        }
    }
}

void Config::Save()
{
    try
    {
        json j{};
        j["position"] = settings.position;
        j["menu"] = settings.menu;
        j["split"] = settings.split;
        j["swapped"] = settings.swapped;
        j["maximized"] = settings.maximized;
        j["fullscreen"] = settings.fullscreen;
        j["topmost"] = settings.topmost;
        j["mainUrl"] = settings.mainUrl;
        j["sideUrl"] = settings.sideUrl;

        ofstream f(paths.config);
        f << std::setw(4) << j << "\n";
        f.close();
    }
    catch (const std::exception& e)
    {
        return;
    }
}

void Config::Initialize()
{
    app.name = APP_NAME;
    app.version = APP_VERSION;
    paths.data = PortableAppDataPath();
    paths.settings = SettingsPath();
    paths.config = ConfigPath();
    paths.db = DbPath();
    paths.js = JsPath();
}

void Config::InitializeColors()
{
    colors.accent = get_system_color(winrt::Windows::UI::ViewManagement::UIColorType::Accent);
    colors.accentDark1 =
        get_system_color(winrt::Windows::UI::ViewManagement::UIColorType::AccentDark1);
    colors.accentDark2 =
        get_system_color(winrt::Windows::UI::ViewManagement::UIColorType::AccentDark2);
    colors.accentDark3 =
        get_system_color(winrt::Windows::UI::ViewManagement::UIColorType::AccentDark3);
    colors.accentLight1 =
        get_system_color(winrt::Windows::UI::ViewManagement::UIColorType::AccentLight1);
    colors.accentLight2 =
        get_system_color(winrt::Windows::UI::ViewManagement::UIColorType::AccentLight2);
    colors.accentLight3 =
        get_system_color(winrt::Windows::UI::ViewManagement::UIColorType::AccentLight3);
    colors.Background =
        get_system_color(winrt::Windows::UI::ViewManagement::UIColorType::Background);
    colors.Foreground =
        get_system_color(winrt::Windows::UI::ViewManagement::UIColorType::Foreground);
}

// json Config::GetCurrent()
// {
//     json j{{"settings",
//             {{"mainUrl", pConfig->settings.mainUrl},
//              {"sideUrl", pConfig->settings.sideUrl},
//              {"accentColor", pConfig->settings.accentColor}}}};
// }

path Config::LocalAppDataPath()
{
    PWSTR buffer{};

    if (FAILED(SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, nullptr, &buffer)))
        return path{};

    path path = wstring(buffer) + path::preferred_separator + to_wide("Airglow");

    CoTaskMemFree(buffer);

    if (!std::filesystem::exists(path))
        std::filesystem::create_directory(path);

    return path;
}

path Config::PortableAppDataPath()
{
    auto cmd = GetCommandLineW();
    int count;
    auto args = CommandLineToArgvW(cmd, &count);
    path path{args[0]};
    LocalFree(args);

    return std::filesystem::canonical(path.remove_filename());
}

path Config::SettingsPath()
{
    if (!std::filesystem::exists(paths.data))
        return path{};

    return (paths.data.wstring() + path::preferred_separator + to_wide("settings"));
}

path Config::ConfigPath()
{
    if (!std::filesystem::exists(paths.settings))
        return path{};

    return (paths.settings.wstring() + path::preferred_separator + to_wide("config.json"));
}

path Config::DbPath()
{
    if (!std::filesystem::exists(paths.settings))
        return path{};

    return (paths.settings.wstring() + path::preferred_separator + to_wide("db.sqlite"));
}

path Config::JsPath()
{
    if (!std::filesystem::exists(paths.data))
        return path{};

    return (paths.data.wstring() + path::preferred_separator + to_wide("js") +
            path::preferred_separator + to_wide("inject.js"));
}

void Config::CreateDb()
{
    auto dbFile{(paths.db).string()};
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
        return;
    }

    if (std::filesystem::exists(paths.db))
    {
        auto debExec = sqlite3_exec(db, sql.c_str(), nullptr, 0, &errMsg);
        if (debExec != SQLITE_OK)
        {
            dberror(errMsg);
            sqlite3_free(errMsg);
        }
    }

    if (!std::filesystem::exists(paths.db))
        return;

    sqlite3_close(db);

    return;
}
