#include "WebView.hxx"

Config* WebView::pConfig{nullptr};

WebView::WebView(Config* config) {}

std::unique_ptr<WebView> WebView::Create(Config* config)
{
    using namespace Microsoft::WRL;
    auto webView{std::unique_ptr<WebView>(new WebView(config))};

    webView->pConfig = config;

    CreateCoreWebView2EnvironmentWithOptions(
        nullptr, pConfig->paths.data.c_str(), nullptr,
        Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [&](HRESULT result, ICoreWebView2Environment* e) -> HRESULT
            {
                // SETTINGS WEBVIEW
                e->CreateCoreWebView2Controller(
                    pConfig->hwnd,
                    Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                        [&](HRESULT result, ICoreWebView2Controller* c) -> HRESULT
                        {
                            using namespace Browsers::Settings;

                            EventRegistrationToken tokenTitle;
                            EventRegistrationToken tokenFavicon;
                            EventRegistrationToken tokenMsg;

                            if (c)
                            {
                                controller = c;
                                controller->get_CoreWebView2(&core);
                            }

                            if (core)
                                browser = core.try_query<ICoreWebView2_19>();

                            if (browser)
                                browser->get_Settings(&settings);

                            if (settings)
                            {
                                settings->put_AreDefaultContextMenusEnabled(false);
                                settings->put_AreDefaultScriptDialogsEnabled(true);
                                settings->put_AreDevToolsEnabled(true);
                                settings->put_AreHostObjectsAllowed(true);
                                settings->put_IsBuiltInErrorPageEnabled(true);
                                settings->put_IsScriptEnabled(true);
                                settings->put_IsStatusBarEnabled(false);
                                settings->put_IsWebMessageEnabled(true);
                                settings->put_IsZoomControlEnabled(false);
                            }

                            if (browser)
                            {
                                controller->put_Bounds(MenuBounds());
                                browser->Navigate(SettingsNavigation().c_str());
                                browser->ExecuteScript(GetMenuScript().c_str(), nullptr);
                                browser->AddScriptToExecuteOnDocumentCreated(
                                    GetMenuScript().c_str(), nullptr);

                                browser->add_DocumentTitleChanged(
                                    Callback<ICoreWebView2DocumentTitleChangedEventHandler>(
                                        [&](ICoreWebView2* sender, IUnknown* args) -> HRESULT
                                        {
                                            SetWindowTitle();

                                            return S_OK;
                                        })
                                        .Get(),
                                    &tokenTitle);

                                browser->add_FaviconChanged(
                                    Callback<ICoreWebView2FaviconChangedEventHandler>(
                                        [&](ICoreWebView2* sender, IUnknown* args) -> HRESULT
                                        {
                                            SetWindowIcon();

                                            return S_OK;
                                        })
                                        .Get(),
                                    &tokenFavicon);

                                browser->add_WebMessageReceived(
                                    Callback<ICoreWebView2WebMessageReceivedEventHandler>(
                                        [&](ICoreWebView2* sender,
                                            ICoreWebView2WebMessageReceivedEventArgs* args)
                                            -> HRESULT
                                        {
                                            if (VerifySettingsUrl(args))
                                                SettingsMessages(args);

                                            return S_OK;
                                        })
                                        .Get(),
                                    &tokenMsg);
                            }

                            return S_OK;
                        })
                        .Get());

                // MAIN WEBVIEW
                e->CreateCoreWebView2Controller(
                    pConfig->hwnd,
                    Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                        [&](HRESULT result, ICoreWebView2Controller* c) -> HRESULT
                        {
                            using namespace Browsers::Main;

                            EventRegistrationToken tokenTitle;
                            EventRegistrationToken tokenFavicon;
                            EventRegistrationToken tokenMsg;

                            if (c)
                            {
                                controller = c;
                                controller->get_CoreWebView2(&core);
                            }

                            if (core)
                                browser = core.try_query<ICoreWebView2_19>();

                            if (browser)
                                browser->get_Settings(&settings);

                            if (settings)
                            {
                                settings->put_AreDefaultContextMenusEnabled(false);
                                settings->put_AreDefaultScriptDialogsEnabled(true);
                                settings->put_AreDevToolsEnabled(true);
                                settings->put_AreHostObjectsAllowed(true);
                                settings->put_IsBuiltInErrorPageEnabled(true);
                                settings->put_IsScriptEnabled(true);
                                settings->put_IsStatusBarEnabled(false);
                                settings->put_IsWebMessageEnabled(true);
                                settings->put_IsZoomControlEnabled(false);
                            }

                            if (browser)
                            {
                                controller->put_Bounds(MainBounds());
                                browser->Navigate(MainNavigation().c_str());

                                auto script = GetScript();
                                browser->ExecuteScript(script.c_str(), nullptr);
                                browser->AddScriptToExecuteOnDocumentCreated(script.c_str(),
                                                                             nullptr);

                                browser->add_DocumentTitleChanged(
                                    Callback<ICoreWebView2DocumentTitleChangedEventHandler>(
                                        [&](ICoreWebView2* sender, IUnknown* args) -> HRESULT
                                        {
                                            SetWindowTitle();

                                            return S_OK;
                                        })
                                        .Get(),
                                    &tokenTitle);

                                browser->add_FaviconChanged(
                                    Callback<ICoreWebView2FaviconChangedEventHandler>(
                                        [&](ICoreWebView2* sender, IUnknown* args) -> HRESULT
                                        {
                                            SetWindowIcon();

                                            return S_OK;
                                        })
                                        .Get(),
                                    &tokenFavicon);

                                browser->add_WebMessageReceived(
                                    Callback<ICoreWebView2WebMessageReceivedEventHandler>(
                                        [&](ICoreWebView2* webview,
                                            ICoreWebView2WebMessageReceivedEventArgs* args)
                                            -> HRESULT
                                        {
                                            MainMessages(args);

                                            return S_OK;
                                        })
                                        .Get(),
                                    &tokenMsg);
                            }

                            return S_OK;
                        })
                        .Get());

                // SIDE WEBVIEW
                e->CreateCoreWebView2Controller(
                    pConfig->hwnd,
                    Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                        [&](HRESULT result, ICoreWebView2Controller* c) -> HRESULT
                        {
                            using namespace Browsers::Side;

                            EventRegistrationToken tokenTitle;
                            EventRegistrationToken tokenFavicon;
                            EventRegistrationToken tokenMsg;

                            if (c)
                            {
                                controller = c;
                                controller->get_CoreWebView2(&core);
                            }

                            if (core)
                                browser = core.try_query<ICoreWebView2_19>();

                            if (browser)
                                browser->get_Settings(&settings);

                            if (settings)
                            {
                                settings->put_AreDefaultContextMenusEnabled(false);
                                settings->put_AreDefaultScriptDialogsEnabled(true);
                                settings->put_AreDevToolsEnabled(true);
                                settings->put_AreHostObjectsAllowed(true);
                                settings->put_IsBuiltInErrorPageEnabled(true);
                                settings->put_IsScriptEnabled(true);
                                settings->put_IsStatusBarEnabled(false);
                                settings->put_IsWebMessageEnabled(true);
                                settings->put_IsZoomControlEnabled(false);
                            }

                            if (browser)
                            {
                                controller->put_Bounds(SideBounds());
                                browser->Navigate(SideNavigation().c_str());

                                auto script = GetScript();
                                browser->ExecuteScript(script.c_str(), nullptr);
                                browser->AddScriptToExecuteOnDocumentCreated(script.c_str(),
                                                                             nullptr);

                                browser->add_DocumentTitleChanged(
                                    Callback<ICoreWebView2DocumentTitleChangedEventHandler>(
                                        [&](ICoreWebView2* sender, IUnknown* args) -> HRESULT
                                        {
                                            SetWindowTitle();

                                            return S_OK;
                                        })
                                        .Get(),
                                    &tokenTitle);

                                browser->add_FaviconChanged(
                                    Callback<ICoreWebView2FaviconChangedEventHandler>(
                                        [&](ICoreWebView2* sender, IUnknown* args) -> HRESULT
                                        {
                                            SetWindowIcon();

                                            return S_OK;
                                        })
                                        .Get(),
                                    &tokenFavicon);

                                browser->add_WebMessageReceived(
                                    Callback<ICoreWebView2WebMessageReceivedEventHandler>(
                                        [&](ICoreWebView2* webview,
                                            ICoreWebView2WebMessageReceivedEventArgs* args)
                                            -> HRESULT
                                        {
                                            MainMessages(args);

                                            return S_OK;
                                        })
                                        .Get(),
                                    &tokenMsg);
                            }

                            return S_OK;
                        })
                        .Get());

                return S_OK;
            })
            .Get());

    return webView;
}

