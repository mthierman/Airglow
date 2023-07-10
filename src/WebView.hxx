#pragma once

#include "Config.hxx"

static wil::com_ptr<ICoreWebView2Controller> settings_controller{};
static wil::com_ptr<ICoreWebView2> settings_core{};
static wil::com_ptr<ICoreWebView2_19> settings_wv{};
static wil::com_ptr<ICoreWebView2Settings> settings_settings{};

static wil::com_ptr<ICoreWebView2Controller> main_controller{};
static wil::com_ptr<ICoreWebView2> main_core{};
static wil::com_ptr<ICoreWebView2_19> main_wv{};
static wil::com_ptr<ICoreWebView2Settings> main_settings{};

static wil::com_ptr<ICoreWebView2Controller> side_controller{};
static wil::com_ptr<ICoreWebView2> side_core{};
static wil::com_ptr<ICoreWebView2_19> side_wv{};
static wil::com_ptr<ICoreWebView2Settings> side_settings{};

class WebView
{
  public:
    static std::unique_ptr<WebView> Create(Config*);
    static void UpdateBounds();
    static void UpdateFocus();
    static void SetWindowTitle();
    static void SetWindowIcon();
    static void Fullscreen();
    static void Topmost();

  private:
    static Config* pConfig;
    static std::pair<wstring, wstring> CommandLine();
    static void Messages(wstring);
    static RECT FullBounds();
    static RECT MenuBounds();
    static RECT MainBounds();
    static RECT SideBounds();
    static wstring GetScriptFile();
    static wstring GetScript();
    static wstring GetMenuScript();

  protected:
    WebView(Config*);
};
