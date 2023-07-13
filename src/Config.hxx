#pragma once

#include "Utility.hxx"

using namespace util;

class Config
{
  public:
    static std::unique_ptr<Config> Create();
    void Load();
    void Save();

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
        string accentColor{""};
    };
    struct Paths
    {
        path data{};
        path config{};
        path db{};
        path gui{};
        path js{};
    };

    Paths paths{};
    Settings settings{};
    HWND hwnd{};
    HBRUSH hbrBackground{};
    HICON hIcon{};

  private:
    path DataPath();
    path ConfigPath();
    path DbPath();
    path GuiPath();
    path JsPath();
    void Tests();

  protected:
    Config();
};
