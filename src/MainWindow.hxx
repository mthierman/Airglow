#pragma once

#include "Config.hxx"

class MainWindow
{
  public:
    static std::unique_ptr<MainWindow> Create(HINSTANCE, int, Config*);
    static Config* pConfig;
    void Show();
    static void Fullscreen(HWND);
    static void Topmost(HWND);
    bool CheckSystemDarkMode();
    bool SetDarkTitle();
    bool SetDarkMode();
    bool SetMica();
    bool Cloak();
    bool Uncloak();
    static __int64 __stdcall _WndProc(HWND, UINT, WPARAM, LPARAM);

    int _OnActivate(HWND, WPARAM);
    int _OnChar(HWND, WPARAM);
    int _OnClose(HWND);
    int _OnCommand();
    int _OnCreate(HWND);
    int _OnDestroy();
    int _OnDpiChanged();
    int _OnEnterSizeMove(HWND);
    int _OnExitSizeMove(HWND);
    int _OnGetMinMaxInfo(HWND, LPARAM);
    int _OnKeyDown(HWND, WPARAM);
    int _OnMove(HWND);
    int _OnMoving(HWND);
    int _OnPaint(HWND);
    int _OnSetFocus(HWND);
    int _OnSettingChange(HWND);
    int _OnSize(HWND, WPARAM);
    int _OnSizing(HWND);
    int _OnWindowPosChanged(HWND);
    int _OnWindowPosChanging(HWND);

    HWND hwnd;

  private:
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
