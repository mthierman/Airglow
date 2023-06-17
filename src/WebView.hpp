using namespace Microsoft::WRL;

void InitializeWebViews(HWND window, std::filesystem::path userData)
{
    CreateCoreWebView2EnvironmentWithOptions(
        nullptr, userData.c_str(), nullptr,
        Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [window](HRESULT result, ICoreWebView2Environment* env) -> HRESULT
            {
                env->CreateCoreWebView2Controller(
                    window,
                    Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                        [window](HRESULT result, ICoreWebView2Controller* controller) -> HRESULT
                        {
                            EventRegistrationToken msgToken;

                            if (controller != nullptr)
                            {
                                settings_controller = controller;
                                settings_controller->get_CoreWebView2(&settings_core);
                            }

                            settings_wv = settings_core.try_query<ICoreWebView2_19>();
                            settings_wv->get_Settings(&settings_settings);
                            settings_settings->put_AreDefaultContextMenusEnabled(false);
                            settings_settings->put_AreDefaultScriptDialogsEnabled(true);
                            settings_settings->put_AreDevToolsEnabled(true);
                            settings_settings->put_AreHostObjectsAllowed(true);
                            settings_settings->put_IsBuiltInErrorPageEnabled(true);
                            settings_settings->put_IsScriptEnabled(true);
                            settings_settings->put_IsStatusBarEnabled(false);
                            settings_settings->put_IsWebMessageEnabled(true);
                            settings_settings->put_IsZoomControlEnabled(false);
                            settings_controller->put_Bounds(GetMenuBounds(window));
                            settings_wv->Navigate(L"https://localhost:8000");
                            settings_wv->ExecuteScript(menuScript.c_str(), nullptr);
                            settings_wv->AddScriptToExecuteOnDocumentCreated(menuScript.c_str(),
                                                                             nullptr);

                            settings_wv->add_WebMessageReceived(
                                Callback<ICoreWebView2WebMessageReceivedEventHandler>(
                                    [window](
                                        ICoreWebView2* webview,
                                        ICoreWebView2WebMessageReceivedEventArgs* args) -> HRESULT
                                    {
                                        wil::unique_cotaskmem_string message;
                                        args->TryGetWebMessageAsString(&message);
                                        auto msg = std::wstring(message.get());
                                        WebViewMessages(window, msg);
                                        webview->PostWebMessageAsString(message.get());
                                        return S_OK;
                                    })
                                    .Get(),
                                &msgToken);

                            return S_OK;
                        })
                        .Get());

                env->CreateCoreWebView2Controller(
                    window,
                    Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                        [window](HRESULT result, ICoreWebView2Controller* controller) -> HRESULT
                        {
                            EventRegistrationToken msgToken;
                            EventRegistrationToken faviconChangedToken;
                            EventRegistrationToken documentTitleChangedToken;

                            if (controller != nullptr)
                            {
                                main_controller = controller;
                                main_controller->get_CoreWebView2(&main_core);
                            }

                            main_wv = main_core.try_query<ICoreWebView2_19>();
                            main_wv->get_Settings(&main_settings);
                            main_settings->put_AreDefaultContextMenusEnabled(true);
                            main_settings->put_AreDefaultScriptDialogsEnabled(true);
                            main_settings->put_AreDevToolsEnabled(true);
                            main_settings->put_AreHostObjectsAllowed(true);
                            main_settings->put_IsBuiltInErrorPageEnabled(true);
                            main_settings->put_IsScriptEnabled(true);
                            main_settings->put_IsStatusBarEnabled(true);
                            main_settings->put_IsWebMessageEnabled(true);
                            main_settings->put_IsZoomControlEnabled(true);
                            main_controller->put_Bounds(GetMainPanelBounds(window));

                            auto args = CommandLine();
                            if (!args.first.empty())
                            {
                                main_wv->Navigate(args.first.c_str());
                            }

                            else
                            {
                                main_wv->Navigate(mainpage.c_str());
                            }

                            main_wv->ExecuteScript(script.c_str(), nullptr);
                            main_wv->AddScriptToExecuteOnDocumentCreated(script.c_str(), nullptr);

                            main_wv->add_DocumentTitleChanged(
                                Callback<ICoreWebView2DocumentTitleChangedEventHandler>(
                                    [window](ICoreWebView2* sender, IUnknown* args) -> HRESULT
                                    {
                                        SetWindowTitle(window);
                                        return S_OK;
                                    })
                                    .Get(),
                                &documentTitleChangedToken);

                            main_wv->add_FaviconChanged(
                                Callback<ICoreWebView2FaviconChangedEventHandler>(
                                    [window](ICoreWebView2* sender, IUnknown* args) -> HRESULT
                                    {
                                        SetWindowIcon(window);
                                        return S_OK;
                                    })
                                    .Get(),
                                &faviconChangedToken);

                            main_wv->add_WebMessageReceived(
                                Callback<ICoreWebView2WebMessageReceivedEventHandler>(
                                    [window](
                                        ICoreWebView2* webview,
                                        ICoreWebView2WebMessageReceivedEventArgs* args) -> HRESULT
                                    {
                                        wil::unique_cotaskmem_string message;
                                        args->TryGetWebMessageAsString(&message);
                                        auto msg = message.get();
                                        WebViewMessages(window, msg);
                                        webview->PostWebMessageAsString(message.get());
                                        return S_OK;
                                    })
                                    .Get(),
                                &msgToken);

                            return S_OK;
                        })
                        .Get());

                env->CreateCoreWebView2Controller(
                    window,
                    Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                        [window](HRESULT result, ICoreWebView2Controller* controller) -> HRESULT
                        {
                            EventRegistrationToken msgToken;
                            EventRegistrationToken faviconChangedToken;
                            EventRegistrationToken documentTitleChangedToken;

                            if (controller != nullptr)
                            {
                                side_controller = controller;
                                side_controller->get_CoreWebView2(&side_core);
                            }

                            side_wv = side_core.try_query<ICoreWebView2_19>();
                            side_wv->get_Settings(&side_settings);
                            side_settings->put_AreDefaultContextMenusEnabled(true);
                            side_settings->put_AreDefaultScriptDialogsEnabled(true);
                            side_settings->put_AreDevToolsEnabled(true);
                            side_settings->put_AreHostObjectsAllowed(true);
                            side_settings->put_IsBuiltInErrorPageEnabled(true);
                            side_settings->put_IsScriptEnabled(true);
                            side_settings->put_IsStatusBarEnabled(true);
                            side_settings->put_IsWebMessageEnabled(true);
                            side_settings->put_IsZoomControlEnabled(true);
                            side_controller->put_Bounds(GetSidePanelBounds(window));

                            auto args = CommandLine();
                            if (!args.second.empty())
                            {
                                side_wv->Navigate(args.second.c_str());
                            }

                            else
                            {
                                side_wv->Navigate(sidepage.c_str());
                            }

                            side_wv->ExecuteScript(script.c_str(), nullptr);
                            side_wv->AddScriptToExecuteOnDocumentCreated(script.c_str(), nullptr);

                            side_wv->add_DocumentTitleChanged(
                                Callback<ICoreWebView2DocumentTitleChangedEventHandler>(
                                    [window](ICoreWebView2* sender, IUnknown* args) -> HRESULT
                                    {
                                        SetWindowTitle(window);
                                        return S_OK;
                                    })
                                    .Get(),
                                &documentTitleChangedToken);

                            side_wv->add_FaviconChanged(
                                Callback<ICoreWebView2FaviconChangedEventHandler>(
                                    [window](ICoreWebView2* sender, IUnknown* args) -> HRESULT
                                    {
                                        SetWindowIcon(window);
                                        return S_OK;
                                    })
                                    .Get(),
                                &faviconChangedToken);

                            side_wv->add_WebMessageReceived(
                                Callback<ICoreWebView2WebMessageReceivedEventHandler>(
                                    [window](
                                        ICoreWebView2* webview,
                                        ICoreWebView2WebMessageReceivedEventArgs* args) -> HRESULT
                                    {
                                        wil::unique_cotaskmem_string message;
                                        args->TryGetWebMessageAsString(&message);
                                        auto msg = message.get();
                                        WebViewMessages(window, msg);
                                        webview->PostWebMessageAsString(message.get());
                                        return S_OK;
                                    })
                                    .Get(),
                                &msgToken);

                            return S_OK;
                        })
                        .Get());

                return S_OK;
            })
            .Get());
}

