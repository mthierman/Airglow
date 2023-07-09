#pragma once

#include "Utility.hxx"

using namespace util;

class Config
{
  public:
    static std::unique_ptr<Config> Create();
    void Save();
    void Load();
    path DataPath();
    path ConfigPath();
    path DbPath();

    path dataPath{};
    path configPath{};
    path dbPath{};

    std::vector<int> position{0, 0, 800, 600};
    bool menu{false};
    bool split{false};
    bool swapped{false};
    bool maximized{true};
    bool fullscreen{false};
    bool topmost{false};
    string mainUrl{"https://www.google.com/"};
    string sideUrl{"https://www.google.com/"};

    HWND hwnd{};

  protected:
    Config();
    void Tests();
};
