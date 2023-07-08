#include "WebView.hxx"

Config* WebView::pConfig{nullptr};

WebView::WebView(HWND hwnd, Config* config) {}

std::unique_ptr<WebView> WebView::Create(HWND hwnd, Config* config)
{
    pConfig = config;

    auto pWebView{std::unique_ptr<WebView>(new WebView(hwnd, pConfig))};

    CreateCoreWebView2EnvironmentWithOptions(
        nullptr, pConfig->dataPath.c_str(), nullptr,
        Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [hwnd](HRESULT result, ICoreWebView2Environment* env) -> HRESULT
            {
                // SETTINGS WEBVIEW
                env->CreateCoreWebView2Controller(
                    hwnd,
                    Microsoft::WRL::Callback<
                        ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                        [hwnd](HRESULT result, ICoreWebView2Controller* controller) -> HRESULT
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
                                settings_controller->put_Bounds(MenuBounds(hwnd));
                                settings_wv->Navigate(L"about:blank");
#ifdef _DEBUG
                                settings_wv->Navigate(L"https://localhost:8000/");
#endif

                                auto script = GetMenuScript();
                                settings_wv->ExecuteScript(script.c_str(), nullptr);
                                settings_wv->AddScriptToExecuteOnDocumentCreated(script.c_str(),
                                                                                 nullptr);

                                settings_wv->add_DocumentTitleChanged(
                                    Microsoft::WRL::Callback<
                                        ICoreWebView2DocumentTitleChangedEventHandler>(
                                        [hwnd](ICoreWebView2* sender, IUnknown* args) -> HRESULT
                                        {
                                            SetWindowTitle(hwnd);
                                            return S_OK;
                                        })
                                        .Get(),
                                    &documentTitleChangedToken);

                                settings_wv->add_FaviconChanged(
                                    Microsoft::WRL::Callback<
                                        ICoreWebView2FaviconChangedEventHandler>(
                                        [hwnd](ICoreWebView2* sender, IUnknown* args) -> HRESULT
                                        {
                                            SetWindowIcon(hwnd);
                                            return S_OK;
                                        })
                                        .Get(),
                                    &faviconChangedToken);

                                settings_wv->add_WebMessageReceived(
                                    Microsoft::WRL::Callback<
                                        ICoreWebView2WebMessageReceivedEventHandler>(
                                        [hwnd](ICoreWebView2* webview,
                                               ICoreWebView2WebMessageReceivedEventArgs* args)
                                            -> HRESULT
                                        {
                                            wil::unique_cotaskmem_string uri;
                                            args->get_Source(&uri);
                                            wstring sourceUri = uri.get();
                                            wstring verifyUri = L"https://localhost:8000/";

                                            if (sourceUri != verifyUri)
                                                return S_OK;

                                            wil::unique_cotaskmem_string messageRaw;
                                            auto tryMessage =
                                                args->TryGetWebMessageAsString(&messageRaw);
                                            if (tryMessage == S_OK)
                                            {
                                                auto message = wstring(messageRaw.get());

                                                if (message.compare(0, 8, L"mainUrl ") == 0)
                                                {
                                                    wprintln(message.substr(8));
                                                    pConfig->mainUrl = ToString(message.substr(8));
                                                }

                                                if (message.compare(0, 8, L"sideUrl ") == 0)
                                                {
                                                    wprintln(message.substr(8));
                                                    pConfig->sideUrl = ToString(message.substr(8));
                                                }

                                                Messages(hwnd, message);

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
                        [hwnd](HRESULT result, ICoreWebView2Controller* controller) -> HRESULT
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

                                main_controller->put_Bounds(MainBounds(hwnd));

                                auto args = CommandLine();

                                if (!args.first.empty())
                                {
                                    main_wv->Navigate(args.first.c_str());
                                }
                                else
                                    main_wv->Navigate(L"https://www.google.com/");

                                auto script = GetScript();
                                main_wv->ExecuteScript(script.c_str(), nullptr);
                                main_wv->AddScriptToExecuteOnDocumentCreated(script.c_str(),
                                                                             nullptr);

                                main_wv->add_DocumentTitleChanged(
                                    Microsoft::WRL::Callback<
                                        ICoreWebView2DocumentTitleChangedEventHandler>(
                                        [hwnd](ICoreWebView2* sender, IUnknown* args) -> HRESULT
                                        {
                                            SetWindowTitle(hwnd);
                                            return S_OK;
                                        })
                                        .Get(),
                                    &documentTitleChangedToken);

                                main_wv->add_FaviconChanged(
                                    Microsoft::WRL::Callback<
                                        ICoreWebView2FaviconChangedEventHandler>(
                                        [hwnd](ICoreWebView2* sender, IUnknown* args) -> HRESULT
                                        {
                                            SetWindowIcon(hwnd);
                                            return S_OK;
                                        })
                                        .Get(),
                                    &faviconChangedToken);

                                main_wv->add_WebMessageReceived(
                                    Microsoft::WRL::Callback<
                                        ICoreWebView2WebMessageReceivedEventHandler>(
                                        [hwnd](ICoreWebView2* webview,
                                               ICoreWebView2WebMessageReceivedEventArgs* args)
                                            -> HRESULT
                                        {
                                            wil::unique_cotaskmem_string message;
                                            if (SUCCEEDED(args->TryGetWebMessageAsString(&message)))
                                            {
                                                auto msg = wstring(message.get());
                                                Messages(hwnd, msg);
                                                webview->PostWebMessageAsString(message.get());
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
                        [hwnd](HRESULT result, ICoreWebView2Controller* controller) -> HRESULT
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

                                side_controller->put_Bounds(SideBounds(hwnd));

                                auto args = CommandLine();

                                if (!args.second.empty())
                                {
                                    side_wv->Navigate(args.second.c_str());
                                }
                                else
                                    side_wv->Navigate(L"https://www.google.com/");

                                auto script = GetScript();
                                side_wv->ExecuteScript(script.c_str(), nullptr);
                                side_wv->AddScriptToExecuteOnDocumentCreated(script.c_str(),
                                                                             nullptr);

                                side_wv->add_DocumentTitleChanged(
                                    Microsoft::WRL::Callback<
                                        ICoreWebView2DocumentTitleChangedEventHandler>(
                                        [hwnd](ICoreWebView2* sender, IUnknown* args) -> HRESULT
                                        {
                                            SetWindowTitle(hwnd);
                                            return S_OK;
                                        })
                                        .Get(),
                                    &documentTitleChangedToken);

                                side_wv->add_FaviconChanged(
                                    Microsoft::WRL::Callback<
                                        ICoreWebView2FaviconChangedEventHandler>(
                                        [hwnd](ICoreWebView2* sender, IUnknown* args) -> HRESULT
                                        {
                                            SetWindowIcon(hwnd);
                                            return S_OK;
                                        })
                                        .Get(),
                                    &faviconChangedToken);

                                side_wv->add_WebMessageReceived(
                                    Microsoft::WRL::Callback<
                                        ICoreWebView2WebMessageReceivedEventHandler>(
                                        [hwnd](ICoreWebView2* webview,
                                               ICoreWebView2WebMessageReceivedEventArgs* args)
                                            -> HRESULT
                                        {
                                            wil::unique_cotaskmem_string message;
                                            if (SUCCEEDED(args->TryGetWebMessageAsString(&message)))
                                            {
                                                auto msg = wstring(message.get());
                                                Messages(hwnd, msg);
                                                webview->PostWebMessageAsString(message.get());
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

    return pWebView;
}

wstring WebView::GetScriptFile(path appData)
{
    stringstream buffer{};
    wstring script{};

    path file = (appData.wstring() + path::preferred_separator + L"Airglow.js");

    if (!std::filesystem::exists(file) && std::filesystem::exists(file))
    {
        ifstream f(file);
        if (!std::filesystem::is_empty(file))
        {
            buffer << f.rdbuf();
            script = ToWide(buffer.str());
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

void WebView::Messages(HWND hwnd, wstring message)
{
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
        pConfig->split = Toggle(pConfig->split);
        WebView::UpdateBounds(hwnd);
        WebView::UpdateFocus();
        WebView::SetWindowTitle(hwnd);
        WebView::SetWindowIcon(hwnd);
        pConfig->Save();
    }

    if (message == swapKey)
    {
#ifdef _DEBUG
        println("F2 (WebView)");
#endif
        pConfig->swapped = Toggle(pConfig->swapped);
        WebView::UpdateBounds(hwnd);
        WebView::UpdateFocus();
        WebView::SetWindowTitle(hwnd);
        WebView::SetWindowIcon(hwnd);
        pConfig->Save();
    }

    if (message == hideMenuKey)
    {
#ifdef _DEBUG
        println("F4 (WebView)");
#endif
        pConfig->menu = Toggle(pConfig->menu);
        WebView::UpdateBounds(hwnd);
        WebView::UpdateFocus();
        WebView::SetWindowTitle(hwnd);
        WebView::SetWindowIcon(hwnd);
        pConfig->Save();
    }

    if (message == maximizeKey)
    {
#ifdef _DEBUG
        println("F6 (WebView)");
#endif
        if (!pConfig->fullscreen)
        {
            WINDOWPLACEMENT wp{sizeof(WINDOWPLACEMENT)};
            GetWindowPlacement(hwnd, &wp);
            if (wp.showCmd == 3)
            {
                ShowWindow(hwnd, SW_SHOWNORMAL);
                SetWindowPos(hwnd, nullptr, pConfig->position[0], pConfig->position[1],
                             pConfig->position[2], pConfig->position[3], 0);
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
        pConfig->fullscreen = Toggle(pConfig->fullscreen);
        MainWindow::Fullscreen(hwnd);
        WebView::UpdateBounds(hwnd);
        pConfig->Save();
    }

    if (message == onTopKey)
    {
#ifdef _DEBUG
        println("F9 (WebView)");
#endif
        pConfig->topmost = Toggle(pConfig->topmost);
        MainWindow::Topmost(hwnd);
        WebView::SetWindowTitle(hwnd);
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

void WebView::UpdateBounds(HWND hwnd)
{
    if (settings_controller)
        settings_controller->put_Bounds(MenuBounds(hwnd));

    if (main_controller)
        main_controller->put_Bounds(MainBounds(hwnd));

    if (side_controller)
        side_controller->put_Bounds(SideBounds(hwnd));
}

void WebView::UpdateFocus()
{
    if (pConfig)
    {
        if (pConfig->menu)
            if (settings_controller)
                settings_controller->MoveFocus(
                    COREWEBVIEW2_MOVE_FOCUS_REASON::COREWEBVIEW2_MOVE_FOCUS_REASON_PROGRAMMATIC);

        if (!pConfig->swapped & !pConfig->menu)
            if (main_controller)
                main_controller->MoveFocus(
                    COREWEBVIEW2_MOVE_FOCUS_REASON::COREWEBVIEW2_MOVE_FOCUS_REASON_PROGRAMMATIC);

        if (pConfig->swapped & !pConfig->menu)
            if (side_controller)
                side_controller->MoveFocus(
                    COREWEBVIEW2_MOVE_FOCUS_REASON::COREWEBVIEW2_MOVE_FOCUS_REASON_PROGRAMMATIC);
    }
}

RECT WebView::FullBounds(HWND hwnd)
{
    RECT bounds{0, 0, 0, 0};
    GetClientRect(hwnd, &bounds);

    return bounds;
}

RECT WebView::MenuBounds(HWND hwnd)
{
    RECT panel{0, 0, 0, 0};
    RECT bounds{0, 0, 0, 0};
    GetClientRect(hwnd, &bounds);

    if (pConfig->menu)
    {
        panel = {
            bounds.left,
            bounds.top,
            bounds.right,
            bounds.bottom,
        };
    }

    return panel;
}

RECT WebView::MainBounds(HWND window)
{
    RECT panel{0, 0, 0, 0};
    RECT bounds{0, 0, 0, 0};
    GetClientRect(window, &bounds);

    if (pConfig->menu)
        return panel;

    if (!pConfig->split & !pConfig->swapped)
        panel = bounds;

    if (!pConfig->split & pConfig->swapped)
        return panel;

    if (pConfig->split & !pConfig->swapped)
    {
        panel = {
            bounds.left,
            bounds.top,
            bounds.right / 2,
            bounds.bottom,
        };
    }

    if (pConfig->split & pConfig->swapped)
    {
        panel = {
            bounds.right / 2,
            bounds.top,
            bounds.right,
            bounds.bottom,
        };
    }

    return panel;
}

RECT WebView::SideBounds(HWND window)
{
    RECT panel{0, 0, 0, 0};
    RECT bounds{0, 0, 0, 0};
    GetClientRect(window, &bounds);

    if (pConfig->menu)
        return panel;

    if (!pConfig->split & !pConfig->swapped)
        return panel;

    if (!pConfig->split & pConfig->swapped)
        panel = bounds;

    if (pConfig->split & !pConfig->swapped)
    {
        panel = {
            bounds.right / 2,
            bounds.top,
            bounds.right,
            bounds.bottom,
        };
    }

    if (pConfig->split & pConfig->swapped)
    {
        panel = {
            bounds.left,
            bounds.top,
            bounds.right / 2,
            bounds.bottom,
        };
    }

    return panel;
}

void WebView::SetWindowTitle(HWND hwnd)
{
    if (settings_wv && pConfig->menu)
    {
        wil::unique_cotaskmem_string s{};
        settings_wv->get_DocumentTitle(&s);
        auto title = s.get();

        if (!pConfig->topmost)
            SetWindowTextW(hwnd, s.get());

        if (pConfig->topmost)
        {
            wstring add = title + wstring(L" [On Top]");
            SetWindowTextW(hwnd, add.c_str());
        }
    }

    if (main_wv && !pConfig->menu && !pConfig->swapped)
    {
        wil::unique_cotaskmem_string s{};
        main_wv->get_DocumentTitle(&s);
        auto title = s.get();

        if (!pConfig->topmost)
            SetWindowTextW(hwnd, title);

        if (pConfig->topmost)
        {
            wstring add = title + wstring(L" [On Top]");
            SetWindowTextW(hwnd, add.c_str());
        }
    }

    if (side_wv && !pConfig->menu && pConfig->swapped)
    {
        wil::unique_cotaskmem_string s{};
        side_wv->get_DocumentTitle(&s);
        auto title = s.get();

        if (!pConfig->topmost)
            SetWindowTextW(hwnd, title);

        if (pConfig->topmost)
        {
            wstring add = title + wstring(L" [On Top]");
            SetWindowTextW(hwnd, add.c_str());
        }
    }
}

void WebView::SetWindowIcon(HWND hwnd)
{
    if (settings_wv && pConfig->menu)
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

    if (main_wv && !pConfig->swapped && !pConfig->menu)
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

    if (side_wv && pConfig->swapped && !pConfig->menu)
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
