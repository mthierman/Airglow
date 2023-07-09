#include "Config.hxx"

Config::Config() {}

std::unique_ptr<Config> Config::Create()
{
    auto pConfig{std::unique_ptr<Config>(new Config())};
    pConfig->dataPath = DataPath();
    pConfig->configPath = ConfigPath();
    pConfig->dbPath = DbPath();
    pConfig->Load();

    return pConfig;
}

void Config::Load()
{
#ifdef _DEBUG
    println("Config::Load()");
#endif
    if (std::filesystem::exists(configPath) && !std::filesystem::is_empty(configPath))
    {
        try
        {
            json config{};
            ifstream f(configPath);
            config = json::parse(f, nullptr, false, true);
            f.close();

            position = config["position"].get<std::vector<int>>();
            menu = config["menu"].get<bool>();
            split = config["split"].get<bool>();
            swapped = config["swapped"].get<bool>();
            maximized = config["maximized"].get<bool>();
            fullscreen = config["fullscreen"].get<bool>();
            topmost = config["topmost"].get<bool>();
            mainUrl = config["mainUrl"].get<string>();
            sideUrl = config["sideUrl"].get<string>();
        }
        catch (const std::exception& e)
        {
#ifdef _DEBUG
            println(e.what());
#endif
        }
    }
}

void Config::Save()
{
#ifdef _DEBUG
    println("Config::Save()");
#endif
    try
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
        f << std::setw(4) << config << "\n";
        f.close();
    }
    catch (const std::exception& e)
    {
#ifdef _DEBUG
        println(e.what());
#endif
    }
}

void Config::Tests()
{
    println(mainUrl);
    println(sideUrl);
    println(bool_to_string(fullscreen));
    println(bool_to_string(maximized));
    println(bool_to_string(menu));
    println(bool_to_string(split));
    println(bool_to_string(swapped));
    println(bool_to_string(topmost));
    println(std::to_string(position[0]));
    println(std::to_string(position[1]));
    println(std::to_string(position[2]));
    println(std::to_string(position[3]));
}
