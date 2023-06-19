#pragma once

class Settings
{
  public:
    Settings();
    std::filesystem::path GetAppDataPath();
    std::filesystem::path GetSettingsFilePath();
    nlohmann::json DefaultSettings();
    nlohmann::json CurrentSettings();
    void LoadSettings();
    void SaveSettings();
    std::filesystem::path appData;
    std::filesystem::path settingsFile;
    std::vector<int> dimensions;
    bool menu;
    bool split;
    bool swapped;
    bool maximized;
    bool fullscreen;
    bool topmost;
    std::string mainpage;
    std::string sidepage;
};
