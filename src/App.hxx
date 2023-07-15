#pragma once

using namespace Gdiplus;

class App
{
  public:
    static std::unique_ptr<App> Create(HINSTANCE, int);
    void Close();

  private:
    unsigned long long gdiplusToken{};
    GdiplusStartupInput gdiplusStartupInput{};

  protected:
    App(HINSTANCE, int);
};
