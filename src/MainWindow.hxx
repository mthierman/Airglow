#pragma once

#include "WebView.hxx"
#include "Settings.hxx"
#include "Utility.hxx"

class MainWindow
{
  public:
    static std::unique_ptr<MainWindow> Create(HINSTANCE, int, Settings*);
    // static WebView* pWebView;
    static Settings* pSettings;
    static __int64 __stdcall _WndProc(HWND, UINT, WPARAM, LPARAM);
    static bool Show(HWND, int);
    HWND m_hWnd;

    // MESSAGES
    int _OnCommand();
    int _OnCreate(HWND);
    int _OnClose(HWND);
    int _OnDestroy();
    int _OnDpiChanged();
    int _OnGetMinMaxInfo(LPARAM);
    int _OnPaint(HWND);
    int _OnSize(HWND);
    int _OnSizing(HWND);
    int _OnWindowPosChanged(HWND);
    int _OnSettingChange(HWND);
    int _OnKeyDown(HWND, WPARAM);

    // THEME
    bool CheckSystemDarkMode();
    bool SetDarkTitle();
    bool SetDarkMode(HWND);
    bool SetMica(HWND);

    // WINDOW
    void MaximizeWindow(HWND);
    void FullscreenWindow(HWND);
    void TopmostWindow(HWND);
    void UpdateFocus();

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
    MainWindow(HINSTANCE, int, Settings*);
};
