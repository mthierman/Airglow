#pragma once

#include "Utility.hxx"

class Config
{
  public:
    static std::unique_ptr<Config> Create();
    tao::json::value Defaults();
    tao::json::value Current();
    void Load();
    void Save();
    void Tests();
    std::filesystem::path pathData;
    std::filesystem::path pathSettings;
    std::vector<int> vectorPosition;
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
    Config();
};
