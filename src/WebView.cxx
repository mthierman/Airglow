#include "WebView.hxx"

#ifndef DEBUG_MSG
// #define DEBUG_MSG
#endif

Config* WebView::pConfig{nullptr};

WebView::WebView(Config* config) {}

std::unique_ptr<WebView> WebView::Create(Config* config)
{
    if (!config)
        return nullptr;

    auto webView{std::unique_ptr<WebView>(new WebView(config))};

    webView->pConfig = config;

    CreateCoreWebView2EnvironmentWithOptions(
        nullptr, pConfig->paths.data.c_str(), nullptr,
        Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [&](HRESULT result, ICoreWebView2Environment* e) -> HRESULT
            {
                if (!e)
                    return E_POINTER;

                // SETTINGS WEBVIEW
                e->CreateCoreWebView2Controller(
                    pConfig->hwnd,
                    Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                        [&](HRESULT result, ICoreWebView2Controller* c) -> HRESULT
                        {
                            using namespace Browsers::Settings;

                            EventRegistrationToken tokenTitle;
                            EventRegistrationToken tokenFavicon;
                            EventRegistrationToken tokenReceivedMsg;
                            EventRegistrationToken tokenPostMsg;

                            if (!c)
                                return E_POINTER;

                            controller = c;
                            controller->get_CoreWebView2(&core);

                            if (!core)
                                return E_POINTER;

                            browser = core.try_query<ICoreWebView2_19>();

                            if (!browser)
                                return E_POINTER;

                            browser->get_Settings(&settings);

                            if (!settings)
                                return E_POINTER;

                            settings->put_AreDefaultContextMenusEnabled(false);
                            settings->put_AreDefaultScriptDialogsEnabled(true);
                            settings->put_AreDevToolsEnabled(true);
                            settings->put_AreHostObjectsAllowed(true);
                            settings->put_IsBuiltInErrorPageEnabled(true);
                            settings->put_IsScriptEnabled(true);
                            settings->put_IsStatusBarEnabled(false);
                            settings->put_IsWebMessageEnabled(true);
                            settings->put_IsZoomControlEnabled(false);

                            controller->put_Bounds(webView->SettingsBounds());

                            browser->SetVirtualHostNameToFolderMapping(
                                L"airglow", pConfig->paths.data.wstring().c_str(),
                                COREWEBVIEW2_HOST_RESOURCE_ACCESS_KIND_ALLOW);

                            browser->Navigate(webView->SettingsNavigation().c_str());

                            browser->AddScriptToExecuteOnDocumentCreated(
                                webView->GetScriptFile().c_str(), nullptr);

                            browser->add_NavigationCompleted(
                                Callback<ICoreWebView2NavigationCompletedEventHandler>(
                                    [&](ICoreWebView2* sender, IUnknown* args) -> HRESULT
                                    {
                                        webView->UpdateConfig();

                                        return S_OK;
                                    })
                                    .Get(),
                                &tokenPostMsg);

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
                                        ICoreWebView2WebMessageReceivedEventArgs* args) -> HRESULT
                                    {
                                        if (webView->VerifySettingsUrl(args))
                                            webView->Messages(args);
                                        pConfig->Save();

                                        return S_OK;
                                    })
                                    .Get(),
                                &tokenReceivedMsg);

                            browser->OpenDevToolsWindow();

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

                            if (!c)
                                return E_POINTER;

                            controller = c;
                            controller->get_CoreWebView2(&core);

                            if (!core)
                                return E_POINTER;

                            browser = core.try_query<ICoreWebView2_19>();

                            if (!browser)
                                return E_POINTER;

                            browser->get_Settings(&settings);

                            if (!settings)
                                return E_POINTER;

                            settings->put_AreDefaultContextMenusEnabled(true);
                            settings->put_AreDefaultScriptDialogsEnabled(true);
                            settings->put_AreDevToolsEnabled(true);
                            settings->put_AreHostObjectsAllowed(true);
                            settings->put_IsBuiltInErrorPageEnabled(true);
                            settings->put_IsScriptEnabled(true);
                            settings->put_IsStatusBarEnabled(true);
                            settings->put_IsWebMessageEnabled(true);
                            settings->put_IsZoomControlEnabled(false);

                            controller->put_Bounds(webView->MainBounds());

                            browser->Navigate(webView->MainNavigation().c_str());

                            browser->AddScriptToExecuteOnDocumentCreated(
                                webView->GetScriptFile().c_str(), nullptr);

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
                                        ICoreWebView2WebMessageReceivedEventArgs* args) -> HRESULT
                                    {
                                        webView->Messages(args);

                                        return S_OK;
                                    })
                                    .Get(),
                                &tokenMsg);

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

                            if (!c)
                                return E_POINTER;

                            controller = c;
                            controller->get_CoreWebView2(&core);

                            if (!core)
                                return E_POINTER;

                            browser = core.try_query<ICoreWebView2_19>();

                            if (!browser)
                                return E_POINTER;

                            browser->get_Settings(&settings);

                            if (!settings)
                                return E_POINTER;

                            settings->put_AreDefaultContextMenusEnabled(true);
                            settings->put_AreDefaultScriptDialogsEnabled(true);
                            settings->put_AreDevToolsEnabled(true);
                            settings->put_AreHostObjectsAllowed(true);
                            settings->put_IsBuiltInErrorPageEnabled(true);
                            settings->put_IsScriptEnabled(true);
                            settings->put_IsStatusBarEnabled(true);
                            settings->put_IsWebMessageEnabled(true);
                            settings->put_IsZoomControlEnabled(false);

                            controller->put_Bounds(webView->SideBounds());

                            browser->Navigate(webView->SideNavigation().c_str());

                            browser->AddScriptToExecuteOnDocumentCreated(
                                webView->GetScriptFile().c_str(), nullptr);

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
                                        ICoreWebView2WebMessageReceivedEventArgs* args) -> HRESULT
                                    {
                                        webView->Messages(args);

                                        return S_OK;
                                    })
                                    .Get(),
                                &tokenMsg);

                            return S_OK;
                        })
                        .Get());

                return S_OK;
            })
            .Get());

    return webView;
}

