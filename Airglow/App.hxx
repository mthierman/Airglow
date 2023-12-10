// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#pragma once

#include "Utility.hxx"
#include "Storage.hxx"
#include "WebView.hxx"

class App
{
  public:
    App(HINSTANCE, PWSTR, int);
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

    static __int64 __stdcall WndProc(HWND, UINT, WPARAM, LPARAM);
    int wm_activate(HWND);
    int wm_close(HWND);
    int wm_destroy();
    int wm_devtoolsmain();
    int wm_devtoolsside();
    int wm_dpichanged(HWND, LPARAM);
    int wm_erasebkgnd(HWND);
    int wm_exitsizemove(HWND);
    int wm_getminmaxinfo(LPARAM);
    int wm_homemain();
    int wm_homeside();
    int wm_keydown(HWND, WPARAM);
    int wm_navigatemain();
    int wm_navigateside();
    int wm_notify();
    int wm_settingchange(HWND);
    int wm_windowposchanged(HWND);

    std::unique_ptr<Storage> storage;
    HWND appHwnd;
    std::unique_ptr<WebView> webviewGui;
    std::unique_ptr<WebView> webviewBar;
    std::unique_ptr<WebView> webviewMain;
    std::unique_ptr<WebView> webviewSide;
    float scaledBar;
};
