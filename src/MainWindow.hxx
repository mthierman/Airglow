#pragma once

#include "Config.hxx"
#include "WebView.hxx"
#include "Utility.hxx"

class MainWindow
{
  public:
    static std::unique_ptr<MainWindow> Create(HINSTANCE, int, Config*);
    static Config* pConfig;
    static void Show(HWND, int);
    static void Fullscreen(HWND);
    static void Topmost(HWND);
    static __int64 __stdcall _WndProc(HWND, UINT, WPARAM, LPARAM);
    HWND m_hWnd;

    int _OnCommand();
    int _OnCreate(HWND);
    int _OnActivate(HWND, WPARAM);
    int _OnClose(HWND);
    int _OnDestroy();
    int _OnDpiChanged();
    int _OnGetMinMaxInfo(HWND, LPARAM);
    int _OnPaint(HWND);
    int _OnSize(HWND, WPARAM);
    int _OnSizing(HWND);
    int _OnEnterSizeMove(HWND);
    int _OnExitSizeMove(HWND);
    int _OnMove(HWND);
    int _OnMoving(HWND);
    // int _OnWindowPosChanging(HWND);
    // int _OnWindowPosChanged(HWND);
    int _OnSetFocus(HWND);
    int _OnSettingChange(HWND);
    int _OnKeyDown(HWND, WPARAM);
    int _OnChar(HWND, WPARAM);

    bool CheckSystemDarkMode();
    bool SetDarkTitle();
    bool SetDarkMode(HWND);
    bool SetMica(HWND);

  private:
    static unsigned long long gdiplusToken;
    static Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    enum PreferredAppMode
    {
        Default,
        AllowDark,
        ForceDark,
        ForceLight,
        Max
    };

  protected:
    MainWindow(HINSTANCE, int, Config*);
};
