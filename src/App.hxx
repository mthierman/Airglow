#pragma once

#include "Utility.hxx"
#include "Data.hxx"
#include "Browser.hxx"

using namespace Utility;
using namespace Gdiplus;
using namespace Microsoft::WRL;

class App
{
  public:
    static std::unique_ptr<App> Create(HINSTANCE, int);
    ~App();

  private:
    App(HINSTANCE, int);

    std::unique_ptr<Browser> browser{nullptr};
    unsigned long long gdiplusToken{};
    GdiplusStartupInput gdiplusStartupInput{};
    Window window{};
    Settings settings{};
    Paths paths{};
    Colors colors{};

    void Show();
    json LoadJson();
    void SaveJson(json);
    void LoadSettings();
    void SaveSettings();

    // void App::CreateDb();

    static __int64 __stdcall _WndProc(HWND, UINT, WPARAM, LPARAM);
    int _OnActivate(HWND, WPARAM, LPARAM);
    int _OnClose(HWND, WPARAM, LPARAM);
    int _OnDestroy(HWND, WPARAM, LPARAM);
    int _OnEraseBackground(HWND, WPARAM, LPARAM);
    int _OnExitSizeMove(HWND, WPARAM, LPARAM);
    int _OnGetMinMaxInfo(HWND, WPARAM, LPARAM);
    int _OnKeyDown(HWND, WPARAM, LPARAM);
    int _OnNotify(HWND, WPARAM, LPARAM);
    int _OnSetFocus(HWND, WPARAM, LPARAM);
    int _OnSettingChange(HWND, WPARAM, LPARAM);
    int _OnWindowPosChanged(HWND, WPARAM, LPARAM);
};
