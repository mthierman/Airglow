#pragma once

#include "Utility.hxx"

using namespace Utility;

class Settings
{
  public:
    struct Configs
    {
        string mainUrl{"google.com"};
        string sideUrl{"google.com"};
        bool menu{false};
        bool split{false};
        bool swapped{false};
    };

    struct Paths
    {
        path data{};
        path settings{};
        path config{};
        path db{};
        path js{};
    };

    struct Colors
    {
        string accent{};
        string accentDark1{};
        string accentDark2{};
        string accentDark3{};
        string accentLight1{};
        string accentLight2{};
        string accentLight3{};
        string Background{};
        string Foreground{};
    };

    static std::unique_ptr<Config> Create();
    void Load();
    void Save();
    Paths GetPaths();
    Colors GetSystemColors();

  private:
    bool Initialize();
    path LocalAppDataPath();
    path PortableAppDataPath();
    path SettingsPath();
    path ConfigPath();
    path DbPath();
    path JsPath();
    void CreateDb();

  protected:
    Config();
};
