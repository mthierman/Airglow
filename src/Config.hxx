#pragma once

#include "Utility.hxx"

using namespace Utility;

class Config
{
  public:
    static std::unique_ptr<Config> Create();
    void Load();
    void Save();
    void PrintCurrent();
    path dataPath{};
    path configPath{};
    std::vector<int> position{0, 0, 800, 600};
    bool menu{false};
    bool split{false};
    bool swapped{false};
    bool maximized{true};
    bool fullscreen{false};
    bool topmost{false};
    string mainUrl{"https://www.google.com/"};
    string sideUrl{"https://www.google.com/"};

  protected:
    Config();
};
