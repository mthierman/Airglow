#pragma once

using namespace Utility;
using namespace Gdiplus;

class App
{
  public:
    static std::unique_ptr<App> Create(HINSTANCE, int);
    ~App();

  private:
    unsigned long long gdiplusToken{};
    GdiplusStartupInput gdiplusStartupInput{};

    App(HINSTANCE, int);
};
