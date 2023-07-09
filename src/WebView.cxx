#include "WebView.hxx"

static wil::com_ptr<ICoreWebView2Controller> settings_controller;
static wil::com_ptr<ICoreWebView2> settings_core;
static wil::com_ptr<ICoreWebView2_19> settings_wv;
static wil::com_ptr<ICoreWebView2Settings> settings_settings;

static wil::com_ptr<ICoreWebView2Controller> main_controller;
static wil::com_ptr<ICoreWebView2> main_core;
static wil::com_ptr<ICoreWebView2_19> main_wv;
static wil::com_ptr<ICoreWebView2Settings> main_settings;

static wil::com_ptr<ICoreWebView2Controller> side_controller;
static wil::com_ptr<ICoreWebView2> side_core;
static wil::com_ptr<ICoreWebView2_19> side_wv;
static wil::com_ptr<ICoreWebView2Settings> side_settings;

Config* WebView::pConfig{nullptr};

WebView::WebView(Config* config) {}

std::unique_ptr<WebView> WebView::Create(Config* config)
{
    auto webView{std::unique_ptr<WebView>(new WebView(config))};

    webView->pConfig = config;

    HWND hwnd = webView->pConfig->hwnd;

    CreateCoreWebView2EnvironmentWithOptions(
        nullptr, config->paths.data.c_str(), nullptr,
        Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [hwnd](HRESULT result, ICoreWebView2Environment* env) -> HRESULT
            {
                // SETTINGS WEBVIEW
                env->CreateCoreWebView2Controller(
                    hwnd, Microsoft::WRL::Callback<
                              ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                              [](HRESULT result, ICoreWebView2Controller* controller) -> HRESULT
                              {
                                  EventRegistrationToken msgToken;
                                  EventRegistrationToken faviconChangedToken;
                                  EventRegistrationToken documentTitleChangedToken;

                                  if (controller)
                                  {
                                      settings_controller = controller;
                                      settings_controller->get_CoreWebView2(&settings_core);
                                  }

                                  if (settings_core)
                                      settings_wv = settings_core.try_query<ICoreWebView2_19>();
                                  if (settings_wv)
                                      settings_wv->get_Settings(&settings_settings);
                                  if (settings_settings)
                                  {
                                      settings_settings->put_AreDefaultContextMenusEnabled(false);
                                      settings_settings->put_AreDefaultScriptDialogsEnabled(true);
                                      settings_settings->put_AreDevToolsEnabled(true);
                                      settings_settings->put_AreHostObjectsAllowed(true);
                                      settings_settings->put_IsBuiltInErrorPageEnabled(true);
                                      settings_settings->put_IsScriptEnabled(true);
                                      settings_settings->put_IsStatusBarEnabled(false);
                                      settings_settings->put_IsWebMessageEnabled(true);
                                      settings_settings->put_IsZoomControlEnabled(false);
                                  }

                                  if (settings_wv)
                                  {
                                      settings_controller->put_Bounds(MenuBounds());
                                      settings_wv->Navigate(L"about:blank");
#ifdef _DEBUG
                                      settings_wv->Navigate(L"https://localhost:8000/");
#endif

                                      auto script = GetMenuScript();
                                      settings_wv->ExecuteScript(script.c_str(), nullptr);
                                      settings_wv->AddScriptToExecuteOnDocumentCreated(
                                          script.c_str(), nullptr);

                                      settings_wv->add_DocumentTitleChanged(
                                          Microsoft::WRL::Callback<
                                              ICoreWebView2DocumentTitleChangedEventHandler>(
                                              [](ICoreWebView2* sender, IUnknown* args) -> HRESULT
                                              {
                                                  SetWindowTitle();
                                                  return S_OK;
                                              })
                                              .Get(),
                                          &documentTitleChangedToken);

                                      settings_wv->add_FaviconChanged(
                                          Microsoft::WRL::Callback<
                                              ICoreWebView2FaviconChangedEventHandler>(
                                              [](ICoreWebView2* sender, IUnknown* args) -> HRESULT
                                              {
                                                  SetWindowIcon();
                                                  return S_OK;
                                              })
                                              .Get(),
                                          &faviconChangedToken);

                                      settings_wv->add_WebMessageReceived(
                                          Microsoft::WRL::Callback<
                                              ICoreWebView2WebMessageReceivedEventHandler>(
                                              [](ICoreWebView2* webview,
                                                 ICoreWebView2WebMessageReceivedEventArgs* args)
                                                  -> HRESULT
                                              {
                                                  wil::unique_cotaskmem_string uri;
                                                  args->get_Source(&uri);
                                                  wstring sourceUri = uri.get();
                                                  wstring verifyUri = L"about:blank";
#ifdef _DEBUG
                                                  verifyUri = L"https://localhost:8000/";
#endif

                                                  if (sourceUri != verifyUri)
                                                      return S_OK;

                                                  wil::unique_cotaskmem_string messageRaw;
                                                  if (SUCCEEDED(args->TryGetWebMessageAsString(
                                                          &messageRaw)))
                                                  {
                                                      auto message = wstring(messageRaw.get());

                                                      if (message.compare(0, 8, L"mainUrl ") == 0)
                                                      {
                                                          wprintln(message.substr(8));
                                                          pConfig->settings.mainUrl =
                                                              to_string(message.substr(8));
                                                      }

                                                      if (message.compare(0, 8, L"sideUrl ") == 0)
                                                      {
                                                          wprintln(message.substr(8));
                                                          pConfig->settings.sideUrl =
                                                              to_string(message.substr(8));
                                                      }

                                                      Messages(message);

                                                      pConfig->Save();
                                                  }

                                                  return S_OK;
                                              })
                                              .Get(),
                                          &msgToken);
                                  }

                                  return S_OK;
                              })
                              .Get());

                // MAIN WEBVIEW
                env->CreateCoreWebView2Controller(
                    hwnd,
                    Microsoft::WRL::Callback<
                        ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                        [](HRESULT result, ICoreWebView2Controller* controller) -> HRESULT
                        {
                            EventRegistrationToken msgToken;
                            EventRegistrationToken faviconChangedToken;
                            EventRegistrationToken documentTitleChangedToken;

                            if (controller)
                            {
                                main_controller = controller;
                                main_controller->get_CoreWebView2(&main_core);
                            }

                            if (main_core)
                                main_wv = main_core.try_query<ICoreWebView2_19>();
                            if (main_wv)
                                main_wv->get_Settings(&main_settings);
                            if (main_settings)
                            {
                                main_settings->put_AreDefaultContextMenusEnabled(true);
                                main_settings->put_AreDefaultScriptDialogsEnabled(true);
                                main_settings->put_AreDevToolsEnabled(true);
                                main_settings->put_AreHostObjectsAllowed(true);
                                main_settings->put_IsBuiltInErrorPageEnabled(true);
                                main_settings->put_IsScriptEnabled(true);
                                main_settings->put_IsStatusBarEnabled(true);
                                main_settings->put_IsWebMessageEnabled(true);
                                main_settings->put_IsZoomControlEnabled(true);
                            }

                            if (main_wv)
                            {

                                main_controller->put_Bounds(MainBounds());

                                auto args = CommandLine();

                                if (!args.first.empty())
                                {
                                    main_wv->Navigate(args.first.c_str());
                                }
                                else
                                    main_wv->Navigate(to_wide(pConfig->settings.mainUrl).c_str());

                                auto script = GetScript();
                                main_wv->ExecuteScript(script.c_str(), nullptr);
                                main_wv->AddScriptToExecuteOnDocumentCreated(script.c_str(),
                                                                             nullptr);

                                main_wv->add_DocumentTitleChanged(
                                    Microsoft::WRL::Callback<
                                        ICoreWebView2DocumentTitleChangedEventHandler>(
                                        [](ICoreWebView2* sender, IUnknown* args) -> HRESULT
                                        {
                                            SetWindowTitle();
                                            return S_OK;
                                        })
                                        .Get(),
                                    &documentTitleChangedToken);

                                main_wv->add_FaviconChanged(
                                    Microsoft::WRL::Callback<
                                        ICoreWebView2FaviconChangedEventHandler>(
                                        [](ICoreWebView2* sender, IUnknown* args) -> HRESULT
                                        {
                                            SetWindowIcon();
                                            return S_OK;
                                        })
                                        .Get(),
                                    &faviconChangedToken);

                                main_wv->add_WebMessageReceived(
                                    Microsoft::WRL::Callback<
                                        ICoreWebView2WebMessageReceivedEventHandler>(
                                        [](ICoreWebView2* webview,
                                           ICoreWebView2WebMessageReceivedEventArgs* args)
                                            -> HRESULT
                                        {
                                            wil::unique_cotaskmem_string messageRaw;
                                            if (SUCCEEDED(
                                                    args->TryGetWebMessageAsString(&messageRaw)))
                                            {
                                                auto message = wstring(messageRaw.get());
                                                Messages(message);
                                            }

                                            return S_OK;
                                        })
                                        .Get(),
                                    &msgToken);
                            }

                            return S_OK;
                        })
                        .Get());

                // SIDE WEBVIEW
                env->CreateCoreWebView2Controller(
                    hwnd,
                    Microsoft::WRL::Callback<
                        ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                        [](HRESULT result, ICoreWebView2Controller* controller) -> HRESULT
                        {
                            EventRegistrationToken msgToken;
                            EventRegistrationToken faviconChangedToken;
                            EventRegistrationToken documentTitleChangedToken;

                            if (controller)
                            {
                                side_controller = controller;
                                side_controller->get_CoreWebView2(&side_core);
                            }

                            if (side_core)
                                side_wv = side_core.try_query<ICoreWebView2_19>();
                            if (side_wv)
                                side_wv->get_Settings(&side_settings);
                            if (side_settings)
                            {
                                side_settings->put_AreDefaultContextMenusEnabled(true);
                                side_settings->put_AreDefaultScriptDialogsEnabled(true);
                                side_settings->put_AreDevToolsEnabled(true);
                                side_settings->put_AreHostObjectsAllowed(true);
                                side_settings->put_IsBuiltInErrorPageEnabled(true);
                                side_settings->put_IsScriptEnabled(true);
                                side_settings->put_IsStatusBarEnabled(true);
                                side_settings->put_IsWebMessageEnabled(true);
                                side_settings->put_IsZoomControlEnabled(true);
                            }

                            if (side_wv)
                            {

                                side_controller->put_Bounds(SideBounds());

                                auto args = CommandLine();

                                if (!args.second.empty())
                                {
                                    side_wv->Navigate(args.second.c_str());
                                }
                                else
                                    side_wv->Navigate(to_wide(pConfig->settings.sideUrl).c_str());

                                auto script = GetScript();
                                side_wv->ExecuteScript(script.c_str(), nullptr);
                                side_wv->AddScriptToExecuteOnDocumentCreated(script.c_str(),
                                                                             nullptr);

                                side_wv->add_DocumentTitleChanged(
                                    Microsoft::WRL::Callback<
                                        ICoreWebView2DocumentTitleChangedEventHandler>(
                                        [](ICoreWebView2* sender, IUnknown* args) -> HRESULT
                                        {
                                            SetWindowTitle();
                                            return S_OK;
                                        })
                                        .Get(),
                                    &documentTitleChangedToken);

                                side_wv->add_FaviconChanged(
                                    Microsoft::WRL::Callback<
                                        ICoreWebView2FaviconChangedEventHandler>(
                                        [](ICoreWebView2* sender, IUnknown* args) -> HRESULT
                                        {
                                            SetWindowIcon();
                                            return S_OK;
                                        })
                                        .Get(),
                                    &faviconChangedToken);

                                side_wv->add_WebMessageReceived(
                                    Microsoft::WRL::Callback<
                                        ICoreWebView2WebMessageReceivedEventHandler>(
                                        [](ICoreWebView2* webview,
                                           ICoreWebView2WebMessageReceivedEventArgs* args)
                                            -> HRESULT
                                        {
                                            wil::unique_cotaskmem_string messageRaw;
                                            if (SUCCEEDED(
                                                    args->TryGetWebMessageAsString(&messageRaw)))
                                            {
                                                auto message = wstring(messageRaw.get());
                                                Messages(message);
                                            }

                                            return S_OK;
                                        })
                                        .Get(),
                                    &msgToken);
                            }

                            return S_OK;
                        })
                        .Get());

                return S_OK;
            })
            .Get());

    return webView;
}

