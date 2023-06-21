#include "Config.hxx"

Config::Config() {}

std::unique_ptr<Config> Config::Create()
{
    auto pConfig = std::unique_ptr<Config>(new Config());

    pConfig->pathData = Utility::DataPath();
    pConfig->pathSettings = Utility::SettingsPath(pConfig->pathData);
    pConfig->Load();

    return pConfig;
}

void Config::Load()
{
    json config;

    if (std::filesystem::exists(pathSettings) && !std::filesystem::is_empty(pathSettings))
    {
        std::ifstream f(pathSettings);
        config = json::parse(f);
        f.close();
    }

    else
    {
        config = Get();
        Save();
    }

    vectorPosition = config["position"].get<std::vector<int>>();
    boolMenu = config["menu"].get<bool>();
    boolSplit = config["split"].get<bool>();
    boolSwapped = config["swapped"].get<bool>();
    boolMaximized = config["maximized"].get<bool>();
    boolFullscreen = config["fullscreen"].get<bool>();
    boolTopmost = config["topmost"].get<bool>();
    stringMain = config["main"].get<std::string>();
    stringMain = config["side"].get<std::string>();
}

void Config::Save()
{
    std::ofstream f(pathSettings);
    f << std::setw(4) << Get() << std::endl;
    f.close();
}

json Config::Get()
{
    json config;
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
