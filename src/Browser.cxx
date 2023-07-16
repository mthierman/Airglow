#include "Browser.hxx"
#include "Utility.hxx"

using namespace Utility;
using namespace Gdiplus;
using namespace Microsoft::WRL;

wil::com_ptr<ICoreWebView2Environment> Browser::browser_environment{nullptr};

wil::com_ptr<ICoreWebView2Controller> Browser::settings_controller{nullptr};
wil::com_ptr<ICoreWebView2> Browser::settings_core{nullptr};
wil::com_ptr<ICoreWebView2_19> Browser::settings_browser{nullptr};
wil::com_ptr<ICoreWebView2Settings> Browser::settings_settings{nullptr};

wil::com_ptr<ICoreWebView2Controller> Browser::main_controller{nullptr};
wil::com_ptr<ICoreWebView2> Browser::main_core{nullptr};
wil::com_ptr<ICoreWebView2_19> Browser::main_browser{nullptr};
wil::com_ptr<ICoreWebView2Settings> Browser::main_settings{nullptr};

wil::com_ptr<ICoreWebView2Controller> Browser::side_controller{nullptr};
wil::com_ptr<ICoreWebView2> Browser::side_core{nullptr};
wil::com_ptr<ICoreWebView2_19> Browser::side_browser{nullptr};
wil::com_ptr<ICoreWebView2Settings> Browser::side_settings{nullptr};

Browser::Browser(HWND hwnd) {}

std::unique_ptr<Browser> Browser::Create(HWND hwnd)
{
    auto browser{std::unique_ptr<Browser>(new Browser(hwnd))};

    if (FAILED(CreateCoreWebView2EnvironmentWithOptions(
            nullptr, path_portable().wstring().c_str(), nullptr,
            Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
                [](HRESULT hr, ICoreWebView2Environment* environment) -> HRESULT
                {
                    if (environment)
                        browser_environment = environment;

                    return S_OK;
                })
                .Get())))
    {
        return nullptr;
    };

    if (FAILED(browser_environment->CreateCoreWebView2Controller(
            hwnd, Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                      [hwnd](HRESULT hr, ICoreWebView2Controller* controller) -> HRESULT
                      {
                          if (controller)
                          {
                              settings_controller = controller;
                              settings_controller->get_CoreWebView2(&settings_core);
                          }

                          if (settings_core)
                              settings_browser = settings_core.try_query<ICoreWebView2_19>();

                          if (settings_browser)
                          {
                              settings_controller->put_Bounds(get_rect(hwnd));
                              settings_browser->Navigate(L"https://wwww.google.com/");
                          }

                          return S_OK;
                      })
                      .Get())))
    {
        return nullptr;
    };

    if (FAILED(browser_environment->CreateCoreWebView2Controller(
            hwnd, Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                      [hwnd](HRESULT hr, ICoreWebView2Controller* controller) -> HRESULT
                      {
                          if (controller)
                          {
                              main_controller = controller;
                              main_controller->get_CoreWebView2(&main_core);
                          }

                          if (main_core)
                              main_browser = main_core.try_query<ICoreWebView2_19>();

                          if (main_browser)
                          {
                              main_controller->put_Bounds(get_rect(hwnd));
                              main_browser->Navigate(L"https://wwww.google.com/");
                          }

                          return S_OK;
                      })
                      .Get())))
    {
        return nullptr;
    };

    if (FAILED(browser_environment->CreateCoreWebView2Controller(
            hwnd, Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                      [hwnd](HRESULT hr, ICoreWebView2Controller* controller) -> HRESULT
                      {
                          if (controller)
                          {
                              side_controller = controller;
                              side_controller->get_CoreWebView2(&side_core);
                          }

                          if (side_core)
                              side_browser = side_core.try_query<ICoreWebView2_19>();

                          if (side_browser)
                          {
                              side_controller->put_Bounds(get_rect(hwnd));
                              side_browser->Navigate(L"https://wwww.google.com/");
                          }

                          return S_OK;
                      })
                      .Get())))
    {
        return nullptr;
    };

    return browser;
}