std::pair<wstring, wstring> WebView::CommandLine()
{
    std::pair<wstring, wstring> commands;

    auto cmd = GetCommandLineW();
    int number;

    auto args = CommandLineToArgvW(cmd, &number);

    if (number == 2)
    {
        commands.first = args[1];
        commands.second = args[1];
    }

    if (number == 3)
    {
        commands.first = args[1];
        commands.second = args[2];
    }

    LocalFree(args);

    if (!commands.first.empty())
    {
        if (!commands.first.starts_with(L"http") || !commands.first.starts_with(L"https"))
            commands.first = L"https://" + commands.first;
    }

    if (!commands.second.empty())
    {
        if (!commands.second.starts_with(L"http") || !commands.second.starts_with(L"https"))
            commands.second = L"https://" + commands.second;
    }

    return commands;
}

wstring WebView::GetScriptFile()
{
    stringstream buffer{};
    wstring script{};

    path file = (pConfig->paths.data.wstring() + path::preferred_separator + L"Airglow.js");

    if (std::filesystem::exists(file))
    {
        ifstream f(file);
        if (!std::filesystem::is_empty(file))
        {
            buffer << f.rdbuf();
            script = to_wide(buffer.str());
        }
        f.close();
    }

    return script;
}

wstring WebView::GetScript()
{
    wstring script{LR"(
        document.onreadystatechange = () => {
            if (document.readyState === "interactive") {
                let scheme = document.createElement("meta");
                scheme.setAttribute("name", "color-scheme");
                scheme.setAttribute("content", "light dark");
                document.getElementsByTagName("head")[0].appendChild(scheme);
                document.documentElement.style.setProperty(
                    "color-scheme",
                    "light dark"
                );
            }
            if (document.readyState === "complete") {
                onkeydown = (e) => {
                    if (e.ctrlKey && e.key === "w") {
                        window.chrome.webview.postMessage("close");
                    } else {
                        window.chrome.webview.postMessage(e.key);
                    }
                };
            }
        };
    )"};

    return script;
}

