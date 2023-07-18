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
    json SerializeJson(Settings);
    Settings DeserializeJson(json);
    json LoadJson();
    void SaveJson(json);
    void LoadSettings();
    void SaveSettings();
    Colors SystemColors();

    static __int64 __stdcall _WndProc(HWND, UINT, WPARAM, LPARAM);
    int _OnActivate(HWND, WPARAM, LPARAM);
    int _OnChar(HWND, WPARAM, LPARAM);
    int _OnClose(HWND, WPARAM, LPARAM);
    int _OnCommand(HWND, WPARAM, LPARAM);
    int _OnCreate(HWND, WPARAM, LPARAM);
    int _OnDestroy(HWND, WPARAM, LPARAM);
    int _OnDpiChanged(HWND, WPARAM, LPARAM);
    int _OnEnterSizeMove(HWND, WPARAM, LPARAM);
    int _OnEraseBackground(HWND, WPARAM, LPARAM);
    int _OnExitSizeMove(HWND, WPARAM, LPARAM);
    int _OnGetMinMaxInfo(HWND, WPARAM, LPARAM);
    int _OnKeyDown(HWND, WPARAM, LPARAM);
    int _OnMouseHover(HWND, WPARAM, LPARAM);
    int _OnMouseLeave(HWND, WPARAM, LPARAM);
    int _OnMouseMove(HWND, WPARAM, LPARAM);
    int _OnMove(HWND, WPARAM, LPARAM);
    int _OnMoving(HWND, WPARAM, LPARAM);
    int _OnPaint(HWND, WPARAM, LPARAM);
    int _OnSetIcon(HWND, WPARAM, LPARAM);
    int _OnSetFocus(HWND, WPARAM, LPARAM);
    int _OnSetText(HWND, WPARAM, LPARAM);
    int _OnSettingChange(HWND, WPARAM, LPARAM);
    int _OnSize(HWND, WPARAM, LPARAM);
    int _OnSizing(HWND, WPARAM, LPARAM);
    int _OnWindowPosChanged(HWND, WPARAM, LPARAM);
    int _OnWindowPosChanging(HWND, WPARAM, LPARAM);

    void Debug();
};
