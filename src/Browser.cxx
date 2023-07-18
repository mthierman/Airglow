#include "Browser.hxx"

using namespace WebView;

Browser::Browser(State::Window& window) {}

std::unique_ptr<Browser> Browser::Create(State::Window& window)
{
    auto browser{std::unique_ptr<Browser>(new Browser(window))};

    auto hwnd{window.hwnd};

    if (FAILED(CreateCoreWebView2EnvironmentWithOptions(
            nullptr, path_portable().wstring().c_str(), nullptr,
            Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
                [&](HRESULT hr, ICoreWebView2Environment* e) -> HRESULT
                {
                    e->CreateCoreWebView2Controller(
                        hwnd, Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                                  [&](HRESULT hr, ICoreWebView2Controller* c) -> HRESULT
                                  {
                                      EventRegistrationToken tokenTitle;
                                      EventRegistrationToken tokenFavicon;
                                      EventRegistrationToken tokenReceivedMsg;
                                      EventRegistrationToken tokenNavigationCompleted;

                                      if (c)
                                      {
                                          Controller::main = c;
                                          Controller::main->get_CoreWebView2(&Core::main);
                                      }

                                      if (Core::main)
                                          Core19::main = Core::main.try_query<ICoreWebView2_19>();

                                      if (Core19::main)
                                      {
                                          Core19::main->AddScriptToExecuteOnDocumentCreated(
                                              js_inject_embed().c_str(), nullptr);

                                          Core19::main->get_Settings(&Settings::main);

                                          Settings::main->put_AreDefaultContextMenusEnabled(true);
                                          Settings::main->put_AreDefaultScriptDialogsEnabled(true);
                                          Settings::main->put_AreDevToolsEnabled(true);
                                          Settings::main->put_AreHostObjectsAllowed(true);
                                          Settings::main->put_IsBuiltInErrorPageEnabled(true);
                                          Settings::main->put_IsScriptEnabled(true);
                                          Settings::main->put_IsStatusBarEnabled(false);
                                          Settings::main->put_IsWebMessageEnabled(true);
                                          Settings::main->put_IsZoomControlEnabled(false);
                                      }

                                      //   auto url{L"https://" + command_line().first};
                                      //   if (!url.empty())
                                      //       Core19::main->Navigate(url.c_str());
                                      //   else
                                      //       Core19::main->Navigate(L"https://www.bing.com/");

                                      Core19::main->Navigate(L"https://www.bing.com/");

                                      browser->Bounds(window);
                                      //   browser->Focus(window);

                                      Core19::main->add_DocumentTitleChanged(
                                          Callback<ICoreWebView2DocumentTitleChangedEventHandler>(
                                              [](ICoreWebView2* sender, IUnknown* args) -> HRESULT
                                              {
                                                  //   set_title();

                                                  return S_OK;
                                              })
                                              .Get(),
                                          &tokenTitle);

                                      Core19::main->add_FaviconChanged(
                                          Callback<ICoreWebView2FaviconChangedEventHandler>(
                                              [](ICoreWebView2* sender, IUnknown* args) -> HRESULT
                                              {
                                                  //   set_icon();

                                                  return S_OK;
                                              })
                                              .Get(),
                                          &tokenFavicon);

                                      Core19::main->add_WebMessageReceived(
                                          Callback<ICoreWebView2WebMessageReceivedEventHandler>(
                                              [&](ICoreWebView2* webview,
                                                  ICoreWebView2WebMessageReceivedEventArgs* args)
                                                  -> HRESULT
                                              {
                                                  browser->Messages(window, args);

                                                  return S_OK;
                                              })
                                              .Get(),
                                          &tokenReceivedMsg);

                                      return S_OK;
                                  })
                                  .Get());

                    e->CreateCoreWebView2Controller(
                        hwnd, Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                                  [&](HRESULT hr, ICoreWebView2Controller* c) -> HRESULT
                                  {
                                      EventRegistrationToken tokenTitle;
                                      EventRegistrationToken tokenFavicon;
                                      EventRegistrationToken tokenReceivedMsg;
                                      EventRegistrationToken tokenNavigationCompleted;

                                      if (c)
                                      {
                                          Controller::side = c;
                                          Controller::side->get_CoreWebView2(&Core::side);
                                      }

                                      if (Core::side)
                                          Core19::side = Core::side.try_query<ICoreWebView2_19>();

                                      if (Core19::side)
                                      {
                                          Core19::side->AddScriptToExecuteOnDocumentCreated(
                                              js_inject_embed().c_str(), nullptr);

                                          Core19::side->get_Settings(&Settings::side);

                                          Settings::side->put_AreDefaultContextMenusEnabled(true);
                                          Settings::side->put_AreDefaultScriptDialogsEnabled(true);
                                          Settings::side->put_AreDevToolsEnabled(true);
                                          Settings::side->put_AreHostObjectsAllowed(true);
                                          Settings::side->put_IsBuiltInErrorPageEnabled(true);
                                          Settings::side->put_IsScriptEnabled(true);
                                          Settings::side->put_IsStatusBarEnabled(false);
                                          Settings::side->put_IsWebMessageEnabled(true);
                                          Settings::side->put_IsZoomControlEnabled(false);
                                      }

                                      //   auto url{L"https://" + command_line().second};
                                      //   if (!url.empty())
                                      //       Core19::side->Navigate(url.c_str());
                                      //   else
                                      //       Core19::side->Navigate(L"https://www.google.com/");

                                      Core19::side->Navigate(L"https://www.google.com/");

                                      browser->Bounds(window);
                                      //   browser->Focus(window);

                                      SendMessageW(hwnd, WM_SETFOCUS, 0, 0);

                                      Core19::side->add_DocumentTitleChanged(
                                          Callback<ICoreWebView2DocumentTitleChangedEventHandler>(
                                              [](ICoreWebView2* sender, IUnknown* args) -> HRESULT
                                              {
                                                  //   set_title();

                                                  return S_OK;
                                              })
                                              .Get(),
                                          &tokenTitle);

                                      Core19::side->add_FaviconChanged(
                                          Callback<ICoreWebView2FaviconChangedEventHandler>(
                                              [](ICoreWebView2* sender, IUnknown* args) -> HRESULT
                                              {
                                                  //   set_icon();

                                                  return S_OK;
                                              })
                                              .Get(),
                                          &tokenFavicon);

                                      Core19::side->add_WebMessageReceived(
                                          Callback<ICoreWebView2WebMessageReceivedEventHandler>(
                                              [&](ICoreWebView2* webview,
                                                  ICoreWebView2WebMessageReceivedEventArgs* args)
                                                  -> HRESULT
                                              {
                                                  browser->Messages(window, args);

                                                  return S_OK;
                                              })
                                              .Get(),
                                          &tokenReceivedMsg);

                                      return S_OK;
                                  })
                                  .Get());

                    e->CreateCoreWebView2Controller(
                        hwnd,
                        Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                            [&](HRESULT hr, ICoreWebView2Controller* c) -> HRESULT
                            {
                                EventRegistrationToken tokenTitle;
                                EventRegistrationToken tokenFavicon;
                                EventRegistrationToken tokenReceivedMsg;
                                EventRegistrationToken tokenNavigationCompleted;

                                if (c)
                                {
                                    Controller::settings = c;
                                    Controller::settings->get_CoreWebView2(&Core::settings);
                                }

                                if (Core::settings)
                                    Core19::settings = Core::settings.try_query<ICoreWebView2_19>();

                                if (Core19::settings)
                                {
                                    Core19::settings->get_Settings(&Settings::settings);

                                    Settings::settings->put_AreDefaultContextMenusEnabled(true);
                                    Settings::settings->put_AreDefaultScriptDialogsEnabled(true);
                                    Settings::settings->put_AreDevToolsEnabled(true);
                                    Settings::settings->put_AreHostObjectsAllowed(true);
                                    Settings::settings->put_IsBuiltInErrorPageEnabled(true);
                                    Settings::settings->put_IsScriptEnabled(true);
                                    Settings::settings->put_IsStatusBarEnabled(false);
                                    Settings::settings->put_IsWebMessageEnabled(true);
                                    Settings::settings->put_IsZoomControlEnabled(false);
                                }

                                Core19::settings->SetVirtualHostNameToFolderMapping(
                                    L"settings", path_settings().wstring().c_str(),
                                    COREWEBVIEW2_HOST_RESOURCE_ACCESS_KIND_ALLOW);

                                Core19::settings->Navigate(L"https://settings/index.html");

                                browser->Bounds(window);
                                // browser->Focus(window);

                                SendMessageW(hwnd, WM_SETFOCUS, 0, 0);

                                Core19::settings->add_DocumentTitleChanged(
                                    Callback<ICoreWebView2DocumentTitleChangedEventHandler>(
                                        [](ICoreWebView2* sender, IUnknown* args) -> HRESULT
                                        {
                                            //   set_title();

                                            return S_OK;
                                        })
                                        .Get(),
                                    &tokenTitle);

                                Core19::settings->add_FaviconChanged(
                                    Callback<ICoreWebView2FaviconChangedEventHandler>(
                                        [](ICoreWebView2* sender, IUnknown* args) -> HRESULT
                                        {
                                            //   set_icon();

                                            return S_OK;
                                        })
                                        .Get(),
                                    &tokenFavicon);

                                Core19::settings->add_WebMessageReceived(
                                    Callback<ICoreWebView2WebMessageReceivedEventHandler>(
                                        [&](ICoreWebView2* webview,
                                            ICoreWebView2WebMessageReceivedEventArgs* args)
                                            -> HRESULT
                                        {
                                            browser->Messages(window, args);

                                            return S_OK;
                                        })
                                        .Get(),
                                    &tokenReceivedMsg);

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

void Browser::Bounds(State::Window& window)
{
    using namespace Controller;
    if (!settings || !main || !side)
        return;

    auto bounds{window_bounds(window.hwnd)};

    if (window.menu)
    {
        main->put_Bounds(RECT{0, 0, 0, 0});
        side->put_Bounds(RECT{0, 0, 0, 0});
        settings->put_Bounds(bounds);
    }

    else
    {
        settings->put_Bounds(RECT{0, 0, 0, 0});

        if (!window.split && !window.swapped)
        {
            main->put_Bounds(bounds);
            side->put_Bounds(RECT{0, 0, 0, 0});
        }

        if (!window.split && window.swapped)
        {
            main->put_Bounds(RECT{0, 0, 0, 0});
            side->put_Bounds(bounds);
        }

        if (window.split && !window.swapped)
        {
            main->put_Bounds(left_panel(bounds));
            side->put_Bounds(right_panel(bounds));
        }

        if (window.split && window.swapped)
        {
            main->put_Bounds(right_panel(bounds));
            side->put_Bounds(left_panel(bounds));
        }
    }
}

void Browser::Focus(State::Window& window)
{
    using namespace Controller;
    if (!settings || !main || !side)
        return;

    if (!window.menu && !window.swapped)
        main->MoveFocus(
            COREWEBVIEW2_MOVE_FOCUS_REASON::COREWEBVIEW2_MOVE_FOCUS_REASON_PROGRAMMATIC);

    if (!window.menu && window.swapped)
        side->MoveFocus(
            COREWEBVIEW2_MOVE_FOCUS_REASON::COREWEBVIEW2_MOVE_FOCUS_REASON_PROGRAMMATIC);

    if (window.menu)
        settings->MoveFocus(
            COREWEBVIEW2_MOVE_FOCUS_REASON::COREWEBVIEW2_MOVE_FOCUS_REASON_PROGRAMMATIC);
}

void Browser::Messages(State::Window& window, ICoreWebView2WebMessageReceivedEventArgs* args)
{
    string splitKey{"F1"};
    string swapKey{"F2"};
    string hideMenuKey{"F4"};
    string maximizeKey{"F6"};
    string fullscreenKey{"F11"};
    string onTopKey{"F9"};
    string closeKey{"close"};

    wil::unique_cotaskmem_string messageRaw;
    if (SUCCEEDED(args->TryGetWebMessageAsString(&messageRaw)))
    {
        auto message = to_string(wstring(messageRaw.get()));

        if (message.compare(0, 8, "mainUrl ") == 0)
        {
            window.mainUrl = message.substr(8);
        }

        if (message.compare(0, 8, "sideUrl ") == 0)
        {
            window.sideUrl = message.substr(8);
        }

        if (message == splitKey)
        {
            SendMessageW(window.hwnd, WM_KEYDOWN, VK_F1, 0);
        }

        if (message == swapKey)
        {
            SendMessageW(window.hwnd, WM_KEYDOWN, VK_F2, 0);
        }

        if (message == hideMenuKey)
        {
            SendMessageW(window.hwnd, WM_KEYDOWN, VK_F4, 0);
        }

        if (message == maximizeKey)
        {
            SendMessageW(window.hwnd, WM_KEYDOWN, VK_F6, 0);
        }

        if (message == fullscreenKey)
        {
            SendMessageW(window.hwnd, WM_KEYDOWN, VK_F11, 0);
        }

        if (message == onTopKey)
        {
            SendMessageW(window.hwnd, WM_KEYDOWN, VK_F9, 0);
        }

        if (message == closeKey)
        {
            SendMessageW(window.hwnd, WM_KEYDOWN, 0x57, 0);
        }
    }
}