wstring WebView::GetMenuScript()
{
    wstring script{LR"(
        document.onreadystatechange = () => {
            if (document.readyState === "interactive") {
                let scheme = document.createElement("meta");
                scheme.setAttribute("name", "color-scheme");
                scheme.setAttribute("content", "light dark");
                document.getElementsByTagName("head")[0].appendChild(scheme);
                document.documentElement.style.setProperty(
                    "color-scheme",
                    "light dark"
                );
            }
            if (document.readyState === "complete") {
                onkeydown = (e) => {
                    if (e.key === "F3") {
                        e.preventDefault();
                    }
                    if (e.ctrlKey && e.key === "w") {
                        window.chrome.webview.postMessage("close");
                    } else {
                        window.chrome.webview.postMessage(e.key);
                    }
                };
            }
        };
    )"};

    return script;
}

void WebView::Messages(wstring message)
{
    HWND hwnd = pConfig->hwnd;
    wstring splitKey{L"F1"};
    wstring swapKey{L"F2"};
    wstring hideMenuKey{L"F4"};
    wstring maximizeKey{L"F6"};
    wstring fullscreenKey{L"F11"};
    wstring onTopKey{L"F9"};
    wstring closeKey{L"close"};

    if (message == splitKey)
    {
#ifdef _DEBUG
        println("F1 (WebView)");
#endif
        pConfig->settings.split = bool_toggle(pConfig->settings.split);
        WebView::UpdateBounds();
        WebView::UpdateFocus();
        WebView::SetWindowTitle();
        WebView::SetWindowIcon();
        pConfig->Save();
    }

    if (message == swapKey)
    {
#ifdef _DEBUG
        println("F2 (WebView)");
#endif
        pConfig->settings.swapped = bool_toggle(pConfig->settings.swapped);
        WebView::UpdateBounds();
        WebView::UpdateFocus();
        WebView::SetWindowTitle();
        WebView::SetWindowIcon();
        pConfig->Save();
    }

    if (message == hideMenuKey)
    {
#ifdef _DEBUG
        println("F4 (WebView)");
#endif
        pConfig->settings.menu = bool_toggle(pConfig->settings.menu);
        WebView::UpdateBounds();
        WebView::UpdateFocus();
        WebView::SetWindowTitle();
        WebView::SetWindowIcon();
        pConfig->Save();
    }

    if (message == maximizeKey)
    {
#ifdef _DEBUG
        println("F6 (WebView)");
#endif
        if (!pConfig->settings.fullscreen)
        {
            WINDOWPLACEMENT wp{sizeof(WINDOWPLACEMENT)};
            GetWindowPlacement(hwnd, &wp);
            if (wp.showCmd == 3)
            {
                ShowWindow(hwnd, SW_SHOWNORMAL);
                SetWindowPos(hwnd, nullptr, pConfig->settings.position[0],
                             pConfig->settings.position[1], pConfig->settings.position[2],
                             pConfig->settings.position[3], 0);
            }

            else
                ShowWindow(hwnd, SW_MAXIMIZE);
        }
    }

    if (message == fullscreenKey)
    {
#ifdef _DEBUG
        println("F11 (WebView)");
#endif
        pConfig->settings.fullscreen = bool_toggle(pConfig->settings.fullscreen);
        Fullscreen();
        WebView::UpdateBounds();
        pConfig->Save();
    }

    if (message == onTopKey)
    {
#ifdef _DEBUG
        println("F9 (WebView)");
#endif
        pConfig->settings.topmost = bool_toggle(pConfig->settings.topmost);
        Topmost();
        WebView::SetWindowTitle();
        pConfig->Save();
    }

    if (message == closeKey)
    {
#ifdef _DEBUG
        println("Ctrl+W (WebView)");
#endif
        PostMessageW(hwnd, WM_CLOSE, 0, 0);
    }
}

