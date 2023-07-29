#include "Browser.hxx"

Browser::Browser(Window& window, Settings& settings, Colors& colors) {}

std::unique_ptr<Browser> Browser::Create(Window& window, Settings& settings, Colors& colors)
{
    auto browser{std::unique_ptr<Browser>(new Browser(window, settings, colors))};

    auto hwnd{window.hwnd};

    auto hr = CreateCoreWebView2EnvironmentWithOptions(
        nullptr, path_portable().wstring().c_str(), nullptr,
        Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [&](HRESULT hr, ICoreWebView2Environment* e) -> HRESULT
            {
                // SETTINGS BROWSER
                e->CreateCoreWebView2Controller(
                    hwnd,
                    Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                        [&](HRESULT hr, ICoreWebView2Controller* c) -> HRESULT
                        {
                            using namespace wv2settings;

                            EventRegistrationToken tokenTitle;
                            EventRegistrationToken tokenFavicon;
                            EventRegistrationToken tokenReceivedMsg;
                            EventRegistrationToken tokenNavigationCompleted;
                            EventRegistrationToken tokenAcceleratorKeyPressed;

                            if (c)
                            {
                                wvController = c;
                                wvController->get_CoreWebView2(&wvCore);
                            }

                            if (wvCore)
                                wvBrowser = wvCore.try_query<ICoreWebView2_19>();

                            if (wvBrowser)
                            {
                                wvBrowser->SetVirtualHostNameToFolderMapping(
                                    L"airglow", path_portable().wstring().c_str(),
                                    COREWEBVIEW2_HOST_RESOURCE_ACCESS_KIND_ALLOW);

                                wvBrowser->get_Settings(&wvSettings);

                                wvSettings->put_AreDefaultContextMenusEnabled(true);
                                wvSettings->put_AreDefaultScriptDialogsEnabled(true);
                                wvSettings->put_AreDevToolsEnabled(true);
                                wvSettings->put_AreHostObjectsAllowed(true);
                                wvSettings->put_IsBuiltInErrorPageEnabled(true);
                                wvSettings->put_IsScriptEnabled(true);
                                wvSettings->put_IsStatusBarEnabled(true);
                                wvSettings->put_IsWebMessageEnabled(true);
                                wvSettings->put_IsZoomControlEnabled(true);
                            }

#ifdef _DEBUG
                            wvBrowser->Navigate(L"https://localhost:8000/");
                            wvBrowser->OpenDevToolsWindow();
#else
                            wvBrowser->Navigate(L"https://airglow/gui/index.html");
#endif

                            browser->Bounds(window, settings);
                            browser->Focus(window, settings);

                            SendMessageW(hwnd, WM_SETFOCUS, 0, 0);

                            wvBrowser->add_DocumentTitleChanged(
                                Callback<ICoreWebView2DocumentTitleChangedEventHandler>(
                                    [&](ICoreWebView2* sender, IUnknown* args) -> HRESULT
                                    {
                                        browser->Title(window, settings);

                                        return S_OK;
                                    })
                                    .Get(),
                                &tokenTitle);

                            wvBrowser->add_FaviconChanged(
                                Callback<ICoreWebView2FaviconChangedEventHandler>(
                                    [&](ICoreWebView2* sender, IUnknown* args) -> HRESULT
                                    {
                                        browser->Icon(window, settings);

                                        return S_OK;
                                    })
                                    .Get(),
                                &tokenFavicon);

                            wvBrowser->add_WebMessageReceived(
                                Callback<ICoreWebView2WebMessageReceivedEventHandler>(
                                    [&](ICoreWebView2* webview,
                                        ICoreWebView2WebMessageReceivedEventArgs* args) -> HRESULT
                                    {
                                        browser->Messages(window, settings, args);

                                        return S_OK;
                                    })
                                    .Get(),
                                &tokenReceivedMsg);

                            wvBrowser->add_NavigationCompleted(
                                Callback<ICoreWebView2NavigationCompletedEventHandler>(
                                    [&](ICoreWebView2* webview,
                                        ICoreWebView2NavigationCompletedEventArgs* args) -> HRESULT
                                    {
                                        browser->PostSettings(settings.Serialize());
                                        browser->PostSettings(colors.Serialize());

                                        return S_OK;
                                    })
                                    .Get(),
                                &tokenNavigationCompleted);

                            wvController->add_AcceleratorKeyPressed(
                                Callback<ICoreWebView2AcceleratorKeyPressedEventHandler>(
                                    [&](ICoreWebView2Controller* sender,
                                        ICoreWebView2AcceleratorKeyPressedEventArgs* args)
                                        -> HRESULT
                                    {
                                        browser->Keys(window, settings, args);

                                        return S_OK;
                                    })
                                    .Get(),
                                &tokenAcceleratorKeyPressed);

                            return S_OK;
                        })
                        .Get());

                // BAR BROWSER
                e->CreateCoreWebView2Controller(
                    hwnd,
                    Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                        [&](HRESULT hr, ICoreWebView2Controller* c) -> HRESULT
                        {
                            using namespace wv2bar;

                            EventRegistrationToken tokenTitle;
                            EventRegistrationToken tokenFavicon;
                            EventRegistrationToken tokenReceivedMsg;
                            EventRegistrationToken tokenNavigationCompleted;
                            EventRegistrationToken tokenAcceleratorKeyPressed;

                            if (c)
                            {
                                wvController = c;
                                wvController->get_CoreWebView2(&wvCore);
                            }

                            if (wvCore)
                                wvBrowser = wvCore.try_query<ICoreWebView2_19>();

                            if (wvBrowser)
                            {
                                wvBrowser->SetVirtualHostNameToFolderMapping(
                                    L"airglow", path_portable().wstring().c_str(),
                                    COREWEBVIEW2_HOST_RESOURCE_ACCESS_KIND_ALLOW);

                                wvBrowser->get_Settings(&wvSettings);

                                wvSettings->put_AreDefaultContextMenusEnabled(true);
                                wvSettings->put_AreDefaultScriptDialogsEnabled(true);
                                wvSettings->put_AreDevToolsEnabled(true);
                                wvSettings->put_AreHostObjectsAllowed(true);
                                wvSettings->put_IsBuiltInErrorPageEnabled(true);
                                wvSettings->put_IsScriptEnabled(true);
                                wvSettings->put_IsStatusBarEnabled(true);
                                wvSettings->put_IsWebMessageEnabled(true);
                                wvSettings->put_IsZoomControlEnabled(true);
                            }

#ifdef _DEBUG
                            wvBrowser->Navigate(L"https://localhost:8000/bar/");
                            wvBrowser->OpenDevToolsWindow();
#else
                            wvBrowser->Navigate(L"https://airglow/gui/bar/index.html");
#endif

                            browser->Bounds(window, settings);
                            browser->Focus(window, settings);

                            SendMessageW(hwnd, WM_SETFOCUS, 0, 0);

                            wvBrowser->add_DocumentTitleChanged(
                                Callback<ICoreWebView2DocumentTitleChangedEventHandler>(
                                    [&](ICoreWebView2* sender, IUnknown* args) -> HRESULT
                                    {
                                        browser->Title(window, settings);

                                        return S_OK;
                                    })
                                    .Get(),
                                &tokenTitle);

                            wvBrowser->add_FaviconChanged(
                                Callback<ICoreWebView2FaviconChangedEventHandler>(
                                    [&](ICoreWebView2* sender, IUnknown* args) -> HRESULT
                                    {
                                        browser->Icon(window, settings);

                                        return S_OK;
                                    })
                                    .Get(),
                                &tokenFavicon);

                            wvBrowser->add_WebMessageReceived(
                                Callback<ICoreWebView2WebMessageReceivedEventHandler>(
                                    [&](ICoreWebView2* webview,
                                        ICoreWebView2WebMessageReceivedEventArgs* args) -> HRESULT
                                    {
                                        browser->Messages(window, settings, args);

                                        return S_OK;
                                    })
                                    .Get(),
                                &tokenReceivedMsg);

                            wvBrowser->add_NavigationCompleted(
                                Callback<ICoreWebView2NavigationCompletedEventHandler>(
                                    [&](ICoreWebView2* webview,
                                        ICoreWebView2NavigationCompletedEventArgs* args) -> HRESULT
                                    {
                                        browser->PostSettings(settings.Serialize());
                                        browser->PostSettings(colors.Serialize());

                                        return S_OK;
                                    })
                                    .Get(),
                                &tokenNavigationCompleted);

                            wvController->add_AcceleratorKeyPressed(
                                Callback<ICoreWebView2AcceleratorKeyPressedEventHandler>(
                                    [&](ICoreWebView2Controller* sender,
                                        ICoreWebView2AcceleratorKeyPressedEventArgs* args)
                                        -> HRESULT
                                    {
                                        browser->Keys(window, settings, args);

                                        return S_OK;
                                    })
                                    .Get(),
                                &tokenAcceleratorKeyPressed);

                            return S_OK;
                        })
                        .Get());

                // MAIN BROWSER
                e->CreateCoreWebView2Controller(
                    hwnd, Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                              [&](HRESULT hr, ICoreWebView2Controller* c) -> HRESULT
                              {
                                  using namespace wv2main;

                                  EventRegistrationToken tokenTitle;
                                  EventRegistrationToken tokenFavicon;
                                  EventRegistrationToken tokenAcceleratorKeyPressed;

                                  if (c)
                                  {
                                      wvController = c;
                                      wvController->get_CoreWebView2(&wvCore);
                                  }

                                  if (wvCore)
                                      wvBrowser = wvCore.try_query<ICoreWebView2_19>();

                                  if (wvBrowser)
                                  {
                                      wvBrowser->get_Settings(&wvSettings);

                                      wvSettings->put_AreDefaultContextMenusEnabled(true);
                                      wvSettings->put_AreDefaultScriptDialogsEnabled(true);
                                      wvSettings->put_AreDevToolsEnabled(true);
                                      wvSettings->put_AreHostObjectsAllowed(true);
                                      wvSettings->put_IsBuiltInErrorPageEnabled(true);
                                      wvSettings->put_IsScriptEnabled(true);
                                      wvSettings->put_IsStatusBarEnabled(true);
                                      wvSettings->put_IsWebMessageEnabled(true);
                                      wvSettings->put_IsZoomControlEnabled(true);
                                  }

                                  if (!command_line().first.empty())
                                      wvBrowser->Navigate(command_line().first.c_str());
                                  else
                                      wvBrowser->Navigate(
                                          (L"https://" + to_wide(settings.mainUrl)).c_str());

                                  browser->Bounds(window, settings);
                                  browser->Focus(window, settings);

                                  wvBrowser->add_DocumentTitleChanged(
                                      Callback<ICoreWebView2DocumentTitleChangedEventHandler>(
                                          [&](ICoreWebView2* sender, IUnknown* args) -> HRESULT
                                          {
                                              browser->Title(window, settings);

                                              return S_OK;
                                          })
                                          .Get(),
                                      &tokenTitle);

                                  wvBrowser->add_FaviconChanged(
                                      Callback<ICoreWebView2FaviconChangedEventHandler>(
                                          [&](ICoreWebView2* sender, IUnknown* args) -> HRESULT
                                          {
                                              browser->Icon(window, settings);

                                              return S_OK;
                                          })
                                          .Get(),
                                      &tokenFavicon);

                                  wvController->add_AcceleratorKeyPressed(
                                      Callback<ICoreWebView2AcceleratorKeyPressedEventHandler>(
                                          [&](ICoreWebView2Controller* sender,
                                              ICoreWebView2AcceleratorKeyPressedEventArgs* args)
                                              -> HRESULT
                                          {
                                              browser->Keys(window, settings, args);

                                              return S_OK;
                                          })
                                          .Get(),
                                      &tokenAcceleratorKeyPressed);

                                  return S_OK;
                              })
                              .Get());

                // SIDE BROWSER
                e->CreateCoreWebView2Controller(
                    hwnd, Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                              [&](HRESULT hr, ICoreWebView2Controller* c) -> HRESULT
                              {
                                  using namespace wv2side;

                                  EventRegistrationToken tokenTitle;
                                  EventRegistrationToken tokenFavicon;
                                  EventRegistrationToken tokenAcceleratorKeyPressed;

                                  if (c)
                                  {
                                      wvController = c;
                                      wvController->get_CoreWebView2(&wvCore);
                                  }

                                  if (wvCore)
                                      wvBrowser = wvCore.try_query<ICoreWebView2_19>();

                                  if (wvBrowser)
                                  {
                                      wvBrowser->get_Settings(&wvSettings);

                                      wvSettings->put_AreDefaultContextMenusEnabled(true);
                                      wvSettings->put_AreDefaultScriptDialogsEnabled(true);
                                      wvSettings->put_AreDevToolsEnabled(true);
                                      wvSettings->put_AreHostObjectsAllowed(true);
                                      wvSettings->put_IsBuiltInErrorPageEnabled(true);
                                      wvSettings->put_IsScriptEnabled(true);
                                      wvSettings->put_IsStatusBarEnabled(true);
                                      wvSettings->put_IsWebMessageEnabled(true);
                                      wvSettings->put_IsZoomControlEnabled(true);
                                  }

                                  if (!command_line().second.empty())
                                      wvBrowser->Navigate(command_line().second.c_str());
                                  else
                                      wvBrowser->Navigate(
                                          (L"https://" + to_wide(settings.sideUrl)).c_str());

                                  browser->Bounds(window, settings);
                                  browser->Focus(window, settings);

                                  wvBrowser->add_DocumentTitleChanged(
                                      Callback<ICoreWebView2DocumentTitleChangedEventHandler>(
                                          [&](ICoreWebView2* sender, IUnknown* args) -> HRESULT
                                          {
                                              browser->Title(window, settings);

                                              return S_OK;
                                          })
                                          .Get(),
                                      &tokenTitle);

                                  wvBrowser->add_FaviconChanged(
                                      Callback<ICoreWebView2FaviconChangedEventHandler>(
                                          [&](ICoreWebView2* sender, IUnknown* args) -> HRESULT
                                          {
                                              browser->Icon(window, settings);

                                              return S_OK;
                                          })
                                          .Get(),
                                      &tokenFavicon);

                                  wvController->add_AcceleratorKeyPressed(
                                      Callback<ICoreWebView2AcceleratorKeyPressedEventHandler>(
                                          [&](ICoreWebView2Controller* sender,
                                              ICoreWebView2AcceleratorKeyPressedEventArgs* args)
                                              -> HRESULT
                                          {
                                              browser->Keys(window, settings, args);

                                              return S_OK;
                                          })
                                          .Get(),
                                      &tokenAcceleratorKeyPressed);

                                  return S_OK;
                              })
                              .Get());

                return S_OK;
            })
            .Get());

    if (hr != S_OK)
        return nullptr;

    return browser;
}

