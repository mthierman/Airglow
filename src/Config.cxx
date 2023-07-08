#include "Config.hxx"

Config::Config() {}

std::unique_ptr<Config> Config::Create()
{
    auto pConfig = std::unique_ptr<Config>(new Config());
    pConfig->pathData = DataPath();
    pConfig->pathSettings = SettingsPath(pConfig->pathData);
    pConfig->Load();

    return pConfig;
}

void Config::Load()
{
    json config;

    if (std::filesystem::exists(pathSettings) && !std::filesystem::is_empty(pathSettings))
    {
        ifstream f(pathSettings);
        config = json::parse(f, nullptr, true, true);
        f.close();
    }

    else
    {
        config = Get();
        Save();
    }

    vectorPosition = config["position"].get<position>();
    boolMenu = config["menu"].get<bool>();
    boolSplit = config["split"].get<bool>();
    boolSwapped = config["swapped"].get<bool>();
    boolMaximized = config["maximized"].get<bool>();
    boolFullscreen = config["fullscreen"].get<bool>();
    boolTopmost = config["topmost"].get<bool>();
    stringMain = config["main"].get<string>();
    stringMain = config["side"].get<string>();
}

void Config::Save()
{
    ofstream f(pathSettings);
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
    println(BoolToString(boolFullscreen));
    println(stringMain);
    println(BoolToString(boolMaximized));
    println(BoolToString(boolMenu));
    println(std::to_string(vectorPosition[0]));
    println(std::to_string(vectorPosition[1]));
    println(std::to_string(vectorPosition[2]));
    println(std::to_string(vectorPosition[3]));
    println(stringSide);
    println(BoolToString(boolSplit));
    println(BoolToString(boolSwapped));
    println(BoolToString(boolTopmost));
}