void WebView::UpdateBounds()
{
    if (!settings_controller || !main_controller || !side_controller)
        return;

    settings_controller->put_Bounds(MenuBounds());
    main_controller->put_Bounds(MainBounds());
    side_controller->put_Bounds(SideBounds());
}

void WebView::UpdateFocus()
{
    if (!pConfig || !settings_controller || !main_controller || !side_controller)
        return;

    if (pConfig->settings.menu)
        settings_controller->MoveFocus(
            COREWEBVIEW2_MOVE_FOCUS_REASON::COREWEBVIEW2_MOVE_FOCUS_REASON_PROGRAMMATIC);

    if (!pConfig->settings.swapped & !pConfig->settings.menu)
        main_controller->MoveFocus(
            COREWEBVIEW2_MOVE_FOCUS_REASON::COREWEBVIEW2_MOVE_FOCUS_REASON_PROGRAMMATIC);

    if (pConfig->settings.swapped & !pConfig->settings.menu)
        side_controller->MoveFocus(
            COREWEBVIEW2_MOVE_FOCUS_REASON::COREWEBVIEW2_MOVE_FOCUS_REASON_PROGRAMMATIC);
}

RECT WebView::FullBounds()
{
    RECT bounds{0, 0, 0, 0};

    if (!pConfig)
        return bounds;

    HWND hwnd = pConfig->hwnd;

    GetClientRect(hwnd, &bounds);

    return bounds;
}

