#pragma once

#include "Utility.hxx"

using namespace util;

class Config
{
  public:
    static std::unique_ptr<Config> Create();
    void Save();

    struct Settings
    {
        std::vector<int> position{0, 0, 800, 600};
        bool menu{false};
        bool split{false};
        bool swapped{false};
        bool maximized{true};
        bool fullscreen{false};
        bool topmost{false};
        string mainUrl{"https://www.google.com/"};
        string sideUrl{"https://www.google.com/"};
    };
    struct Paths
    {
        path data{};
        path config{};
        path db{};
        path gui{};
    };

    Paths paths{};
    Settings settings{};
    HWND hwnd{};
    HBRUSH hbrBackground{};
    HICON hIcon{};

  private:
    void Load();
    path DataPath();
    path ConfigPath();
    path DbPath();
    path GuiPath();
    void Tests();

  protected:
    Config();
};