std::wstring GetScriptFile(std::filesystem::path appData)
{
    std::stringstream buffer;
    std::wstring script;

    std::filesystem::path file =
        (appData.wstring() + std::filesystem::path::preferred_separator + L"Airglow.js");

    if (!std::filesystem::exists(file))

        if (std::filesystem::exists(file))
        {
            std::ifstream f(file);
            if (!std::filesystem::is_empty(file))
            {
                buffer << f.rdbuf();
                script = ToWide(buffer.str());
            }
            f.close();
        }

    return script;
}

std::wstring GetScript(std::filesystem::path appData)
{
    std::wstring script = LR"(
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
    )";

    return script;
}

std::wstring GetMenuScript(std::filesystem::path appData)
{
    std::wstring script = LR"(
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
    )";

    return script;
}

void SetWindowTitle(HWND window)
{
    std::wstring titleTop = L" [On Top]";

    if (!swapped)
    {
        wil::unique_cotaskmem_string wv_title;
        main_wv->get_DocumentTitle(&wv_title);
        auto title = wv_title.get();

        if (!ontop)
            SetWindowTextW(window, title);

        if (ontop)
        {
            std::wstring add = title + titleTop;
            SetWindowTextW(window, add.c_str());
        }
    }

    else
    {
        wil::unique_cotaskmem_string wv_title;
        side_wv->get_DocumentTitle(&wv_title);
        auto title = wv_title.get();

        if (!ontop)
            SetWindowTextW(window, title);

        if (ontop)
        {
            std::wstring add = title + titleTop;
            SetWindowTextW(window, add.c_str());
        }
    }
}