RECT WebView::MenuBounds()
{
    RECT bounds{0, 0, 0, 0};

    if (!pConfig || !pConfig->settings.menu)
        return bounds;

    HWND hwnd = pConfig->hwnd;

    if (GetClientRect(hwnd, &bounds))
    {
        return RECT{
            bounds.left,
            bounds.top,
            bounds.right,
            bounds.bottom,
        };
    }

    return bounds;
}

RECT WebView::MainBounds()
{
    RECT bounds{0, 0, 0, 0};

    if (!pConfig || pConfig->settings.menu ||
        (!pConfig->settings.split && pConfig->settings.swapped))
        return bounds;

    HWND hwnd = pConfig->hwnd;

    if (GetClientRect(hwnd, &bounds))
    {
        if (!pConfig->settings.split && !pConfig->settings.swapped)
            return bounds;

        if (pConfig->settings.split & !pConfig->settings.swapped)
        {
            return RECT{
                bounds.left,
                bounds.top,
                bounds.right / 2,
                bounds.bottom,
            };
        }

        if (pConfig->settings.split & pConfig->settings.swapped)
        {
            return RECT{
                bounds.right / 2,
                bounds.top,
                bounds.right,
                bounds.bottom,
            };
        }
    }

    return bounds;
}

RECT WebView::SideBounds()
{
    RECT bounds{0, 0, 0, 0};

    if (!pConfig || pConfig->settings.menu ||
        (!pConfig->settings.split && !pConfig->settings.swapped))
        return bounds;

    HWND hwnd = pConfig->hwnd;

    if (GetClientRect(hwnd, &bounds))
    {
        if (!pConfig->settings.split & pConfig->settings.swapped)
            return bounds;

        if (pConfig->settings.split & !pConfig->settings.swapped)
        {
            return RECT{
                bounds.right / 2,
                bounds.top,
                bounds.right,
                bounds.bottom,
            };
        }

        if (pConfig->settings.split & pConfig->settings.swapped)
        {
            return RECT{
                bounds.left,
                bounds.top,
                bounds.right / 2,
                bounds.bottom,
            };
        }
    }

    return bounds;
}

