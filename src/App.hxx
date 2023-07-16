#pragma once

class App
{
  public:
    ~App();
    static std::unique_ptr<App> Create(HINSTANCE, int);

  private:
    App(HINSTANCE, int);
    unsigned long long gdiplusToken{};
    Gdiplus::GdiplusStartupInput gdiplusStartupInput{};
};
