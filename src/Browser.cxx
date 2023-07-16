#include "Browser.hxx"

using namespace WebView;

Browser::Browser(HWND hwnd) {}

std::unique_ptr<Browser> Browser::Create(HWND hwnd)
{
    auto browser{std::unique_ptr<Browser>(new Browser(hwnd))};

    browser->hwnd = hwnd;

    if (FAILED(CreateCoreWebView2EnvironmentWithOptions(
            nullptr, path_portable().wstring().c_str(), nullptr,
            Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
                [&browser, hwnd](HRESULT hr, ICoreWebView2Environment* e) -> HRESULT
                {
                    e->CreateCoreWebView2Controller(
                        hwnd, Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                                  [hwnd](HRESULT hr, ICoreWebView2Controller* c) -> HRESULT
                                  {
                                      if (c)
                                      {
                                          Controller::main = c;
                                          Controller::main->get_CoreWebView2(&Core::main);
                                      }

                                      if (Core::main)
                                          Core19::main = Core::main.try_query<ICoreWebView2_19>();

                                      Core19::main->Navigate(L"https://www.juce.com/");

                                      auto bounds{get_rect(hwnd)};
                                      Controller::main->put_Bounds(RECT{
                                          bounds.left,
                                          bounds.top,
                                          bounds.right / 2,
                                          bounds.bottom,
                                      });

                                      return S_OK;
                                  })
                                  .Get());

                    e->CreateCoreWebView2Controller(
                        hwnd, Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                                  [hwnd](HRESULT hr, ICoreWebView2Controller* c) -> HRESULT
                                  {
                                      if (c)
                                      {
                                          Controller::side = c;
                                          Controller::side->get_CoreWebView2(&Core::side);
                                      }

                                      if (Core::side)
                                          Core19::side = Core::side.try_query<ICoreWebView2_19>();

                                      Core19::side->Navigate(L"https://www.google.com/");

                                      auto bounds{get_rect(hwnd)};
                                      Controller::side->put_Bounds(RECT{
                                          bounds.right / 2,
                                          bounds.top,
                                          bounds.right,
                                          bounds.bottom,
                                      });

                                      return S_OK;
                                  })
                                  .Get());

                    e->CreateCoreWebView2Controller(
                        hwnd, Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                                  [hwnd](HRESULT hr, ICoreWebView2Controller* c) -> HRESULT
                                  {
                                      if (c)
                                      {
                                          Controller::settings = c;
                                          Controller::settings->get_CoreWebView2(&Core::settings);
                                      }

                                      if (Core::settings)
                                          Core19::settings =
                                              Core::settings.try_query<ICoreWebView2_19>();

                                      Core19::settings->Navigate(L"https://www.github.com/");

                                      auto bounds{get_rect(hwnd)};
                                      Controller::settings->put_Bounds(RECT{0, 0, 0, 0});

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

void Browser::Bounds()
{
    using namespace Controller;
    if (!settings || !main || !side)
        return;

    auto bounds{get_rect(hwnd)};

    main->put_Bounds(RECT{
        bounds.left,
        bounds.top,
        bounds.right / 2,
        bounds.bottom,
    });

    side->put_Bounds(RECT{
        bounds.right / 2,
        bounds.top,
        bounds.right,
        bounds.bottom,
    });

    settings->put_Bounds(RECT{0, 0, 0, 0});
}

void Browser::Focus()
{
    // settings->MoveFocus(
    //     COREWEBVIEW2_MOVE_FOCUS_REASON::COREWEBVIEW2_MOVE_FOCUS_REASON_PROGRAMMATIC);

    // main->MoveFocus(COREWEBVIEW2_MOVE_FOCUS_REASON::COREWEBVIEW2_MOVE_FOCUS_REASON_PROGRAMMATIC);

    // side->MoveFocus(COREWEBVIEW2_MOVE_FOCUS_REASON::COREWEBVIEW2_MOVE_FOCUS_REASON_PROGRAMMATIC);
}

void Browser::Navigate()
{
    using namespace Core19;
    if (!settings || !main || !side)
        return;

    auto args = command_line();

    println("TEST");

    main->Navigate(L"https://www.bing.com/");

    side->Navigate(L"https://www.bing.com/");

    settings->Navigate(L"https://www.bing.com/");
}
