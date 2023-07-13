#include "Config.hxx"

Config::Config() {}

std::unique_ptr<Config> Config::Create()
{
    auto config{std::unique_ptr<Config>(new Config())};

    config->Load();
    config->settings.accentColor =
        get_system_color(winrt::Windows::UI::ViewManagement::UIColorType::Accent);

    if (!std::filesystem::exists(config->paths.data) ||
        !std::filesystem::exists(config->paths.config))
        return nullptr;

    return config;
}

void Config::Load()
{
    paths.data = PortableAppDataPath();
    paths.config = ConfigPath();
    paths.db = DbPath();
    paths.js = JsPath();

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
            settings.accentColor = j["accentColor"].get<string>();
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
        j["accentColor"] = settings.accentColor;

        ofstream f(paths.config);
        f << std::setw(4) << j << "\n";
        f.close();
    }
    catch (const std::exception& e)
    {
        return;
    }
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

    return path.remove_filename();
}

path Config::ConfigPath()
{
    if (!std::filesystem::exists(paths.data))
        return path{};

    return (paths.data.wstring() + path::preferred_separator + to_wide("Config.json"));
}

path Config::DbPath()
{
    if (!std::filesystem::exists(paths.data))
        return path{};

    return (paths.data.wstring() + path::preferred_separator + to_wide("Database.sqlite"));
}

path Config::JsPath()
{
    if (!std::filesystem::exists(paths.data))
        return path{};

    return (paths.data.wstring() + path::preferred_separator + to_wide("inject.js"));
}

void Config::Tests()
{
    println(settings.mainUrl);
    println(settings.sideUrl);
    println(settings.accentColor);
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
