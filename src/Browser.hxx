#pragma once

#include "Utility.hxx"
#include "Data.hxx"

using namespace Utility;
using namespace Gdiplus;
using namespace Microsoft::WRL;

struct WebView2
{
    winrt::com_ptr<ICoreWebView2Controller> wvCoreController;
    winrt::com_ptr<ICoreWebView2Controller4> wvController;
    winrt::com_ptr<ICoreWebView2> wvCore;
    winrt::com_ptr<ICoreWebView2_19> wvBrowser;
    winrt::com_ptr<ICoreWebView2Settings> wvSettings;
};

class Browser
{
  public:
    static std::unique_ptr<Browser> Create(Window&, Settings&, Colors&);
    void Bounds(Window&, Settings&);
    void Focus(Window&, Settings&);
    void Title(Window&, Settings&);
    void Icon(Window&, Settings&);
    void SettingsMessages(Window&, Settings&, ICoreWebView2WebMessageReceivedEventArgs* args);
    void BarMessages(Window&, Settings&, ICoreWebView2WebMessageReceivedEventArgs* args);
    void Keys(Window&, Settings&, ICoreWebView2AcceleratorKeyPressedEventArgs* args);
    void PostSettings(json);
    void NavigateHome(Settings& settings);
    void NavigateMain(Settings& settings);
    void NavigateSide(Settings& settings);
    void FocusBar();
    void FocusSettings();
    void EscapeFocusBar(Settings& settings);

  private:
    Browser(Window&, Settings&, Colors&);
};

namespace wv2main
{
static winrt::com_ptr<ICoreWebView2Controller> wvCoreController;
static winrt::com_ptr<ICoreWebView2Controller4> wvController;
static winrt::com_ptr<ICoreWebView2> wvCore;
static winrt::com_ptr<ICoreWebView2_19> wvBrowser;
static winrt::com_ptr<ICoreWebView2Settings> wvSettings;
} // namespace wv2main

namespace wv2side
{
static winrt::com_ptr<ICoreWebView2Controller> wvCoreController;
static winrt::com_ptr<ICoreWebView2Controller4> wvController;
static winrt::com_ptr<ICoreWebView2> wvCore;
static winrt::com_ptr<ICoreWebView2_19> wvBrowser;
static winrt::com_ptr<ICoreWebView2Settings> wvSettings;
} // namespace wv2side

namespace wv2settings
{
static winrt::com_ptr<ICoreWebView2Controller> wvCoreController;
static winrt::com_ptr<ICoreWebView2Controller4> wvController;
static winrt::com_ptr<ICoreWebView2> wvCore;
static winrt::com_ptr<ICoreWebView2_19> wvBrowser;
static winrt::com_ptr<ICoreWebView2Settings> wvSettings;
} // namespace wv2settings

namespace wv2bar
{
static winrt::com_ptr<ICoreWebView2Controller> wvCoreController;
static winrt::com_ptr<ICoreWebView2Controller4> wvController;
static winrt::com_ptr<ICoreWebView2> wvCore;
static winrt::com_ptr<ICoreWebView2_19> wvBrowser;
static winrt::com_ptr<ICoreWebView2Settings> wvSettings;
} // namespace wv2bar
