#include "Settings.hxx"

Settings::Settings()
{
    appData = GetAppDataPath();
    settingsFile = GetSettingsFilePath();
}

std::filesystem::path Settings::GetAppDataPath()
{
    std::filesystem::path path;
    std::wstring outBuffer;
    PWSTR buffer;

    auto getKnownFolderPath = SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, nullptr, &buffer);

    if (getKnownFolderPath != S_OK)
    {
        CoTaskMemFree(buffer);
        return path;
    }

    outBuffer = buffer;
    path = outBuffer + std::filesystem::path::preferred_separator + L"Airglow";

    CoTaskMemFree(buffer);

    if (!std::filesystem::exists(path))
        std::filesystem::create_directory(path);

    return path;
}

std::filesystem::path Settings::GetSettingsFilePath()
{
    auto appData = GetAppDataPath();
    std::filesystem::path path =
        (appData.wstring() + std::filesystem::path::preferred_separator + L"Airglow.json");

    if (!std::filesystem::exists(path))
    {
        nlohmann::json defaultSettings = DefaultSettings();
        std::ofstream f(path);
        f << defaultSettings.dump(4);
        f.close();
    }

    return path;
}

nlohmann::json Settings::DefaultSettings()
{
    nlohmann::json settings;

    settings["dimensions"] = {0, 0, 800, 600};
    settings["topmost"] = false;
    settings["maximized"] = false;
    settings["fullscreen"] = false;
    settings["split"] = false;
    settings["swapped"] = false;
    settings["menu"] = false;
    settings["mainpage"] = "https://www.google.com/";
    settings["sidepage"] = "https://www.google.com/";

    return settings;
}

nlohmann::json Settings::CurrentSettings()
{
    nlohmann::json settings;

    settings["dimensions"] = dimensions;
    settings["topmost"] = topmost;
    settings["maximized"] = maximized;
    settings["fullscreen"] = fullscreen;
    settings["split"] = split;
    settings["swapped"] = swapped;
    settings["menu"] = menu;
    settings["mainpage"] = mainpage;
    settings["sidepage"] = sidepage;

    return settings;
}

void Settings::LoadSettings()
{
    nlohmann::json settings = DefaultSettings();

    if (std::filesystem::exists(settingsFile))
    {
        std::ifstream f(settingsFile);
        if (!std::filesystem::is_empty(settingsFile))
            settings = nlohmann::json::parse(f);
        f.close();
    }

    dimensions = settings["dimensions"].get<std::vector<int>>();
    fullscreen = settings["fullscreen"].get<bool>();
    maximized = settings["maximized"].get<bool>();
    menu = settings["menu"].get<bool>();
    topmost = settings["topmost"].get<bool>();
    split = settings["split"].get<bool>();
    swapped = settings["swapped"].get<bool>();
    mainpage = settings["mainpage"].get<std::string>();
    sidepage = settings["sidepage"].get<std::string>();
}

void Settings::SaveSettings()
{
    nlohmann::json settings = CurrentSettings();

    if (std::filesystem::exists(settingsFile))
    {
        std::ofstream f(settingsFile);
        f << settings.dump(4);
        f.close();
    }
}
