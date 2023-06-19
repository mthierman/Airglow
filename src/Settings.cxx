#include "Settings.hxx"

Settings::Settings(){};

std::unique_ptr<Settings> Settings::Create() { return std::unique_ptr<Settings>(new Settings()); };

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
    auto appData = this->GetAppDataPath();
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

    settings["dimensions"] = this->dimensions;
    settings["topmost"] = this->topmost;
    settings["maximized"] = this->maximized;
    settings["fullscreen"] = this->fullscreen;
    settings["split"] = this->split;
    settings["swapped"] = this->swapped;
    settings["menu"] = this->menu;
    settings["mainpage"] = this->mainpage;
    settings["sidepage"] = this->sidepage;

    return settings;
}

nlohmann::json Settings::LoadSettings()
{
    auto file = this->GetSettingsFilePath();
    nlohmann::json settings = this->DefaultSettings();

    if (std::filesystem::exists(file))
    {
        std::ifstream f(file);
        if (!std::filesystem::is_empty(file))
            settings = nlohmann::json::parse(f);
        f.close();
    }

    this->dimensions = settings["dimensions"].get<std::vector<int>>();
    this->fullscreen = settings["fullscreen"].get<bool>();
    this->maximized = settings["maximized"].get<bool>();
    this->menu = settings["menu"].get<bool>();
    this->topmost = settings["topmost"].get<bool>();
    this->split = settings["split"].get<bool>();
    this->swapped = settings["swapped"].get<bool>();
    this->mainpage = settings["mainpage"].get<std::string>();
    this->sidepage = settings["sidepage"].get<std::string>();

    return settings;
}

void Settings::SaveSettings()
{
    auto file = this->GetSettingsFilePath();
    nlohmann::json settings = this->CurrentSettings();

    if (std::filesystem::exists(file))
    {
        std::ofstream f(file);
        f << settings.dump(4);
        f.close();
    }
}