void Browser::Bounds(Window& window, Settings& settings)
{
    if (!wv2main::wvController || !wv2side::wvController || !wv2settings::wvController ||
        !wv2bar::wvController)
        return;

    auto bounds{window_bounds(window.hwnd)};

    if (settings.menu)
    {
        wv2settings::wvController->put_Bounds(bounds);
        wv2bar::wvController->put_Bounds(RECT{0, 0, 0, 0});
        wv2main::wvController->put_Bounds(RECT{0, 0, 0, 0});
        wv2side::wvController->put_Bounds(RECT{0, 0, 0, 0});
    }

    else
    {
        wv2settings::wvController->put_Bounds(RECT{0, 0, 0, 0});

        wv2bar::wvController->put_Bounds(bar_panel(bounds));

        if (!settings.split && !settings.swapped)
        {
            wv2main::wvController->put_Bounds(bounds);
            wv2side::wvController->put_Bounds(RECT{0, 0, 0, 0});

            return;
        }

        if (!settings.split && settings.swapped)
        {
            wv2main::wvController->put_Bounds(RECT{0, 0, 0, 0});
            wv2side::wvController->put_Bounds(bounds);

            return;
        }

        if (!settings.horizontal && settings.split && !settings.swapped)
        {
            wv2main::wvController->put_Bounds(left_panel(bounds));
            wv2side::wvController->put_Bounds(right_panel(bounds));

            return;
        }

        if (!settings.horizontal && settings.split && settings.swapped)
        {
            wv2main::wvController->put_Bounds(right_panel(bounds));
            wv2side::wvController->put_Bounds(left_panel(bounds));

            return;
        }

        if (settings.horizontal && settings.split && !settings.swapped)
        {
            wv2main::wvController->put_Bounds(top_panel(bounds));
            wv2side::wvController->put_Bounds(bottom_panel(bounds));

            return;
        }

        if (settings.horizontal && settings.split && settings.swapped)
        {
            wv2main::wvController->put_Bounds(bottom_panel(bounds));
            wv2side::wvController->put_Bounds(top_panel(bounds));

            return;
        }
    }
}

