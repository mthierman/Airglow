#include "WebView.hxx"

Config* WebView::pConfig{nullptr};

WebView::WebView(Config* config) {}

std::unique_ptr<WebView> WebView::Create(Config* config)
{
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
                                controller->put_Bounds(webView->MenuBounds());

                                browser->Navigate(webView->SettingsNavigation().c_str());

                                browser->ExecuteScript(webView->GetMenuScript().c_str(), nullptr);
                                browser->AddScriptToExecuteOnDocumentCreated(
                                    webView->GetMenuScript().c_str(), nullptr);

                                browser->add_DocumentTitleChanged(
                                    Callback<ICoreWebView2DocumentTitleChangedEventHandler>(
                                        [&](ICoreWebView2* sender, IUnknown* args) -> HRESULT
                                        {
                                            webView->SetWindowTitle();

                                            return S_OK;
                                        })
                                        .Get(),
                                    &tokenTitle);

                                browser->add_FaviconChanged(
                                    Callback<ICoreWebView2FaviconChangedEventHandler>(
                                        [&](ICoreWebView2* sender, IUnknown* args) -> HRESULT
                                        {
                                            webView->SetWindowIcon();

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
                                            if (webView->VerifySettingsUrl(args))
                                                webView->Messages(args);

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
                                settings->put_AreDefaultContextMenusEnabled(true);
                                settings->put_AreDefaultScriptDialogsEnabled(true);
                                settings->put_AreDevToolsEnabled(true);
                                settings->put_AreHostObjectsAllowed(true);
                                settings->put_IsBuiltInErrorPageEnabled(true);
                                settings->put_IsScriptEnabled(true);
                                settings->put_IsStatusBarEnabled(true);
                                settings->put_IsWebMessageEnabled(true);
                                settings->put_IsZoomControlEnabled(false);
                            }

                            if (browser)
                            {
                                controller->put_Bounds(webView->MainBounds());

                                browser->Navigate(webView->MainNavigation().c_str());

                                browser->ExecuteScript(webView->GetScript().c_str(), nullptr);
                                browser->AddScriptToExecuteOnDocumentCreated(
                                    webView->GetScript().c_str(), nullptr);

                                browser->add_DocumentTitleChanged(
                                    Callback<ICoreWebView2DocumentTitleChangedEventHandler>(
                                        [&](ICoreWebView2* sender, IUnknown* args) -> HRESULT
                                        {
                                            webView->SetWindowTitle();

                                            return S_OK;
                                        })
                                        .Get(),
                                    &tokenTitle);

                                browser->add_FaviconChanged(
                                    Callback<ICoreWebView2FaviconChangedEventHandler>(
                                        [&](ICoreWebView2* sender, IUnknown* args) -> HRESULT
                                        {
                                            webView->SetWindowIcon();

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
                                            webView->Messages(args);

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
                                settings->put_AreDefaultContextMenusEnabled(true);
                                settings->put_AreDefaultScriptDialogsEnabled(true);
                                settings->put_AreDevToolsEnabled(true);
                                settings->put_AreHostObjectsAllowed(true);
                                settings->put_IsBuiltInErrorPageEnabled(true);
                                settings->put_IsScriptEnabled(true);
                                settings->put_IsStatusBarEnabled(true);
                                settings->put_IsWebMessageEnabled(true);
                                settings->put_IsZoomControlEnabled(false);
                            }

                            if (browser)
                            {
                                controller->put_Bounds(webView->SideBounds());

                                browser->Navigate(webView->SideNavigation().c_str());

                                browser->ExecuteScript(webView->GetScript().c_str(), nullptr);
                                browser->AddScriptToExecuteOnDocumentCreated(
                                    webView->GetScript().c_str(), nullptr);

                                browser->add_DocumentTitleChanged(
                                    Callback<ICoreWebView2DocumentTitleChangedEventHandler>(
                                        [&](ICoreWebView2* sender, IUnknown* args) -> HRESULT
                                        {
                                            webView->SetWindowTitle();

                                            return S_OK;
                                        })
                                        .Get(),
                                    &tokenTitle);

                                browser->add_FaviconChanged(
                                    Callback<ICoreWebView2FaviconChangedEventHandler>(
                                        [&](ICoreWebView2* sender, IUnknown* args) -> HRESULT
                                        {
                                            webView->SetWindowIcon();

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
                                            webView->Messages(args);

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

void WebView::Messages(ICoreWebView2WebMessageReceivedEventArgs* args)
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
        if (!pConfig)
            return;

        auto message = to_string(wstring(messageRaw.get()));

        if (message.compare(0, 8, "mainUrl ") == 0)
        {
#ifdef _DEBUG
            println("mainUrl (WebView)");
#endif
            pConfig->settings.mainUrl = message.substr(8);
        }

        if (message.compare(0, 8, "sideUrl ") == 0)
        {
#ifdef _DEBUG
            println("sideUrl (WebView)");
#endif
            pConfig->settings.sideUrl = message.substr(8);
        }

        if (message == splitKey)
        {
#ifdef _DEBUG
            println("F1 (WebView)");
#endif
            SendMessageW(pConfig->hwnd, WM_KEYDOWN, VK_F1, 0);
        }

        if (message == swapKey)
        {
#ifdef _DEBUG
            println("F2 (WebView)");
#endif
            SendMessageW(pConfig->hwnd, WM_KEYDOWN, VK_F2, 0);
        }

        if (message == hideMenuKey)
        {
#ifdef _DEBUG
            println("F4 (WebView)");
#endif
            SendMessageW(pConfig->hwnd, WM_KEYDOWN, VK_F4, 0);
        }

        if (message == maximizeKey)
        {
#ifdef _DEBUG
            println("F6 (WebView)");
#endif
            SendMessageW(pConfig->hwnd, WM_KEYDOWN, VK_F6, 0);
        }

        if (message == fullscreenKey)
        {
#ifdef _DEBUG
            println("F11 (WebView)");
#endif
            SendMessageW(pConfig->hwnd, WM_KEYDOWN, VK_F11, 0);
        }

        if (message == onTopKey)
        {
#ifdef _DEBUG
            println("F9 (WebView)");
#endif
            SendMessageW(pConfig->hwnd, WM_KEYDOWN, VK_F9, 0);
        }

        if (message == closeKey)
        {
#ifdef _DEBUG
            println("Ctrl+W (WebView)");
#endif
            SendMessageW(pConfig->hwnd, WM_KEYDOWN, 0x57, 0);
        }
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

void WebView::SetWindowTitle()
{
    if (!pConfig || !Browsers::Settings::controller || !Browsers::Main::controller ||
        !Browsers::Side::controller)
        return;

    if (pConfig->settings.menu)
    {
        wil::unique_cotaskmem_string s{};
        Browsers::Settings::browser->get_DocumentTitle(&s);
        auto title = s.get();

        if (!pConfig->settings.topmost)
            SetWindowTextW(pConfig->hwnd, title);

        if (pConfig->settings.topmost)
        {
            wstring add = title + wstring(L" [On Top]");
            SetWindowTextW(pConfig->hwnd, add.c_str());
        }
    }

    if (!pConfig->settings.menu && !pConfig->settings.swapped)
    {
        wil::unique_cotaskmem_string s{};
        Browsers::Main::browser->get_DocumentTitle(&s);
        auto title = s.get();

        if (!pConfig->settings.topmost)
            SetWindowTextW(pConfig->hwnd, title);

        if (pConfig->settings.topmost)
        {
            wstring add = title + wstring(L" [On Top]");
            SetWindowTextW(pConfig->hwnd, add.c_str());
        }
    }

    if (!pConfig->settings.menu && pConfig->settings.swapped)
    {
        wil::unique_cotaskmem_string s{};
        Browsers::Side::browser->get_DocumentTitle(&s);
        auto title = s.get();

        if (!pConfig->settings.topmost)
            SetWindowTextW(pConfig->hwnd, title);

        if (pConfig->settings.topmost)
        {
            wstring add = title + wstring(L" [On Top]");
            SetWindowTextW(pConfig->hwnd, add.c_str());
        }
    }
}

void WebView::SetWindowIcon()
{

    if (!pConfig || !Browsers::Settings::controller || !Browsers::Main::controller ||
        !Browsers::Side::controller)
        return;

    if (pConfig->settings.menu)
    {
#ifdef _DEBUG
        LPWSTR faviconUri;
        Browsers::Settings::browser->get_FaviconUri(&faviconUri);
        wprintln(wstring(faviconUri));
#endif
        Browsers::Settings::browser->GetFavicon(
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
                            SendMessageW(pConfig->hwnd, WM_SETICON, ICON_SMALL,
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
        Browsers::Main::browser->get_FaviconUri(&faviconUri);
        wprintln(wstring(faviconUri));
#endif
        Browsers::Main::browser->GetFavicon(
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
                            SendMessageW(pConfig->hwnd, WM_SETICON, ICON_SMALL,
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
        Browsers::Side::browser->get_FaviconUri(&faviconUri);
        wprintln(wstring(faviconUri));
#endif
        Browsers::Side::browser->GetFavicon(
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
                            SendMessageW(pConfig->hwnd, WM_SETICON, ICON_SMALL,
                                         (LPARAM)favicon.get());
                        }
                    }
                    return S_OK;
                })
                .Get());
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

RECT WebView::FullBounds() { return get_rect(pConfig->hwnd); }

RECT WebView::MenuBounds()
{
    if (!pConfig->settings.menu)
        return RECT{0, 0, 0, 0};

    return get_rect(pConfig->hwnd);
}

RECT WebView::MainBounds()
{
    auto bounds{get_rect(pConfig->hwnd)};

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

    if (pConfig->settings.menu)
        return RECT{0, 0, 0, 0};

    return bounds;
}

RECT WebView::SideBounds()
{
    auto bounds{get_rect(pConfig->hwnd)};

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

    if (pConfig->settings.menu)
        return RECT{0, 0, 0, 0};

    return bounds;
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
    return wstring{LR"(
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
}

wstring WebView::GetMenuScript()
{
    return wstring{LR"(
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
}
