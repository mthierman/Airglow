#include "Browser.hxx"
#include "Utility.hxx"

static wil::com_ptr<ICoreWebView2Environment> browser_environment;
static wil::com_ptr<ICoreWebView2Controller> settings_controller{nullptr};
static wil::com_ptr<ICoreWebView2> settings_core{nullptr};
static wil::com_ptr<ICoreWebView2_19> settings_browser{nullptr};
static wil::com_ptr<ICoreWebView2Settings> settings_settings{nullptr};
static wil::com_ptr<ICoreWebView2Controller> main_controller{nullptr};
static wil::com_ptr<ICoreWebView2> main_core{nullptr};
static wil::com_ptr<ICoreWebView2_19> main_browser{nullptr};
static wil::com_ptr<ICoreWebView2Settings> main_settings{nullptr};
static wil::com_ptr<ICoreWebView2Controller> side_controller{nullptr};
static wil::com_ptr<ICoreWebView2> side_core{nullptr};
static wil::com_ptr<ICoreWebView2_19> side_browser{nullptr};
static wil::com_ptr<ICoreWebView2Settings> side_settings{nullptr};

Browser::Browser() {}

std::unique_ptr<Browser> Browser::Create()
{

    auto browser{std::unique_ptr<Browser>(new Browser())};

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

    return browser;
}

void Browser::CreateBrowsers(HWND hwnd)
{
    browser_environment->CreateCoreWebView2Controller(
        hwnd, Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                  [=](HRESULT hr, ICoreWebView2Controller* controller) -> HRESULT
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
                          //   settings_controller->put_Bounds(get_rect(hwnd));
                          settings_browser->Navigate(L"https://wwww.google.com/");
                      }

                      return S_OK;
                  })
                  .Get());

    browser_environment->CreateCoreWebView2Controller(
        hwnd, Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                  [=](HRESULT hr, ICoreWebView2Controller* controller) -> HRESULT
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
                          //   main_controller->put_Bounds(get_rect(hwnd));
                          main_browser->Navigate(L"https://wwww.google.com/");
                      }

                      return S_OK;
                  })
                  .Get());

    browser_environment->CreateCoreWebView2Controller(
        hwnd, Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                  [=](HRESULT hr, ICoreWebView2Controller* controller) -> HRESULT
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
                          //   side_controller->put_Bounds(get_rect(hwnd));
                          side_browser->Navigate(L"https://wwww.google.com/");
                      }

                      return S_OK;
                  })
                  .Get());
}

// void Bounds(HWND hwnd)
// {
//     settings_controller->put_Bounds(get_rect(hwnd));
//     main_controller->put_Bounds(get_rect(hwnd));
//     side_controller->put_Bounds(get_rect(hwnd));
// }
