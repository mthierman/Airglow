#pragma once

#include "Config.hxx"
#include "MainWindow.hxx"

static wil::com_ptr<ICoreWebView2_19> settings_wv;
static wil::com_ptr<ICoreWebView2Controller> settings_controller;
static wil::com_ptr<ICoreWebView2> settings_core;
static wil::com_ptr<ICoreWebView2Settings> settings_settings;
static wil::com_ptr<ICoreWebView2_19> main_wv;
static wil::com_ptr<ICoreWebView2Controller> main_controller;
static wil::com_ptr<ICoreWebView2> main_core;
static wil::com_ptr<ICoreWebView2Settings> main_settings;
static wil::com_ptr<ICoreWebView2_19> side_wv;
static wil::com_ptr<ICoreWebView2Controller> side_controller;
static wil::com_ptr<ICoreWebView2> side_core;
static wil::com_ptr<ICoreWebView2Settings> side_settings;

class WebView
{
  public:
    static std::unique_ptr<WebView> Create(HWND, Config*);
    static Config* pConfig;
    static void Messages(HWND, wstring);
    static void UpdateBounds(HWND);
    static void UpdateFocus();
    static RECT FullBounds(HWND);
    static RECT MenuBounds(HWND);
    static RECT MainBounds(HWND);
    static RECT SideBounds(HWND);
    static void SetWindowTitle(HWND);
    static void SetWindowIcon(HWND);

  private:
    static wstring GetScriptFile(path);
    static wstring GetScript();
    static wstring GetMenuScript();

  protected:
    WebView(HWND, Config*);
};
