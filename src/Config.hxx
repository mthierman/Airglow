#pragma once

#include "Utility.hxx"

using namespace Utility;

class Config
{
  public:
    static std::unique_ptr<Config> Create();
    void InitializeColors();
    void Load();
    void Save();

    struct App
    {
        string name{};
        string version{};
    };

    struct Paths
    {
        path data{};
        path settings{};
        path config{};
        path db{};
        path js{};
    };

    struct Window
    {
        HWND hwnd{};
        HBRUSH hbrBackground{};
        HICON hIcon{};
    };

    struct Colors
    {
        std::string accent{};
        std::string accentDark1{};
        std::string accentDark2{};
        std::string accentDark3{};
        std::string accentLight1{};
        std::string accentLight2{};
        std::string accentLight3{};
        std::string Background{};
        std::string Foreground{};
    };

    struct Settings
    {
        std::vector<int> position{0, 0, 0, 0};
        bool menu{false};
        bool split{false};
        bool swapped{false};
        bool maximized{false};
        bool fullscreen{false};
        bool topmost{false};
        string mainUrl{"google.com"};
        string sideUrl{"google.com"};
    };

    App app{};
    Paths paths{};
    Window window{};
    Colors colors{};
    Settings settings{};

  private:
    void Initialize();
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
