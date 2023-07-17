#pragma once

#include "Utility.hxx"

using namespace Utility;
using namespace Gdiplus;
using namespace Microsoft::WRL;

class Browser
{
  public:
    static std::unique_ptr<Browser> Create(HWND);
    void Bounds(State::Window);
    void Focus();
    void Navigate();

  private:
    Browser(HWND);
    HWND hwnd;
};

namespace WebView
{
namespace Controller
{
static wil::com_ptr<ICoreWebView2Controller> main;
static wil::com_ptr<ICoreWebView2Controller> side;
static wil::com_ptr<ICoreWebView2Controller> settings;
} // namespace Controller
namespace Core
{
static wil::com_ptr<ICoreWebView2> main;
static wil::com_ptr<ICoreWebView2> side;
static wil::com_ptr<ICoreWebView2> settings;
} // namespace Core
namespace Core19
{
static wil::com_ptr<ICoreWebView2_19> main;
static wil::com_ptr<ICoreWebView2_19> side;
static wil::com_ptr<ICoreWebView2_19> settings;
} // namespace Core19
namespace Settings
{
static wil::com_ptr<ICoreWebView2Settings> main;
static wil::com_ptr<ICoreWebView2Settings> side;
static wil::com_ptr<ICoreWebView2Settings> settings;
} // namespace Settings
} // namespace WebView
