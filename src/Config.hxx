#pragma once

#include "Utility.hxx"

using namespace util;

struct Paths
{
    path data{};
    path config{};
    path db{};
};

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

class Config
{
  public:
    static std::unique_ptr<Config> Create();
    void Save();

    struct Paths
    {
        path data{};
        path config{};
        path db{};
    };

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

    Config::Paths paths{};
    Config::Settings settings{};
    HWND hwnd{};

  private:
    void Load();
    path DataPath();
    path ConfigPath();
    path DbPath();
    void Tests();

  protected:
    Config();
};