wstring WebView::MainNavigation()
{
    auto args = CommandLine();

    if (!args.first.empty())
    {
        return args.first;
    }

    return to_wide(pConfig->settings.mainUrl);
}

wstring WebView::SideNavigation()
{
    auto args = CommandLine();

    if (!args.second.empty())
    {
        return args.second;
    }

    return to_wide(pConfig->settings.sideUrl);
}

wstring WebView::SettingsNavigation()
{
#ifdef _DEBUG
    return L"https://localhost:8000/";
#endif

    return L"about:blank";
}

void WebView::MainMessages(ICoreWebView2WebMessageReceivedEventArgs* args)
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
                GetWindowPlacement(pConfig->hwnd, &wp);
                if (wp.showCmd == 3)
                {
                    ShowWindow(pConfig->hwnd, SW_SHOWNORMAL);
                    SetWindowPos(pConfig->hwnd, nullptr, pConfig->settings.position[0],
                                 pConfig->settings.position[1], pConfig->settings.position[2],
                                 pConfig->settings.position[3], 0);
                }

                else
                    ShowWindow(pConfig->hwnd, SW_MAXIMIZE);
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
            PostMessageW(pConfig->hwnd, WM_CLOSE, 0, 0);
        }
    }
}

bool WebView::VerifySettingsUrl(ICoreWebView2WebMessageReceivedEventArgs* args)
{
    wil::unique_cotaskmem_string uri;
    args->get_Source(&uri);
    wstring sourceUri = uri.get();
    wstring verifyUri = L"about:blank";
#ifdef _DEBUG
    verifyUri = L"https://localhost:8000/";
#endif
    if (sourceUri != verifyUri)
        return false;

    return true;
}

