#pragma once

#include "Config.hxx"

class MainWindow
{
  public:
    static std::unique_ptr<MainWindow> Create(HINSTANCE, int, Config*);
    static __int64 __stdcall _WndProc(HWND, UINT, WPARAM, LPARAM);
    static Config* pConfig;
    void Show();
    void Fullscreen();
    void Topmost();
    bool CheckSystemDarkMode();
    bool SetDarkTitle();
    bool SetDarkMode();
    bool SetMica();
    bool Cloak();
    bool Uncloak();

    int _OnActivate(WPARAM);
    int _OnChar(WPARAM);
    int _OnClose();
    int _OnCommand();
    int _OnCreate();
    int _OnDestroy();
    int _OnDpiChanged();
    int _OnEnterSizeMove();
    int _OnExitSizeMove();
    int _OnGetMinMaxInfo(LPARAM);
    int _OnKeyDown(WPARAM);
    int _OnMove();
    int _OnMoving();
    int _OnPaint();
    int _OnSetFocus();
    int _OnSettingChange();
    int _OnSize(WPARAM);
    int _OnSizing();
    int _OnWindowPosChanged();
    int _OnWindowPosChanging();

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
