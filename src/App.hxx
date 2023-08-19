#pragma once

#include "Utility.hxx"
#include "Storage.hxx"
#include "WebView.hxx"
#include "include/Definitions.hxx"

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
    void bar_web_message_received(winrt::CoreWebView2WebMessageReceivedEventArgs const&);
    winrt::Windows::Foundation::Rect panel_gui(RECT);
    winrt::Windows::Foundation::Rect panel_bar(RECT);
    winrt::Windows::Foundation::Rect panel_full(RECT);
    winrt::Windows::Foundation::Rect panel_left(RECT);
    winrt::Windows::Foundation::Rect panel_right(RECT);
    winrt::Windows::Foundation::Rect panel_top(RECT);
    winrt::Windows::Foundation::Rect panel_bot(RECT);

    static __int64 __stdcall _WndProc(HWND, UINT, WPARAM, LPARAM);
    int wm_activate(HWND, UINT, WPARAM, LPARAM);
    int wm_close(HWND, UINT, WPARAM, LPARAM);
    int wm_destroy(HWND, UINT, WPARAM, LPARAM);
    int wm_dpichanged(HWND, UINT, WPARAM, LPARAM);
    int wm_erasebkgnd(HWND, UINT, WPARAM, LPARAM);
    int wm_exitsizemove(HWND, UINT, WPARAM, LPARAM);
    int wm_getminmaxinfo(HWND, UINT, WPARAM, LPARAM);
    int wm_keydown(HWND, UINT, WPARAM, LPARAM);
    int wm_navigatemain(HWND, UINT, WPARAM, LPARAM);
    int wm_navigateside(HWND, UINT, WPARAM, LPARAM);
    int wm_notify(HWND, UINT, WPARAM, LPARAM);
    int wm_settingchange(HWND, UINT, WPARAM, LPARAM);
    int wm_windowposchanged(HWND, UINT, WPARAM, LPARAM);

    Storage* storage{nullptr};
    HWND appHwnd{nullptr};

    std::unique_ptr<WebView> webviewGui;
    std::unique_ptr<WebView> webviewBar;
    std::unique_ptr<WebView> webviewMain;
    std::unique_ptr<WebView> webviewSide;
};
