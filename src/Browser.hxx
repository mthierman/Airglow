#pragma once

#include "Utility.hxx"
#include "Data.hxx"

using namespace Utility;
using namespace Gdiplus;
using namespace Microsoft::WRL;

class Browser
{
  public:
    static std::unique_ptr<Browser> Create(Window&, Settings&, Colors&);
    void Bounds(Window&, Settings&);
    void Focus(Window&, Settings&);
    void Title(Window&, Settings&);
    void Icon(Window&, Settings&);
    void Messages(Window&, Settings&, ICoreWebView2WebMessageReceivedEventArgs* args);
    void Keys(Window&, Settings&, ICoreWebView2AcceleratorKeyPressedEventArgs* args);
    void PostSettings(json);
    void NavigateHome(Settings& settings);

  private:
    Browser(Window&, Settings&, Colors&);
};

namespace wv2main
{
static wil::com_ptr<ICoreWebView2Controller> wvController;
static wil::com_ptr<ICoreWebView2> wvCore;
static wil::com_ptr<ICoreWebView2_19> wvBrowser;
static wil::com_ptr<ICoreWebView2Settings> wvSettings;
} // namespace wv2main

namespace wv2side
{
static wil::com_ptr<ICoreWebView2Controller> wvController;
static wil::com_ptr<ICoreWebView2> wvCore;
static wil::com_ptr<ICoreWebView2_19> wvBrowser;
static wil::com_ptr<ICoreWebView2Settings> wvSettings;
} // namespace wv2side

namespace wv2settings
{
static wil::com_ptr<ICoreWebView2Controller> wvController;
static wil::com_ptr<ICoreWebView2> wvCore;
static wil::com_ptr<ICoreWebView2_19> wvBrowser;
static wil::com_ptr<ICoreWebView2Settings> wvSettings;
} // namespace wv2settings

namespace wv2bar
{
static wil::com_ptr<ICoreWebView2Controller> wvController;
static wil::com_ptr<ICoreWebView2> wvCore;
static wil::com_ptr<ICoreWebView2_19> wvBrowser;
static wil::com_ptr<ICoreWebView2Settings> wvSettings;
} // namespace wv2bar
