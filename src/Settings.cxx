#include "Settings.hxx"

Settings::Settings() {}

std::unique_ptr<Settings> Settings::Create()
{
    auto pSettings = std::unique_ptr<Settings>(new Settings());
    pSettings->pathData = pSettings->DataPath();
    pSettings->pathSettings = pSettings->SettingsPath(pSettings->pathData);
    pSettings->Load(pSettings->pathSettings);

    return pSettings;
}

std::filesystem::path Settings::DataPath()
{
    std::filesystem::path data;
    std::wstring outBuffer;
    PWSTR buffer;

    auto getKnownFolderPath = SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, nullptr, &buffer);

    if (getKnownFolderPath != S_OK)
    {
        CoTaskMemFree(buffer);
        return data;
    }

    outBuffer = buffer;
    data = outBuffer + std::filesystem::path::preferred_separator + L"Airglow";

    CoTaskMemFree(buffer);

    if (!std::filesystem::exists(data))
        std::filesystem::create_directory(data);

    return data;
}

std::filesystem::path Settings::SettingsPath(std::filesystem::path path)
{
    std::filesystem::path settings =
        (path.wstring() + std::filesystem::path::preferred_separator + L"Airglow.json");

    return settings;
}

nlohmann::json Settings::Defaults()
{
    nlohmann::json settings;
    settings["position"] = {0, 0, 800, 600};
    settings["menu"] = false;
    settings["split"] = false;
    settings["swapped"] = false;
    settings["maximized"] = false;
    settings["fullscreen"] = false;
    settings["topmost"] = false;
    settings["main"] = "https://www.google.com/";
    settings["side"] = "https://www.google.com/";

    return settings;
}

void Settings::Load(std::filesystem::path path)
{
    nlohmann::json settings = Defaults();

    if (!std::filesystem::exists(path))
    {
        std::ofstream f(path);
        f << settings.dump(4);
        f.close();
    }

    if (std::filesystem::exists(path))
    {
        std::ifstream f(path);
        if (!std::filesystem::is_empty(path))
            settings = nlohmann::json::parse(f);
        f.close();
    }

    boolPosition = settings["position"].get<std::vector<int>>();
    boolMenu = settings["menu"].get<bool>();
    boolSplit = settings["split"].get<bool>();
    boolSwapped = settings["swapped"].get<bool>();
    boolMaximized = settings["maximized"].get<bool>();
    boolFullscreen = settings["fullscreen"].get<bool>();
    boolTopmost = settings["topmost"].get<bool>();
    stringMain = settings["main"].get<std::string>();
    stringSide = settings["side"].get<std::string>();
}

nlohmann::json Settings::Current()
{
    nlohmann::json settings;
    settings["position"] = boolPosition;
    settings["menu"] = boolMenu;
    settings["split"] = boolSplit;
    settings["swapped"] = boolSwapped;
    settings["maximized"] = boolMaximized;
    settings["fullscreen"] = boolFullscreen;
    settings["topmost"] = boolTopmost;
    settings["main"] = stringMain;
    settings["side"] = stringSide;

    return settings;
}

void Settings::Save()
{
    nlohmann::json settings = Current();

    if (std::filesystem::exists(pathSettings))
    {
        std::ofstream f(pathSettings);
        f << settings.dump(4);
        f.close();
    }
}
