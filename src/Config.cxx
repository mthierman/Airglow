#include "Config.hxx"

Config::Config() {}

std::unique_ptr<Config> Config::Create()
{
    auto pConfig = std::unique_ptr<Config>(new Config());

    pConfig->pathData = pConfig->DataPath();
    pConfig->pathSettings = pConfig->SettingsPath(pConfig->pathData);

    return pConfig;
}

void Config::Load()
{
    tao::json::value config;

    if (std::filesystem::exists(pathSettings) && !std::filesystem::is_empty(pathSettings))
    {
        std::ifstream f(pathSettings);
        config = tao::json::from_file(pathSettings);
        f.close();
    }

    else
    {
        config = Get();
        Save();
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

void Config::Save()
{
    std::ofstream f(pathSettings);
    f << std::setw(4) << Get() << std::endl;
    f.close();
}

tao::json::value Config::Get()
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

void Config::Tests()
{

    Utility::print(Utility::BoolToString(boolFullscreen));
    Utility::print(stringMain);
    Utility::print(Utility::BoolToString(boolMaximized));
    Utility::print(Utility::BoolToString(boolMenu));
    Utility::print(std::to_string(vectorPosition[0]));
    Utility::print(std::to_string(vectorPosition[1]));
    Utility::print(std::to_string(vectorPosition[2]));
    Utility::print(std::to_string(vectorPosition[3]));
    Utility::print(stringSide);
    Utility::print(Utility::BoolToString(boolSplit));
    Utility::print(Utility::BoolToString(boolSwapped));
    Utility::print(Utility::BoolToString(boolTopmost));
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
        (path.wstring() + std::filesystem::path::preferred_separator + L"Config.json");

    return config;
}
