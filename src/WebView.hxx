#pragma once

#include "Config.hxx"

using namespace Microsoft::WRL;

namespace Browsers
{
namespace Main
{
static wil::com_ptr<ICoreWebView2Controller> controller{};
static wil::com_ptr<ICoreWebView2> core{};
static wil::com_ptr<ICoreWebView2_19> browser{};
static wil::com_ptr<ICoreWebView2Settings> settings{};
}; // namespace Main

namespace Side
{
static wil::com_ptr<ICoreWebView2Controller> controller{};
static wil::com_ptr<ICoreWebView2> core{};
static wil::com_ptr<ICoreWebView2_19> browser{};
static wil::com_ptr<ICoreWebView2Settings> settings{};
}; // namespace Side

namespace Settings
{
static wil::com_ptr<ICoreWebView2Controller> controller{};
static wil::com_ptr<ICoreWebView2> core{};
static wil::com_ptr<ICoreWebView2_19> browser{};
static wil::com_ptr<ICoreWebView2Settings> settings{};
}; // namespace Settings
}; // namespace Browsers

class WebView
{
  public:
    static std::unique_ptr<WebView> Create(Config*);
    static wstring MainNavigation();
    static wstring SideNavigation();
    static wstring SettingsNavigation();
    static bool VerifySettingsUrl(ICoreWebView2WebMessageReceivedEventArgs* args);
    static void Messages(ICoreWebView2WebMessageReceivedEventArgs*);
    // static void MainMessages(ICoreWebView2WebMessageReceivedEventArgs*);
    // static void SettingsMessages(ICoreWebView2WebMessageReceivedEventArgs*);
    // static void Messages(wstring);
    static void UpdateBounds();
    static void UpdateFocus();
    static void SetWindowTitle();
    static void SetWindowIcon();
    static void Fullscreen();
    static void Topmost();

  private:
    static Config* pConfig;
    static std::pair<wstring, wstring> CommandLine();
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