void Browser::Focus(Window& window, Settings& settings)
{
    if (!wv2main::wvController || !wv2side::wvController || !wv2settings::wvController ||
        !wv2bar::wvController)
        return;

    if (!settings.menu && !settings.swapped)
        wv2main::wvController->MoveFocus(
            COREWEBVIEW2_MOVE_FOCUS_REASON::COREWEBVIEW2_MOVE_FOCUS_REASON_PROGRAMMATIC);

    if (!settings.menu && settings.swapped)
        wv2side::wvController->MoveFocus(
            COREWEBVIEW2_MOVE_FOCUS_REASON::COREWEBVIEW2_MOVE_FOCUS_REASON_PROGRAMMATIC);

    if (settings.menu)
        wv2settings::wvController->MoveFocus(
            COREWEBVIEW2_MOVE_FOCUS_REASON::COREWEBVIEW2_MOVE_FOCUS_REASON_PROGRAMMATIC);
}

void Browser::Title(Window& window, Settings& settings)
{
    if (!wv2main::wvController || !wv2side::wvController || !wv2settings::wvController ||
        !wv2bar::wvController)
        return;

    if (settings.menu)
    {
        wil::unique_cotaskmem_string s{};
        wv2settings::wvBrowser->get_DocumentTitle(&s);
        auto title = s.get();

        if (!settings.topmost)
            SetWindowTextW(window.hwnd, title);

        if (settings.topmost)
        {
            wstring add = title + wstring(L" [On Top]");
            SetWindowTextW(window.hwnd, add.c_str());
        }
    }

    if (!settings.menu && !settings.swapped)
    {
        wil::unique_cotaskmem_string s{};
        wv2main::wvBrowser->get_DocumentTitle(&s);
        auto title = s.get();

        if (!settings.topmost)
            SetWindowTextW(window.hwnd, title);

        if (settings.topmost)
        {
            wstring add = title + wstring(L" [On Top]");
            SetWindowTextW(window.hwnd, add.c_str());
        }
    }

    if (!settings.menu && settings.swapped)
    {
        wil::unique_cotaskmem_string s{};
        wv2side::wvBrowser->get_DocumentTitle(&s);
        auto title = s.get();

        if (!settings.topmost)
            SetWindowTextW(window.hwnd, title);

        if (settings.topmost)
        {
            wstring add = title + wstring(L" [On Top]");
            SetWindowTextW(window.hwnd, add.c_str());
        }
    }
}