void WebView::UpdateBounds()
{
    if (!Browsers::Settings::controller || !Browsers::Main::controller ||
        !Browsers::Side::controller)
        return;

    Browsers::Settings::controller->put_Bounds(SettingsBounds());
    Browsers::Main::controller->put_Bounds(MainBounds());
    Browsers::Side::controller->put_Bounds(SideBounds());
}

void WebView::UpdateConfig()
{
    if (!pConfig || !Browsers::Settings::browser)
        return;

    json j{{"settings",
            {{"mainUrl", pConfig->settings.mainUrl},
             {"sideUrl", pConfig->settings.sideUrl},
             {"accentColor", pConfig->settings.accentColor}}}};

    Browsers::Settings::browser->PostWebMessageAsJson(to_wide(j.dump()).c_str());
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
        // #ifdef DEBUG_MSG
        //         LPWSTR faviconUri;
        //         Browsers::Settings::browser->get_FaviconUri(&faviconUri);
        //         wprintln(wstring(faviconUri));
        // #endif
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
                            SetClassLongPtrW(pConfig->hwnd, GCLP_HICONSM, (LONG_PTR)favicon.get());
                            SetClassLongPtrW(pConfig->hwnd, GCLP_HICON, (LONG_PTR)pConfig->hIcon);
                        }
                    }
                    return S_OK;
                })
                .Get());
    }

    if (!pConfig->settings.swapped && !pConfig->settings.menu)
    {
        // #ifdef DEBUG_MSG
        //         LPWSTR faviconUri;
        //         Browsers::Main::browser->get_FaviconUri(&faviconUri);
        //         wprintln(wstring(faviconUri));
        // #endif
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
                            SetClassLongPtrW(pConfig->hwnd, GCLP_HICONSM, (LONG_PTR)favicon.get());
                            SetClassLongPtrW(pConfig->hwnd, GCLP_HICON, (LONG_PTR)pConfig->hIcon);
                        }
                    }
                    return S_OK;
                })
                .Get());
    }

    if (pConfig->settings.swapped && !pConfig->settings.menu)
    {
        // #ifdef DEBUG_MSG
        //         LPWSTR faviconUri;
        //         Browsers::Side::browser->get_FaviconUri(&faviconUri);
        //         wprintln(wstring(faviconUri));
        // #endif
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
                            SetClassLongPtrW(pConfig->hwnd, GCLP_HICONSM, (LONG_PTR)favicon.get());
                            SetClassLongPtrW(pConfig->hwnd, GCLP_HICON, (LONG_PTR)pConfig->hIcon);
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
    int count;

    auto args = CommandLineToArgvW(cmd, &count);

    if (count == 2)
    {
        commands.first = args[1];
        commands.second = wstring{};
    }

    if (count == 3)
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

wstring WebView::MainNavigation()
{
    auto args = CommandLine();

    if (!args.first.empty())
    {
        return args.first;
    }

    return to_wide("https://" + pConfig->settings.mainUrl);
}

wstring WebView::SideNavigation()
{
    auto args = CommandLine();

    if (!args.second.empty())
    {
        return args.second;
    }

    return to_wide("https://" + pConfig->settings.sideUrl);
}

bool WebView::VerifySettingsUrl(ICoreWebView2WebMessageReceivedEventArgs* args)
{
    wstring verify{L"https://airglow/index.html"};
#ifdef _DEBUG
    verify = L"https://localhost:8000/";
#endif
    wil::unique_cotaskmem_string s{};
    args->get_Source(&s);
    wstring source = s.get();
    if (source != verify)
        return false;

    return true;
}

wstring WebView::SettingsNavigation()
{
#ifdef _DEBUG
    return L"https://localhost:8000/";
#endif
    return L"https://airglow/index.html";
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

#ifdef DEBUG_MSG
        println(message);
#endif

        if (message.compare(0, 8, "mainUrl ") == 0)
        {
#ifdef DEBUG_MSG
            println("mainUrl (WebView)");
#endif
            pConfig->settings.mainUrl = message.substr(8);
        }

        if (message.compare(0, 8, "sideUrl ") == 0)
        {
#ifdef DEBUG_MSG
            println("sideUrl (WebView)");
#endif
            pConfig->settings.sideUrl = message.substr(8);
        }

        if (message == splitKey)
        {
#ifdef DEBUG_MSG
            println("F1 (WebView)");
#endif
            SendMessageW(pConfig->hwnd, WM_KEYDOWN, VK_F1, 0);
        }

        if (message == swapKey)
        {
#ifdef DEBUG_MSG
            println("F2 (WebView)");
#endif
            SendMessageW(pConfig->hwnd, WM_KEYDOWN, VK_F2, 0);
        }

        if (message == hideMenuKey)
        {
#ifdef DEBUG_MSG
            println("F4 (WebView)");
#endif
            SendMessageW(pConfig->hwnd, WM_KEYDOWN, VK_F4, 0);
        }

        if (message == maximizeKey)
        {
#ifdef DEBUG_MSG
            println("F6 (WebView)");
#endif
            SendMessageW(pConfig->hwnd, WM_KEYDOWN, VK_F6, 0);
        }

        if (message == fullscreenKey)
        {
#ifdef DEBUG_MSG
            println("F11 (WebView)");
#endif
            SendMessageW(pConfig->hwnd, WM_KEYDOWN, VK_F11, 0);
        }

        if (message == onTopKey)
        {
#ifdef DEBUG_MSG
            println("F9 (WebView)");
#endif
            SendMessageW(pConfig->hwnd, WM_KEYDOWN, VK_F9, 0);
        }

        if (message == closeKey)
        {
#ifdef DEBUG_MSG
            println("Ctrl+W (WebView)");
#endif
            SendMessageW(pConfig->hwnd, WM_KEYDOWN, 0x57, 0);
        }
    }
}

RECT WebView::SettingsBounds()
{
    if (!pConfig->settings.menu)
        return RECT{0, 0, 0, 0};

    return get_rect(pConfig->hwnd);
}

RECT WebView::MainBounds()
{
    if (pConfig->settings.menu || !pConfig->settings.split && pConfig->settings.swapped)
        return RECT{0, 0, 0, 0};

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

    return bounds;
}

RECT WebView::SideBounds()
{
    if (pConfig->settings.menu || !pConfig->settings.split && !pConfig->settings.swapped)
        return RECT{0, 0, 0, 0};

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

    return bounds;
}

wstring WebView::GetScriptFile()
{
    stringstream buffer{};
    wstring script{};

    if (std::filesystem::exists(pConfig->paths.js))
    {
        ifstream f(pConfig->paths.js);
        if (!std::filesystem::is_empty(pConfig->paths.js))
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
