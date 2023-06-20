#include "Config.hxx"

Config::Config() {}

std::unique_ptr<Config> Config::Create()
{
    auto pConfig = std::unique_ptr<Config>(new Config());
    pConfig->pathData = pConfig->DataPath();
    pConfig->pathSettings = pConfig->SettingsPath(pConfig->pathData);
    pConfig->Load();

    return pConfig;
}

std::filesystem::path Config::DataPath()
{
    std::filesystem::path path;
    std::wstring outBuffer;
    PWSTR buffer;

    if (SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, nullptr, &buffer) != S_OK)
    {
        CoTaskMemFree(buffer);
        return path;
    }

    path = std::wstring(buffer) + std::filesystem::path::preferred_separator + L"Airglow";

    CoTaskMemFree(buffer);

    if (!std::filesystem::exists(path))
        std::filesystem::create_directory(path);

    return path;
}

std::filesystem::path Config::SettingsPath(std::filesystem::path path)
{
    std::filesystem::path config =
        (path.wstring() + std::filesystem::path::preferred_separator + L"config.json");

    return config;
}

tao::json::value Config::Defaults()
{
    tao::json::value config = {{"position", tao::json::value::array({0, 0, 800, 600})},
                               {"menu", false},
                               {"split", false},
                               {"swapped", false},
                               {"maximized", false},
                               {"fullscreen", false},
                               {"topmost", false},
                               {"main", "https://www.google.com/"},
                               {"side", "https://www.google.com/"}};

    return config;
}

tao::json::value Config::Current()
{
    tao::json::value config;
    config["position"] = vectorPosition;
    config["menu"] = boolMenu;
    config["split"] = boolSplit;
    config["swapped"] = boolSwapped;
    config["maximized"] = boolMaximized;
    config["fullscreen"] = boolFullscreen;
    config["topmost"] = boolTopmost;
    config["main"] = stringMain;
    config["side"] = stringSide;

    return config;
}

void Config::Load()
{
    auto config = Defaults();

    if (!std::filesystem::exists(pathSettings))
    {
        std::ofstream f(pathSettings);
        f << std::setw(4) << config << std::endl;
        f.close();
    }

    if (std::filesystem::exists(pathSettings))
    {
        std::ifstream f(pathSettings);
        if (!std::filesystem::is_empty(pathSettings))
            config = tao::json::from_file(pathSettings);
        f.close();
    }

    vectorPosition = config.as<std::vector<int>>("position");
    boolMenu = config.as<bool>("menu");
    boolSplit = config.as<bool>("split");
    boolSwapped = config.as<bool>("swapped");
    boolMaximized = config.as<bool>("maximized");
    boolFullscreen = config.as<bool>("fullscreen");
    boolTopmost = config.as<bool>("topmost");
    stringMain = config.as<std::string>("main");
    stringSide = config.as<std::string>("side");
}

void Config::SaveOnce()
{
    auto config = Current();

    if (std::filesystem::exists(pathSettings))
    {
        std::ofstream f(pathSettings);
        f << std::setw(4) << config << std::endl;
        f.close();
    }
}

void Config::Save()
{
    streamSettingsPath = std::ofstream(pathSettings);
    auto config = Current();
    streamSettingsPath << std::setw(4) << config << std::endl;
    streamSettingsPath.close();
}

void Config::Tests()
{
    Utility::print(std::to_string(vectorPosition[0]));
    Utility::print(std::to_string(vectorPosition[1]));
    Utility::print(std::to_string(vectorPosition[2]));
    Utility::print(std::to_string(vectorPosition[3]));
    Utility::print(Utility::BoolToString(boolMenu));
    Utility::print(Utility::BoolToString(boolSplit));
    Utility::print(Utility::BoolToString(boolSwapped));
    Utility::print(Utility::BoolToString(boolMaximized));
    Utility::print(Utility::BoolToString(boolFullscreen));
    Utility::print(Utility::BoolToString(boolTopmost));
    Utility::print(stringMain);
    Utility::print(stringSide);
}