void Browser::Icon(Window& window, Settings& settings)
{
    if (!wv2main::wvBrowser || !wv2side::wvBrowser || !wv2settings::wvBrowser || !wv2bar::wvBrowser)
        return;

    if (settings.menu)
    {
        LPWSTR faviconUri;
        wv2settings::wvBrowser->get_FaviconUri(&faviconUri);

        wv2settings::wvBrowser->GetFavicon(
            COREWEBVIEW2_FAVICON_IMAGE_FORMAT_PNG,
            Callback<ICoreWebView2GetFaviconCompletedHandler>(
                [&](HRESULT result, IStream* iconStream) -> HRESULT
                {
                    if (iconStream)
                    {
                        Gdiplus::Bitmap iconBitmap(iconStream);
                        wil::unique_hicon icon;
                        if (iconBitmap.GetHICON(&icon) == Gdiplus::Status::Ok)
                        {
                            auto favicon = std::move(icon);
                            SetClassLongPtrW(window.hwnd, GCLP_HICONSM, (LONG_PTR)favicon.get());
                            SetClassLongPtrW(window.hwnd, GCLP_HICON, (LONG_PTR)window.icon);
                        }
                    }
                    return S_OK;
                })
                .Get());
    }

    if (!settings.swapped && !settings.menu)
    {
        LPWSTR faviconUri;
        wv2main::wvBrowser->get_FaviconUri(&faviconUri);

        wv2main::wvBrowser->GetFavicon(
            COREWEBVIEW2_FAVICON_IMAGE_FORMAT_PNG,
            Callback<ICoreWebView2GetFaviconCompletedHandler>(
                [&](HRESULT result, IStream* iconStream) -> HRESULT
                {
                    if (iconStream)
                    {
                        Gdiplus::Bitmap iconBitmap(iconStream);
                        wil::unique_hicon icon;
                        if (iconBitmap.GetHICON(&icon) == Gdiplus::Status::Ok)
                        {
                            auto favicon = std::move(icon);
                            SetClassLongPtrW(window.hwnd, GCLP_HICONSM, (LONG_PTR)favicon.get());
                            SetClassLongPtrW(window.hwnd, GCLP_HICON, (LONG_PTR)window.icon);
                        }
                    }
                    return S_OK;
                })
                .Get());
    }

    if (settings.swapped && !settings.menu)
    {
        LPWSTR faviconUri;
        wv2side::wvBrowser->get_FaviconUri(&faviconUri);

        wv2side::wvBrowser->GetFavicon(
            COREWEBVIEW2_FAVICON_IMAGE_FORMAT_PNG,
            Callback<ICoreWebView2GetFaviconCompletedHandler>(
                [&](HRESULT result, IStream* iconStream) -> HRESULT
                {
                    if (iconStream)
                    {
                        Gdiplus::Bitmap iconBitmap(iconStream);
                        wil::unique_hicon icon;
                        if (iconBitmap.GetHICON(&icon) == Gdiplus::Status::Ok)
                        {
                            auto favicon = std::move(icon);
                            SetClassLongPtrW(window.hwnd, GCLP_HICONSM, (LONG_PTR)favicon.get());
                            SetClassLongPtrW(window.hwnd, GCLP_HICON, (LONG_PTR)window.icon);
                        }
                    }
                    return S_OK;
                })
                .Get());
    }
}

