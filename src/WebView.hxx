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
    void UpdateBounds();
    void UpdateFocus();
    void SetWindowTitle();
    HICON GetFavicon();
    void SetWindowIcon();

  private:
    static Config* pConfig;
    std::pair<wstring, wstring> CommandLine();
    wstring MainNavigation();
    wstring SideNavigation();
    bool VerifySettingsUrl(ICoreWebView2WebMessageReceivedEventArgs* args);
    wstring SettingsNavigation();
    void Messages(ICoreWebView2WebMessageReceivedEventArgs*);
    RECT FullBounds();
    RECT MenuBounds();
    RECT MainBounds();
    RECT SideBounds();
    wstring GetScriptFile();
    wstring GetScript();
    wstring GetMenuScript();

  protected:
    WebView(Config*);
};
