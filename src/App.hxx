#pragma once

#include "Utility.hxx"
#include "Storage.hxx"
#include "WebView.hxx"

class App
{
  public:
    App(Storage*, HINSTANCE, PWSTR, int);
    ~App();

    HWND get_hwnd();

  private:
    HWND create_window(HINSTANCE);
    void show_window();
    void resized();

    static __int64 __stdcall _WndProc(HWND, UINT, WPARAM, LPARAM);
    int wm_activate(HWND, UINT, WPARAM, LPARAM);
    int wm_close(HWND, UINT, WPARAM, LPARAM);
    int wm_destroy(HWND, UINT, WPARAM, LPARAM);
    int wm_dpichanged(HWND, UINT, WPARAM, LPARAM);
    int wm_erasebkgnd(HWND, UINT, WPARAM, LPARAM);
    int wm_exitsizemove(HWND, UINT, WPARAM, LPARAM);
    int wm_getminmaxinfo(HWND, UINT, WPARAM, LPARAM);
    int wm_keydown(HWND, UINT, WPARAM, LPARAM);
    int wm_notify(HWND, UINT, WPARAM, LPARAM);
    int wm_setfocus(HWND, UINT, WPARAM, LPARAM);
    int wm_settingchange(HWND, UINT, WPARAM, LPARAM);
    int wm_windowposchanged(HWND, UINT, WPARAM, LPARAM);

    Storage* storage{nullptr};
    HWND appHwnd{nullptr};

    std::unique_ptr<WebView> webviewGui;
    std::unique_ptr<WebView> webviewBar;
    std::unique_ptr<WebView> webviewMain;
    std::unique_ptr<WebView> webviewSide;
};
