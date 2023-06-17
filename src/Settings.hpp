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
    std::filesystem::path file =
        (appData.wstring() + std::filesystem::path::preferred_separator + L"Airglow.json");

    if (!std::filesystem::exists(file))
    {
        nlohmann::json defaultSettings = DefaultSettings();
        std::ofstream f(file);
        f << defaultSettings.dump(4);
        f.close();
    }

    return file;
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
    settings["mainpage"] = "https://wwww.google.com/";
    settings["sidepage"] = "https://wwww.google.com/";

    return settings;
}

nlohmann::json CurrentSettings(HWND window)
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

nlohmann::json LoadSettings(std::filesystem::path settingsFile)
{
    nlohmann::json saved = DefaultSettings();

    if (std::filesystem::exists(settingsFile))
    {
        std::ifstream f(settingsFile);
        if (!std::filesystem::is_empty(settingsFile))
            saved = nlohmann::json::parse(f);
        f.close();
    }

    return saved;
}

void SaveSettings(nlohmann::json input, std::filesystem::path settingsFile)
{
    if (std::filesystem::exists(settingsFile))
    {
        std::ofstream f(settingsFile);
        f << input.dump(4);
        f.close();
    }
}