void WebView::Fullscreen()
{
    static RECT position;

    auto style = GetWindowLongPtrW(pConfig->hwnd, GWL_STYLE);
    if (style & WS_OVERLAPPEDWINDOW)
    {
        MONITORINFO mi = {sizeof(mi)};
        GetWindowRect(pConfig->hwnd, &position);
        if (GetMonitorInfoW(MonitorFromWindow(pConfig->hwnd, MONITOR_DEFAULTTONEAREST), &mi))
        {
            SetWindowLongPtrW(pConfig->hwnd, GWL_STYLE, style & ~WS_OVERLAPPEDWINDOW);
            SetWindowPos(pConfig->hwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top,
                         mi.rcMonitor.right - mi.rcMonitor.left,
                         mi.rcMonitor.bottom - mi.rcMonitor.top,
                         SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
        }
    }

    else
    {
        SetWindowLongPtrW(pConfig->hwnd, GWL_STYLE, style | WS_OVERLAPPEDWINDOW);
        SetWindowPos(pConfig->hwnd, nullptr, 0, 0, 0, 0,
                     SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
        SetWindowPos(pConfig->hwnd, nullptr, position.left, position.top,
                     (position.right - position.left), (position.bottom - position.top), 0);
    }
}

void WebView::Topmost()
{
    FLASHWINFO fwi{};
    fwi.cbSize = sizeof(FLASHWINFO);
    fwi.hwnd = pConfig->hwnd;
    fwi.dwFlags = FLASHW_CAPTION;
    fwi.uCount = 1;
    fwi.dwTimeout = 100;

    auto style = GetWindowLongPtrW(pConfig->hwnd, GWL_EXSTYLE);
    if (style & WS_EX_TOPMOST)
    {
        SetWindowPos(pConfig->hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        FlashWindowEx(&fwi);
    }

    else
    {
        SetWindowPos(pConfig->hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        FlashWindowEx(&fwi);
    }
}

void WebView::SetWindowTitle()
{
    if (!pConfig || !settings_wv || !main_wv || !side_wv)
        return;

    HWND hwnd = pConfig->hwnd;

    if (pConfig->settings.menu)
    {
        wil::unique_cotaskmem_string s{};
        settings_wv->get_DocumentTitle(&s);
        auto title = s.get();

        if (!pConfig->settings.topmost)
            SetWindowTextW(hwnd, s.get());

        if (pConfig->settings.topmost)
        {
            wstring add = title + wstring(L" [On Top]");
            SetWindowTextW(hwnd, add.c_str());
        }
    }

    if (!pConfig->settings.menu && !pConfig->settings.swapped)
    {
        wil::unique_cotaskmem_string s{};
        main_wv->get_DocumentTitle(&s);
        auto title = s.get();

        if (!pConfig->settings.topmost)
            SetWindowTextW(hwnd, title);

        if (pConfig->settings.topmost)
        {
            wstring add = title + wstring(L" [On Top]");
            SetWindowTextW(hwnd, add.c_str());
        }
    }

    if (!pConfig->settings.menu && pConfig->settings.swapped)
    {
        wil::unique_cotaskmem_string s{};
        side_wv->get_DocumentTitle(&s);
        auto title = s.get();

        if (!pConfig->settings.topmost)
            SetWindowTextW(hwnd, title);

        if (pConfig->settings.topmost)
        {
            wstring add = title + wstring(L" [On Top]");
            SetWindowTextW(hwnd, add.c_str());
        }
    }
}

void WebView::SetWindowIcon()
{
    if (!pConfig || !settings_wv || !main_wv || !side_wv)
        return;

    HWND hwnd = pConfig->hwnd;

    if (pConfig->settings.menu)
    {
#ifdef _DEBUG
        LPWSTR faviconUri;
        settings_wv->get_FaviconUri(&faviconUri);
        wprintln(wstring(faviconUri));
#endif

        settings_wv->GetFavicon(COREWEBVIEW2_FAVICON_IMAGE_FORMAT_PNG,
                                Microsoft::WRL::Callback<ICoreWebView2GetFaviconCompletedHandler>(
                                    [hwnd](HRESULT result, IStream* iconStream) -> HRESULT
                                    {
                                        if (iconStream)
                                        {
                                            Gdiplus::Bitmap iconBitmap(iconStream);
                                            wil::unique_hicon icon;
                                            if (iconBitmap.GetHICON(&icon) == Gdiplus::Status::Ok)
                                            {
                                                auto favicon = std::move(icon);
                                                SendMessageW(hwnd, WM_SETICON, ICON_BIG,
                                                             (LPARAM)favicon.get());
                                            }
                                        }
                                        return S_OK;
                                    })
                                    .Get());
    }

    if (!pConfig->settings.swapped && !pConfig->settings.menu)
    {

#ifdef _DEBUG
        LPWSTR faviconUri;
        main_wv->get_FaviconUri(&faviconUri);
        wprintln(wstring(faviconUri));
#endif

        main_wv->GetFavicon(COREWEBVIEW2_FAVICON_IMAGE_FORMAT_PNG,
                            Microsoft::WRL::Callback<ICoreWebView2GetFaviconCompletedHandler>(
                                [hwnd](HRESULT result, IStream* iconStream) -> HRESULT
                                {
                                    if (iconStream)
                                    {
                                        Gdiplus::Bitmap iconBitmap(iconStream);
                                        wil::unique_hicon icon;
                                        if (iconBitmap.GetHICON(&icon) == Gdiplus::Status::Ok)
                                        {
                                            auto favicon = std::move(icon);
                                            SendMessageW(hwnd, WM_SETICON, ICON_BIG,
                                                         (LPARAM)favicon.get());
                                        }
                                    }
                                    return S_OK;
                                })
                                .Get());
    }

    if (pConfig->settings.swapped && !pConfig->settings.menu)
    {
#ifdef _DEBUG
        LPWSTR faviconUri;
        side_wv->get_FaviconUri(&faviconUri);
        wprintln(wstring(faviconUri));
#endif

        side_wv->GetFavicon(COREWEBVIEW2_FAVICON_IMAGE_FORMAT_PNG,
                            Microsoft::WRL::Callback<ICoreWebView2GetFaviconCompletedHandler>(
                                [hwnd](HRESULT result, IStream* iconStream) -> HRESULT
                                {
                                    if (iconStream)
                                    {
                                        Gdiplus::Bitmap iconBitmap(iconStream);
                                        wil::unique_hicon icon;
                                        if (iconBitmap.GetHICON(&icon) == Gdiplus::Status::Ok)
                                        {
                                            auto favicon = std::move(icon);
                                            SendMessageW(hwnd, WM_SETICON, ICON_BIG,
                                                         (LPARAM)favicon.get());
                                        }
                                    }
                                    return S_OK;
                                })
                                .Get());
    }
}
