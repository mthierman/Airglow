#include "Browser.hxx"

Browser::Browser(HWND hwnd) {}

std::unique_ptr<Browser> Browser::Create(HWND hwnd)
{
    auto browser{std::unique_ptr<Browser>(new Browser(hwnd))};

    if (FAILED(CreateCoreWebView2EnvironmentWithOptions(
            nullptr, path_portable().wstring().c_str(), nullptr,
            Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
                [=](HRESULT hr, ICoreWebView2Environment* e) -> HRESULT
                {
                    e->CreateCoreWebView2Controller(
                        hwnd, Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                                  [=](HRESULT hr, ICoreWebView2Controller* controller) -> HRESULT
                                  {
                                      using namespace Browsers::Settings;

                                      if (controller)
                                      {
                                          settings_controller = controller;
                                          settings_controller->get_CoreWebView2(&settings_core);
                                      }

                                      if (settings_core)
                                          settings_browser =
                                              settings_core.try_query<ICoreWebView2_19>();

                                      if (settings_browser)
                                      {
                                          //   settings_controller->put_Bounds(get_rect(hwnd));
                                          //   settings_browser->Navigate(L"https://wwww.google.com/");
                                      }

                                      return S_OK;
                                  })
                                  .Get());

                    e->CreateCoreWebView2Controller(
                        hwnd, Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                                  [=](HRESULT hr, ICoreWebView2Controller* controller) -> HRESULT
                                  {
                                      using namespace Browsers::Main;

                                      if (controller)
                                      {
                                          main_controller = controller;
                                          main_controller->get_CoreWebView2(&main_core);
                                      }

                                      if (main_core)
                                          main_browser = main_core.try_query<ICoreWebView2_19>();

                                      if (main_browser)
                                      {
                                          //   main_controller->put_Bounds(get_rect(hwnd));
                                          //   main_browser->Navigate(L"https://wwww.google.com/");
                                      }

                                      return S_OK;
                                  })
                                  .Get());

                    e->CreateCoreWebView2Controller(
                        hwnd, Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                                  [=](HRESULT hr, ICoreWebView2Controller* controller) -> HRESULT
                                  {
                                      using namespace Browsers::Side;

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
                                  .Get());

                    return S_OK;
                })
                .Get())))
    {
        return nullptr;
    };

    return browser;
}
