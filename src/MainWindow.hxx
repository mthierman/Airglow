#pragma once

#include "Config.hxx"
#include "WebView.hxx"

class MainWindow
{
  public:
    static std::unique_ptr<MainWindow> Create(HINSTANCE, int, Config*);

  private:
    static __int64 __stdcall _WndProc(HWND, UINT, WPARAM, LPARAM);
    void Show();
    bool CheckSystemDarkMode();
    bool SetDarkTitle();
    bool SetDarkMode();
    bool SetMica();
    bool Cloak();
    bool Uncloak();
    int _OnActivate(WPARAM, LPARAM);
    int _OnChar(WPARAM, LPARAM);
    int _OnClose(WPARAM, LPARAM);
    int _OnCommand(WPARAM, LPARAM);
    int _OnCreate(WPARAM, LPARAM);
    int _OnDestroy(WPARAM, LPARAM);
    int _OnDpiChanged(WPARAM, LPARAM);
    int _OnEnterSizeMove(WPARAM, LPARAM);
    int _OnExitSizeMove(WPARAM, LPARAM);
    int _OnGetMinMaxInfo(WPARAM, LPARAM);
    int _OnKeyDown(WPARAM, LPARAM);
    int _OnMove(WPARAM, LPARAM);
    int _OnMoving(WPARAM, LPARAM);
    int _OnPaint(WPARAM, LPARAM);
    int _OnSetFocus(WPARAM, LPARAM);
    int _OnSettingChange(WPARAM, LPARAM);
    int _OnSize(WPARAM, LPARAM);
    int _OnSizing(WPARAM, LPARAM);
    int _OnWindowPosChanged(WPARAM, LPARAM);
    int _OnWindowPosChanging(WPARAM, LPARAM);
    Config* pConfig{nullptr};
    WebView* pWebView{nullptr};
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
