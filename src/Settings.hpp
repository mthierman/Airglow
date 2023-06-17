std::filesystem::path GetAppDataPath()
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

std::filesystem::path GetSettingsFilePath(std::filesystem::path appData)
{
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

nlohmann::json DefaultSettings()
{
    nlohmann::json settings;
    settings["dimensions"] = {0, 0, 800, 600};
    settings["ontop"] = false;
    settings["maximized"] = false;
    settings["fullscreen"] = false;
    settings["split"] = false;
    settings["swapped"] = false;
    settings["menu"] = false;
    settings["mainpage"] = "https://www.google.com/";
    settings["sidepage"] = "https://www.google.com/";

    return settings;
}

nlohmann::json CurrentSettings()
{
    nlohmann::json settings;
    settings["dimensions"] = dimensions;
    settings["ontop"] = ontop;
    settings["maximized"] = maximized;
    settings["fullscreen"] = fullscreen;
    settings["split"] = split;
    settings["swapped"] = swapped;
    settings["menu"] = menu;
    settings["mainpage"] = ToString(mainpage);
    settings["sidepage"] = ToString(sidepage);

    return settings;
}

nlohmann::json LoadSettings()
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
    ontop = settings["ontop"].get<bool>();
    split = settings["split"].get<bool>();
    swapped = settings["swapped"].get<bool>();
    mainpage = ToWide(settings["mainpage"].get<std::string>());
    sidepage = ToWide(settings["sidepage"].get<std::string>());

    return settings;
}

void SaveSettings()
{
    nlohmann::json settings = CurrentSettings();

    if (std::filesystem::exists(settingsFile))
    {
        std::ofstream f(settingsFile);
        f << settings.dump(4);
        f.close();
    }
}
