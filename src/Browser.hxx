#pragma once

#include "Utility.hxx"

using namespace Utility;
using namespace Gdiplus;
using namespace Microsoft::WRL;

class Browser
{
  public:
    static std::unique_ptr<Browser> Create(HWND);

  private:
    Browser(HWND);
};

namespace Browsers
{
namespace Settings
{
static wil::com_ptr<ICoreWebView2Controller> settings_controller;
static wil::com_ptr<ICoreWebView2> settings_core;
static wil::com_ptr<ICoreWebView2_19> settings_browser;
static wil::com_ptr<ICoreWebView2Settings> settings_settings;
}; // namespace Settings
namespace Main
{
static wil::com_ptr<ICoreWebView2Controller> main_controller;
static wil::com_ptr<ICoreWebView2> main_core;
static wil::com_ptr<ICoreWebView2_19> main_browser;
static wil::com_ptr<ICoreWebView2Settings> main_settings;
}; // namespace Main
namespace Side
{
static wil::com_ptr<ICoreWebView2Controller> side_controller;
static wil::com_ptr<ICoreWebView2> side_core;
static wil::com_ptr<ICoreWebView2_19> side_browser;
static wil::com_ptr<ICoreWebView2Settings> side_settings;
}; // namespace Side
} // namespace Browsers
