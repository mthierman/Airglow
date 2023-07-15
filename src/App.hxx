#pragma once

class App
{
  public:
    static std::unique_ptr<App> Create(HINSTANCE, int);
    ~App();

  private:
    App(HINSTANCE, int);
    unsigned long long gdiplusToken{};
    Gdiplus::GdiplusStartupInput gdiplusStartupInput{};
    MSG msg{};
};
