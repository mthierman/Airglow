#include "Config.hxx"

Config::Config() {}

std::unique_ptr<Config> Config::Create()
{
    auto config{std::unique_ptr<Config>(new Config())};

    config->Load();

    if (!std::filesystem::exists(config->paths.data) ||
        !std::filesystem::exists(config->paths.config))
        return nullptr;

    return config;
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
#ifdef _DEBUG
        println(e.what());
#endif
    }
}

void Config::Load()
{
    paths.data = DataPath();
    paths.config = ConfigPath();
    paths.db = DbPath();

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
#ifdef _DEBUG
            println(e.what());
#endif
        }
    }
}

path Config::DataPath()
{
    PWSTR buffer{};

    if (FAILED(SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, nullptr, &buffer)))
        return path{};

    path path = wstring(buffer) + path::preferred_separator + L"Airglow";

    CoTaskMemFree(buffer);

    if (!std::filesystem::exists(path))
        std::filesystem::create_directory(path);

    return path;
}

path Config::ConfigPath()
{
    if (!std::filesystem::exists(paths.data))
        return path{};

    return (paths.data.wstring() + path::preferred_separator + L"Config.json");
}

path Config::DbPath()
{
    if (!std::filesystem::exists(paths.data))
        return path{};

    return (paths.data.wstring() + path::preferred_separator + L"Database.sqlite");
}

void Config::Tests()
{
    println(settings.mainUrl);
    println(settings.sideUrl);
    println(bool_to_string(settings.fullscreen));
    println(bool_to_string(settings.maximized));
    println(bool_to_string(settings.menu));
    println(bool_to_string(settings.split));
    println(bool_to_string(settings.swapped));
    println(bool_to_string(settings.topmost));
    println(std::to_string(settings.position[0]));
    println(std::to_string(settings.position[1]));
    println(std::to_string(settings.position[2]));
    println(std::to_string(settings.position[3]));
}
