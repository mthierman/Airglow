#pragma once

#include "Utility.hxx"

class Config
{
  public:
    static std::unique_ptr<Config> Create();
    void Load();
    void Save();
    tao::json::value Get();
    void Tests();

    tao::json::value config;
    std::filesystem::path pathData;
    std::filesystem::path pathSettings;

    std::vector<int> vectorPosition{0, 0, 800, 600};
    bool boolMenu{false};
    bool boolSplit{false};
    bool boolSwapped{false};
    bool boolMaximized{true};
    bool boolFullscreen{false};
    bool boolTopmost{false};
    std::string stringMain{"https://www.google.com/"};
    std::string stringSide{"https://www.google.com/"};

  private:
    std::filesystem::path DataPath();
    std::filesystem::path SettingsPath(std::filesystem::path appData);
    std::ofstream streamSettingsPath;

  protected:
    Config();
};
