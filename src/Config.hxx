#pragma once

#include "Utility.hxx"

using namespace Utility;

class Config
{
  public:
    static std::unique_ptr<Config> Create();
    void Load();
    void Save();
    json Get();
    void Tests();

    path pathData;
    path pathSettings;

    position vectorPosition{0, 0, 800, 600};
    bool boolMenu{false};
    bool boolSplit{false};
    bool boolSwapped{false};
    bool boolMaximized{true};
    bool boolFullscreen{false};
    bool boolTopmost{false};
    string stringMain{"https://www.google.com/"};
    string stringSide{"https://www.google.com/"};

  private:
    ofstream streamSettingsPath;

  protected:
    Config();
};