void Browser::Keys(Window& window, Settings& settings,
                   ICoreWebView2AcceleratorKeyPressedEventArgs* args)
{
    COREWEBVIEW2_KEY_EVENT_KIND kind;
    auto keyCheck{args->get_KeyEventKind(&kind)};
    if (kind == COREWEBVIEW2_KEY_EVENT_KIND_KEY_DOWN ||
        kind == COREWEBVIEW2_KEY_EVENT_KIND_SYSTEM_KEY_DOWN)
    {
        UINT key;
        args->get_VirtualKey(&key);

        println(std::to_string(key));

        switch (key)
        {
        case 19:
            args->put_Handled(TRUE);
            SendMessageW(window.hwnd, WM_KEYDOWN, VK_PAUSE, 0);

            break;

        case 87:
            args->put_Handled(TRUE);
            SendMessageW(window.hwnd, WM_KEYDOWN, 0x57, 0);

            break;

        case 112:
            args->put_Handled(TRUE);
            SendMessageW(window.hwnd, WM_KEYDOWN, VK_F1, 0);

            break;

        case 113:
            args->put_Handled(TRUE);
            SendMessageW(window.hwnd, WM_KEYDOWN, VK_F2, 0);

            break;

        case 114:
            args->put_Handled(TRUE);
            SendMessageW(window.hwnd, WM_KEYDOWN, VK_F3, 0);

            break;

        case 115:
            args->put_Handled(TRUE);
            SendMessageW(window.hwnd, WM_KEYDOWN, VK_F4, 0);

            break;

        case 117:
            args->put_Handled(TRUE);
            SendMessageW(window.hwnd, WM_KEYDOWN, VK_F6, 0);

            break;

        case 118:
            args->put_Handled(TRUE);
            SendMessageW(window.hwnd, WM_KEYDOWN, VK_F7, 0);

            break;

        case 119:
            args->put_Handled(TRUE);
            SendMessageW(window.hwnd, WM_KEYDOWN, VK_F8, 0);

            break;

        case 120:
            args->put_Handled(TRUE);
            SendMessageW(window.hwnd, WM_KEYDOWN, VK_F9, 0);

            break;

        case 121:
            args->put_Handled(TRUE);
            SendMessageW(window.hwnd, WM_KEYDOWN, VK_F10, 0);

            break;

        case 122:
            args->put_Handled(TRUE);
            SendMessageW(window.hwnd, WM_KEYDOWN, VK_F11, 0);

            break;
        }
    }
}

void Browser::Messages(Window& window, Settings& settings,
                       ICoreWebView2WebMessageReceivedEventArgs* args)
{
    wil::unique_cotaskmem_string messageRaw;

    if (SUCCEEDED(args->get_WebMessageAsJson(&messageRaw)))
    {
        auto j = json::parse(to_string(messageRaw.get()));

        settings.mainUrl = j["mainUrl"].get<string>();
        settings.sideUrl = j["sideUrl"].get<string>();

        SendMessageW(window.hwnd, WM_NOTIFY, 0, 0);
    };
}

void Browser::PostSettings(json j)
{
    if (!wv2settings::wvBrowser || !wv2bar::wvBrowser)
        return;

    wv2settings::wvBrowser->PostWebMessageAsJson(to_wide(j.dump()).c_str());
    wv2bar::wvBrowser->PostWebMessageAsJson(to_wide(j.dump()).c_str());
}

void Browser::NavigateHome(Settings& settings)
{
    if (!wv2main::wvBrowser || !wv2side::wvBrowser)
        return;

    wv2main::wvBrowser->Navigate((L"https://" + to_wide(settings.mainUrl)).c_str());
    wv2side::wvBrowser->Navigate((L"https://" + to_wide(settings.sideUrl)).c_str());
}
