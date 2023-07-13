#pragma once

#include "Utility.hxx"

using namespace util;

class Config
{
  public:
    static std::unique_ptr<Config> Create();
    void Load();
    void Save();
    path PortableAppDataPath();

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
        string accentColor{"#FFFFFFFF"};
    };
    struct Paths
    {
        path data{};
        path config{};
        path db{};
        path js{};
    };

    Paths paths{};
    Settings settings{};
    HWND hwnd{};
    HBRUSH hbrBackground{};
    HICON hIcon{};

  private:
    path LocalAppDataPath();
    path ConfigPath();
    path DbPath();
    path JsPath();
    void Tests();

  protected:
    Config();
};
