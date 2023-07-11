#pragma once

#include "Config.hxx"
#include "WebView.hxx"

class MainWindow
{
  public:
    static std::unique_ptr<MainWindow> Create(HINSTANCE, int, Config*);
    HBRUSH hbrBackground{};
    HWND hwnd{};
    HICON hIcon{};

  private:
    static Config* pConfig;
    static WebView* pWebView;
    static __int64 __stdcall _WndProc(HWND, UINT, WPARAM, LPARAM);
    void Show();
    void Fullscreen();
    void Topmost();
    void Maximize();
    bool CheckSystemDarkMode();
    bool SetDarkTitle();
    bool SetDarkMode();
    bool SetMica();
    bool Cloak();
    bool Uncloak();
    int _OnActivate(HWND, WPARAM, LPARAM);
    int _OnChar(HWND, WPARAM, LPARAM);
    int _OnClose(HWND, WPARAM, LPARAM);
    int _OnCommand(HWND, WPARAM, LPARAM);
    int _OnCreate(HWND, WPARAM, LPARAM);
    int _OnDestroy(HWND, WPARAM, LPARAM);
    int _OnDpiChanged(HWND, WPARAM, LPARAM);
    int _OnEnterSizeMove(HWND, WPARAM, LPARAM);
    int _OnExitSizeMove(HWND, WPARAM, LPARAM);
    int _OnGetMinMaxInfo(HWND, WPARAM, LPARAM);
    int _OnKeyDown(HWND, WPARAM, LPARAM);
    int _OnMove(HWND, WPARAM, LPARAM);
    int _OnMoving(HWND, WPARAM, LPARAM);
    int _OnPaint(HWND, WPARAM, LPARAM);
    int _OnSetIcon(HWND, WPARAM, LPARAM);
    int _OnSetFocus(HWND, WPARAM, LPARAM);
    int _OnSettingChange(HWND, WPARAM, LPARAM);
    int _OnSize(HWND, WPARAM, LPARAM);
    int _OnSizing(HWND, WPARAM, LPARAM);
    int _OnWindowPosChanged(HWND, WPARAM, LPARAM);
    int _OnWindowPosChanging(HWND, WPARAM, LPARAM);

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