void WebView::SettingsMessages(ICoreWebView2WebMessageReceivedEventArgs* args)
{
    wil::unique_cotaskmem_string messageRaw;
    if (SUCCEEDED(args->TryGetWebMessageAsString(&messageRaw)))
    {
        auto message = wstring(messageRaw.get());

        if (message.compare(0, 8, L"mainUrl ") == 0)
        {
            wprintln(message.substr(8));
            pConfig->settings.mainUrl = to_string(message.substr(8));
        }

        if (message.compare(0, 8, L"sideUrl ") == 0)
        {
            wprintln(message.substr(8));
            pConfig->settings.sideUrl = to_string(message.substr(8));
        }

        Messages(message);

        pConfig->Save();
    }
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
    if (!Browsers::Settings::controller || !Browsers::Main::controller ||
        !Browsers::Side::controller)
        return;

    Browsers::Settings::controller->put_Bounds(MenuBounds());
    Browsers::Main::controller->put_Bounds(MainBounds());
    Browsers::Side::controller->put_Bounds(SideBounds());
}

void WebView::UpdateFocus()
{
    if (!pConfig || !Browsers::Settings::controller || !Browsers::Main::controller ||
        !Browsers::Side::controller)
        return;

    if (pConfig->settings.menu)
        Browsers::Settings::controller->MoveFocus(
            COREWEBVIEW2_MOVE_FOCUS_REASON::COREWEBVIEW2_MOVE_FOCUS_REASON_PROGRAMMATIC);

    if (!pConfig->settings.swapped & !pConfig->settings.menu)
        Browsers::Main::controller->MoveFocus(
            COREWEBVIEW2_MOVE_FOCUS_REASON::COREWEBVIEW2_MOVE_FOCUS_REASON_PROGRAMMATIC);

    if (pConfig->settings.swapped & !pConfig->settings.menu)
        Browsers::Side::controller->MoveFocus(
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
    if (!pConfig || !Browsers::Settings::controller || !Browsers::Main::controller ||
        !Browsers::Side::controller)
        return;

    HWND hwnd = pConfig->hwnd;

    if (pConfig->settings.menu)
    {
        SetWindowTextW(hwnd, L"Settings");
        // wil::unique_cotaskmem_string s{};
        // settings_wv->get_DocumentTitle(&s);
        // auto title = s.get();

        // if (!pConfig->settings.topmost)
        //     SetWindowTextW(hwnd, s.get());

        // if (pConfig->settings.topmost)
        // {
        //     wstring add = title + wstring(L" [On Top]");
        //     SetWindowTextW(hwnd, add.c_str());
        // }
    }

    if (!pConfig->settings.menu && !pConfig->settings.swapped)
    {
        wil::unique_cotaskmem_string s{};
        Browsers::Main::browser->get_DocumentTitle(&s);
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
        Browsers::Side::browser->get_DocumentTitle(&s);
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

    if (!pConfig || !Browsers::Settings::controller || !Browsers::Main::controller ||
        !Browsers::Side::controller)
        return;

    // wstring programIcon{L"PROGRAM_ICON"};
    // auto hIcon = (HICON)LoadImageW(pConfig->hinstance, L"PROGRAM_ICON", IMAGE_ICON, 0, 0,
    //                                LR_DEFAULTCOLOR | LR_DEFAULTSIZE | LR_SHARED);
    // auto hIconSm = (HICON)LoadImageW(pConfig->hinstance, programIcon.c_str(), IMAGE_ICON, 0, 0,
    //                                  LR_DEFAULTCOLOR | LR_DEFAULTSIZE | LR_SHARED);

    HWND hwnd = pConfig->hwnd;

    if (pConfig->settings.menu)
    {
#ifdef _DEBUG
        LPWSTR faviconUri;
        Browsers::Settings::browser->get_FaviconUri(&faviconUri);
        wprintln(wstring(faviconUri));
#endif

        SendMessageW(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)pConfig->hIcon);

        // settings_wv->GetFavicon(COREWEBVIEW2_FAVICON_IMAGE_FORMAT_PNG,
        //                         Microsoft::WRL::Callback<ICoreWebView2GetFaviconCompletedHandler>(
        //                             [hwnd](HRESULT result, IStream* iconStream) -> HRESULT
        //                             {
        //                                 if (iconStream)
        //                                 {
        //                                     Gdiplus::Bitmap iconBitmap(iconStream);
        //                                     wil::unique_hicon icon;
        //                                     if (iconBitmap.GetHICON(&icon) ==
        //                                     Gdiplus::Status::Ok)
        //                                     {
        //                                         auto favicon = std::move(icon);
        //                                         SendMessageW(hwnd, WM_SETICON, ICON_BIG,
        //                                                      (LPARAM)favicon.get());
        //                                     }
        //                                 }
        //                                 return S_OK;
        //                             })
        //                             .Get());
    }

    if (!pConfig->settings.swapped && !pConfig->settings.menu)
    {

#ifdef _DEBUG
        LPWSTR faviconUri;
        Browsers::Main::browser->get_FaviconUri(&faviconUri);
        wprintln(wstring(faviconUri));
#endif

        Browsers::Main::browser->GetFavicon(
            COREWEBVIEW2_FAVICON_IMAGE_FORMAT_PNG,
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
                            SendMessageW(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)favicon.get());
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
        Browsers::Side::browser->get_FaviconUri(&faviconUri);
        wprintln(wstring(faviconUri));
#endif

        Browsers::Side::browser->GetFavicon(
            COREWEBVIEW2_FAVICON_IMAGE_FORMAT_PNG,
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
                            SendMessageW(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)favicon.get());
                        }
                    }
                    return S_OK;
                })
                .Get());
    }
}
