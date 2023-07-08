#include "Config.hxx"

Config::Config() {}

std::unique_ptr<Config> Config::Create()
{
    auto pConfig{std::unique_ptr<Config>(new Config())};
    pConfig->dataPath = DataPath();
    pConfig->configPath = ConfigPath();
    pConfig->Load();

    return pConfig;
}

void Config::Load()
{
    json config{};

    if (std::filesystem::exists(configPath) && !std::filesystem::is_empty(configPath))
    {
        ifstream f(configPath);
        config = json::parse(f, nullptr, true, true);

        position = config["position"].get<std::vector<int>>();
        menu = config["menu"].get<bool>();
        split = config["split"].get<bool>();
        swapped = config["swapped"].get<bool>();
        maximized = config["maximized"].get<bool>();
        fullscreen = config["fullscreen"].get<bool>();
        topmost = config["topmost"].get<bool>();
        mainUrl = config["mainUrl"].get<string>();
        sideUrl = config["sideUrl"].get<string>();

        f.close();
    }
}

void Config::Save()
{
    json config{};

    config["position"] = position;
    config["menu"] = menu;
    config["split"] = split;
    config["swapped"] = swapped;
    config["maximized"] = maximized;
    config["fullscreen"] = fullscreen;
    config["topmost"] = topmost;
    config["mainUrl"] = mainUrl;
    config["sideUrl"] = sideUrl;

    ofstream f(configPath);
    f << std::setw(4) << config << std::endl;
    f.close();
}

void Config::PrintCurrent()
{
    println(mainUrl);
    println(sideUrl);
    println(BoolToString(fullscreen));
    println(BoolToString(maximized));
    println(BoolToString(menu));
    println(BoolToString(split));
    println(BoolToString(swapped));
    println(BoolToString(topmost));
    println(std::to_string(position[0]));
    println(std::to_string(position[1]));
    println(std::to_string(position[2]));
    println(std::to_string(position[3]));
}
