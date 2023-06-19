#pragma once

class Settings
{
  public:
    static std::unique_ptr<Settings> Create();
    nlohmann::json Defaults();
    void Load(std::filesystem::path);
    nlohmann::json Current();
    void Save();
    std::filesystem::path pathData;
    std::filesystem::path pathSettings;
    std::vector<int> boolPosition;
    bool boolMenu;
    bool boolSplit;
    bool boolSwapped;
    bool boolMaximized;
    bool boolFullscreen;
    bool boolTopmost;
    std::string stringMain;
    std::string stringSide;

  private:
    std::filesystem::path DataPath();
    std::filesystem::path SettingsPath(std::filesystem::path appData);

  protected:
    Settings();
};
