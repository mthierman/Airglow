#include "Browser.hxx"

Browser::Browser(Window& window, Settings& settings, Colors& colors) {}

std::unique_ptr<Browser> Browser::Create(Window& window, Settings& settings, Colors& colors)
{
    // SetEnvironmentVariableW(L"WEBVIEW2_DEFAULT_BACKGROUND_COLOR", L"0");
    // SetEnvironmentVariableW(L"WEBVIEW2_ADDITIONAL_BROWSER_ARGUMENTS",
    // L"--allow-file-access-from-files");

    auto browser{std::unique_ptr<Browser>(new Browser(window, settings, colors))};

    auto hwnd{window.hwnd};

    auto options{Microsoft::WRL::Make<CoreWebView2EnvironmentOptions>()};
    options->put_AdditionalBrowserArguments(L"--allow-file-access-from-files");

    auto env{CreateCoreWebView2EnvironmentWithOptions(
        nullptr, path_portable().wstring().c_str(), options.Get(),
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
                                wvCoreController = c;

                                wvController =
                                    wvCoreController.try_query<ICoreWebView2Controller4>();

                                // COREWEBVIEW2_COLOR color{0, 0, 0, 0};
                                // wvController->put_DefaultBackgroundColor(color);

                                wvController->get_CoreWebView2(&wvCore);
                            }

                            wvBrowser = wvCore.try_query<ICoreWebView2_19>();

                            wvBrowser->get_Settings(&wvSettings);

                            wvSettings->put_AreDefaultContextMenusEnabled(true);
                            wvSettings->put_AreDefaultScriptDialogsEnabled(true);
                            wvSettings->put_AreHostObjectsAllowed(true);
                            wvSettings->put_IsBuiltInErrorPageEnabled(true);
                            wvSettings->put_IsScriptEnabled(true);
                            wvSettings->put_IsStatusBarEnabled(false);
                            wvSettings->put_IsWebMessageEnabled(true);
                            wvSettings->put_IsZoomControlEnabled(false);

#ifdef _DEBUG
                            wvSettings->put_AreDevToolsEnabled(true);
                            // wvBrowser->OpenDevToolsWindow();
                            wvBrowser->Navigate(L"https://localhost:8000/settings/");
#else
                            wvSettings->put_AreDevToolsEnabled(false);
                            wvBrowser->Navigate(path_settings().wstring().c_str());
#endif

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

                            wvBrowser->add_WebMessageReceived(
                                Callback<ICoreWebView2WebMessageReceivedEventHandler>(
                                    [&](ICoreWebView2* webview,
                                        ICoreWebView2WebMessageReceivedEventArgs* args) -> HRESULT
                                    {
                                        browser->SettingsMessages(window, settings, args);

                                        return S_OK;
                                    })
                                    .Get(),
                                &tokenReceivedMsg);

                            wvBrowser->add_NavigationCompleted(
                                Callback<ICoreWebView2NavigationCompletedEventHandler>(
                                    [&](ICoreWebView2* webview,
                                        ICoreWebView2NavigationCompletedEventArgs* args) -> HRESULT
                                    {
                                        SendMessageW(window.hwnd, WM_NOTIFY, 0, 0);

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
                                wvCoreController = c;

                                wvController =
                                    wvCoreController.try_query<ICoreWebView2Controller4>();

                                // COREWEBVIEW2_COLOR color{0, 0, 0, 0};
                                // wvController->put_DefaultBackgroundColor(color);

                                wvController->get_CoreWebView2(&wvCore);
                            }

                            wvBrowser = wvCore.try_query<ICoreWebView2_19>();

                            wvBrowser->get_Settings(&wvSettings);

                            wvSettings->put_AreDefaultContextMenusEnabled(true);
                            wvSettings->put_AreDefaultScriptDialogsEnabled(true);
                            wvSettings->put_AreHostObjectsAllowed(true);
                            wvSettings->put_IsBuiltInErrorPageEnabled(true);
                            wvSettings->put_IsScriptEnabled(true);
                            wvSettings->put_IsStatusBarEnabled(false);
                            wvSettings->put_IsWebMessageEnabled(true);
                            wvSettings->put_IsZoomControlEnabled(false);

#ifdef _DEBUG
                            wvSettings->put_AreDevToolsEnabled(true);
                            // wvBrowser->OpenDevToolsWindow();
                            wvBrowser->Navigate(L"https://localhost:8000/bar/");
#else
                            wvSettings->put_AreDevToolsEnabled(false);
                            wvBrowser->Navigate(path_bar().wstring().c_str());
#endif

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

                            wvBrowser->add_WebMessageReceived(
                                Callback<ICoreWebView2WebMessageReceivedEventHandler>(
                                    [&](ICoreWebView2* webview,
                                        ICoreWebView2WebMessageReceivedEventArgs* args) -> HRESULT
                                    {
                                        browser->BarMessages(window, settings, args);

                                        return S_OK;
                                    })
                                    .Get(),
                                &tokenReceivedMsg);

                            wvBrowser->add_NavigationCompleted(
                                Callback<ICoreWebView2NavigationCompletedEventHandler>(
                                    [&](ICoreWebView2* webview,
                                        ICoreWebView2NavigationCompletedEventArgs* args) -> HRESULT
                                    {
                                        SendMessageW(window.hwnd, WM_NOTIFY, 0, 0);

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
                                  EventRegistrationToken tokenSourceChanged;
                                  EventRegistrationToken tokenAcceleratorKeyPressed;

                                  if (c)
                                  {
                                      wvCoreController = c;

                                      wvController =
                                          wvCoreController.try_query<ICoreWebView2Controller4>();

                                      //   COREWEBVIEW2_COLOR color{0, 0, 0, 0};
                                      //   wvController->put_DefaultBackgroundColor(color);

                                      wvController->get_CoreWebView2(&wvCore);
                                  }

                                  wvBrowser = wvCore.try_query<ICoreWebView2_19>();

                                  wvBrowser->get_Settings(&wvSettings);

                                  wvSettings->put_AreDefaultContextMenusEnabled(true);
                                  wvSettings->put_AreDefaultScriptDialogsEnabled(true);
                                  wvSettings->put_AreDevToolsEnabled(true);
                                  wvSettings->put_AreHostObjectsAllowed(true);
                                  wvSettings->put_IsBuiltInErrorPageEnabled(true);
                                  wvSettings->put_IsScriptEnabled(true);
                                  wvSettings->put_IsStatusBarEnabled(false);
                                  wvSettings->put_IsWebMessageEnabled(true);
                                  wvSettings->put_IsZoomControlEnabled(true);

                                  browser->NavigateMain(settings);
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

                                  wvBrowser->add_SourceChanged(
                                      Callback<ICoreWebView2SourceChangedEventHandler>(
                                          [&](ICoreWebView2* sender,
                                              ICoreWebView2SourceChangedEventArgs* args) -> HRESULT
                                          {
                                              wil::unique_cotaskmem_string uri;
                                              sender->get_Source(&uri);

                                              settings.currentPageMain = to_string(uri.get());

                                              SendMessageW(window.hwnd, WM_NOTIFY, 0, 0);

                                              return S_OK;
                                          })
                                          .Get(),
                                      &tokenSourceChanged);

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
                                  EventRegistrationToken tokenSourceChanged;
                                  EventRegistrationToken tokenAcceleratorKeyPressed;

                                  if (c)
                                  {
                                      wvCoreController = c;

                                      wvController =
                                          wvCoreController.try_query<ICoreWebView2Controller4>();

                                      //   COREWEBVIEW2_COLOR color{0, 0, 0, 0};
                                      //   wvController->put_DefaultBackgroundColor(color);

                                      wvController->get_CoreWebView2(&wvCore);
                                  }

                                  wvBrowser = wvCore.try_query<ICoreWebView2_19>();

                                  wvBrowser->get_Settings(&wvSettings);

                                  wvSettings->put_AreDefaultContextMenusEnabled(true);
                                  wvSettings->put_AreDefaultScriptDialogsEnabled(true);
                                  wvSettings->put_AreDevToolsEnabled(true);
                                  wvSettings->put_AreHostObjectsAllowed(true);
                                  wvSettings->put_IsBuiltInErrorPageEnabled(true);
                                  wvSettings->put_IsScriptEnabled(true);
                                  wvSettings->put_IsStatusBarEnabled(false);
                                  wvSettings->put_IsWebMessageEnabled(true);
                                  wvSettings->put_IsZoomControlEnabled(true);

                                  browser->NavigateSide(settings);
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

                                  wvBrowser->add_SourceChanged(
                                      Callback<ICoreWebView2SourceChangedEventHandler>(
                                          [&](ICoreWebView2* sender,
                                              ICoreWebView2SourceChangedEventArgs* args) -> HRESULT
                                          {
                                              wil::unique_cotaskmem_string uri;
                                              sender->get_Source(&uri);

                                              settings.currentPageSide = to_string(uri.get());

                                              SendMessageW(window.hwnd, WM_NOTIFY, 0, 0);

                                              return S_OK;
                                          })
                                          .Get(),
                                      &tokenSourceChanged);

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
            .Get())};

    if (env != S_OK)
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
            wv2main::wvController->put_Bounds(full_panel(bounds));
            wv2side::wvController->put_Bounds(RECT{0, 0, 0, 0});

            return;
        }

        if (!settings.split && settings.swapped)
        {
            wv2main::wvController->put_Bounds(RECT{0, 0, 0, 0});
            wv2side::wvController->put_Bounds(full_panel(bounds));

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
    // if (!wv2main::wvController || !wv2side::wvController || !wv2settings::wvController ||
    //     !wv2bar::wvController)
    //     return;

    // if (!settings.menu && !settings.swapped)
    //     wv2main::wvController->MoveFocus(
    //         COREWEBVIEW2_MOVE_FOCUS_REASON::COREWEBVIEW2_MOVE_FOCUS_REASON_PROGRAMMATIC);

    // if (!settings.menu && settings.swapped)
    //     wv2side::wvController->MoveFocus(
    //         COREWEBVIEW2_MOVE_FOCUS_REASON::COREWEBVIEW2_MOVE_FOCUS_REASON_PROGRAMMATIC);

    // if (settings.menu)
    //     wv2settings::wvController->MoveFocus(
    //         COREWEBVIEW2_MOVE_FOCUS_REASON::COREWEBVIEW2_MOVE_FOCUS_REASON_PROGRAMMATIC);
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
        SetClassLongPtrW(window.hwnd, GCLP_HICONSM, (LONG_PTR)window.icon);
        SetClassLongPtrW(window.hwnd, GCLP_HICON, (LONG_PTR)window.icon);
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

        case 76:
            args->put_Handled(TRUE);
            SendMessageW(window.hwnd, WM_KEYDOWN, 0x4C, 0);

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

void Browser::SettingsMessages(Window& window, Settings& settings,
                               ICoreWebView2WebMessageReceivedEventArgs* args)
{
    wil::unique_cotaskmem_string messageRaw;

    if (SUCCEEDED(args->get_WebMessageAsJson(&messageRaw)))
    {
        auto j = json::parse(to_string(messageRaw.get()));

        if (!j["homepageMain"].empty())
        {
            auto s{j["homepageMain"].get<string>()};

            if (s.starts_with("https://") || s.starts_with("http://"))
                settings.homepageMain = s;
            else
                settings.homepageMain = "https://" + s;
        }

        if (!j["homepageSide"].empty())
        {
            auto s{j["homepageSide"].get<string>()};

            if (s.starts_with("https://") || s.starts_with("http://"))
                settings.homepageSide = s;
            else
                settings.homepageSide = "https://" + s;
        }

        SendMessageW(window.hwnd, WM_NOTIFY, 0, 0);
    };
}

void Browser::BarMessages(Window& window, Settings& settings,
                          ICoreWebView2WebMessageReceivedEventArgs* args)
{
    wil::unique_cotaskmem_string messageRaw;

    if (SUCCEEDED(args->get_WebMessageAsJson(&messageRaw)))
    {
        auto j = json::parse(to_string(messageRaw.get()));

        if (!j["currentPageMain"].empty())
        {
            auto s{j["currentPageMain"].get<string>()};

            if (s.starts_with("https://"))
                wv2main::wvBrowser->Navigate((to_wide(s)).c_str());

            else
                wv2main::wvBrowser->Navigate((L"https://" + to_wide(s)).c_str());

            wv2main::wvController->MoveFocus(
                COREWEBVIEW2_MOVE_FOCUS_REASON::COREWEBVIEW2_MOVE_FOCUS_REASON_PROGRAMMATIC);
        }

        if (!j["currentPageSide"].empty())
        {
            auto s{j["currentPageSide"].get<string>()};

            if (s.starts_with("https://"))
                wv2side::wvBrowser->Navigate((to_wide(s)).c_str());

            else
                wv2side::wvBrowser->Navigate((L"https://" + to_wide(s)).c_str());

            wv2side::wvController->MoveFocus(
                COREWEBVIEW2_MOVE_FOCUS_REASON::COREWEBVIEW2_MOVE_FOCUS_REASON_PROGRAMMATIC);
        }

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

    wv2main::wvBrowser->Navigate((to_wide(settings.homepageMain)).c_str());
    wv2side::wvBrowser->Navigate((to_wide(settings.homepageSide)).c_str());
}

void Browser::NavigateMain(Settings& settings)
{
    if (!wv2main::wvBrowser)
        return;

    auto args{command_line()};

    if (!args.first.empty())
    {
        if (args.first.starts_with(L"https://") || args.first.starts_with(L"http://"))
            wv2main::wvBrowser->Navigate(args.first.c_str());

        else
            wv2main::wvBrowser->Navigate((L"https://" + args.first).c_str());
        return;
    }

    if (settings.homepageMain == "")
    {
#ifdef _DEBUG
        wv2main::wvBrowser->Navigate(L"https://localhost:8000/");
#else
        wv2main::wvBrowser->Navigate(path_home().wstring().c_str());
#endif
        return;
    }

    wv2main::wvBrowser->Navigate((to_wide(settings.homepageMain)).c_str());
}

void Browser::NavigateSide(Settings& settings)
{
    if (!wv2side::wvBrowser)
        return;

    auto args{command_line()};

    if (!args.second.empty())
    {
        if (args.second.starts_with(L"https://") || args.second.starts_with(L"http://"))
            wv2side::wvBrowser->Navigate(args.second.c_str());

        else
            wv2side::wvBrowser->Navigate((L"https://" + args.second).c_str());
        return;
    }

    if (settings.homepageSide == "")
    {
#ifdef _DEBUG
        wv2side::wvBrowser->Navigate(L"https://localhost:8000/");
#else
        wv2side::wvBrowser->Navigate(path_home().wstring().c_str());
#endif
        return;
    }

    wv2side::wvBrowser->Navigate((to_wide(settings.homepageSide)).c_str());
}

void Browser::FocusBar()
{
    wv2bar::wvController->MoveFocus(
        COREWEBVIEW2_MOVE_FOCUS_REASON::COREWEBVIEW2_MOVE_FOCUS_REASON_PROGRAMMATIC);
}

void Browser::FocusSettings()
{
    wv2settings::wvController->MoveFocus(
        COREWEBVIEW2_MOVE_FOCUS_REASON::COREWEBVIEW2_MOVE_FOCUS_REASON_PROGRAMMATIC);
}

void Browser::EscapeFocusBar(Settings& settings)
{
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