void SetWindowIcon(HWND window)
{
    if (!swapped)
    {
        if (main_controller != nullptr)
        {
            main_wv->GetFavicon(COREWEBVIEW2_FAVICON_IMAGE_FORMAT_PNG,
                                Callback<ICoreWebView2GetFaviconCompletedHandler>(
                                    [window](HRESULT result, IStream* iconStream) -> HRESULT
                                    {
                                        if (iconStream != nullptr)
                                        {
                                            Gdiplus::Bitmap iconBitmap(iconStream);
                                            wil::unique_hicon icon;
                                            if (iconBitmap.GetHICON(&icon) == Gdiplus::Status::Ok)
                                            {
                                                auto favicon = std::move(icon);
                                                SendMessageW(window, WM_SETICON, ICON_BIG,
                                                             (LPARAM)favicon.get());
                                            }
                                        }
                                        return S_OK;
                                    })
                                    .Get());
        }
    }

    else
    {
        if (side_controller != nullptr)
        {
            side_wv->GetFavicon(COREWEBVIEW2_FAVICON_IMAGE_FORMAT_PNG,
                                Callback<ICoreWebView2GetFaviconCompletedHandler>(
                                    [window](HRESULT result, IStream* iconStream) -> HRESULT
                                    {
                                        if (iconStream != nullptr)
                                        {
                                            Gdiplus::Bitmap iconBitmap(iconStream);
                                            wil::unique_hicon icon;
                                            if (iconBitmap.GetHICON(&icon) == Gdiplus::Status::Ok)
                                            {
                                                auto favicon = std::move(icon);
                                                SendMessageW(window, WM_SETICON, ICON_BIG,
                                                             (LPARAM)favicon.get());
                                            }
                                        }
                                        return S_OK;
                                    })
                                    .Get());
        }
    }
}
