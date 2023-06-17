void InitializeMenu(HWND window, std::filesystem::path userData)
{
    using namespace Microsoft::WRL;

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
                            if (controller != nullptr)
                            {
                                wv_controller3 = controller;
                                wv_controller3->get_CoreWebView2(&wv_core3);
                            }

                            wv3 = wv_core3.try_query<ICoreWebView2_19>();

                            wv3->get_Settings(&wv_settings3);
                            wv_settings3->put_AreDefaultContextMenusEnabled(false);
                            wv_settings3->put_AreDefaultScriptDialogsEnabled(true);
                            wv_settings3->put_AreDevToolsEnabled(true);
                            wv_settings3->put_AreHostObjectsAllowed(true);
                            wv_settings3->put_IsBuiltInErrorPageEnabled(true);
                            wv_settings3->put_IsScriptEnabled(true);
                            wv_settings3->put_IsStatusBarEnabled(false);
                            wv_settings3->put_IsWebMessageEnabled(true);
                            wv_settings3->put_IsZoomControlEnabled(false);

                            wv_controller3->put_Bounds(GetMenuBounds(window));

                            wv3->Navigate(L"https://localhost:8000");

                            wv3->ExecuteScript(menuScript.c_str(), nullptr);
                            wv3->AddScriptToExecuteOnDocumentCreated(menuScript.c_str(), nullptr);

                            EventRegistrationToken msgToken;

                            wv3->add_WebMessageReceived(
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
                return S_OK;
            })
            .Get());
}

void InitializeMainPanel(HWND window, std::filesystem::path userData)
{
    using namespace Microsoft::WRL;

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
                            if (controller != nullptr)
                            {
                                wv_controller = controller;
                                wv_controller->get_CoreWebView2(&wv_core);
                            }

                            wv = wv_core.try_query<ICoreWebView2_19>();

                            wv->get_Settings(&wv_settings);
                            wv_settings->put_AreDefaultContextMenusEnabled(true);
                            wv_settings->put_AreDefaultScriptDialogsEnabled(true);
                            wv_settings->put_AreDevToolsEnabled(true);
                            wv_settings->put_AreHostObjectsAllowed(true);
                            wv_settings->put_IsBuiltInErrorPageEnabled(true);
                            wv_settings->put_IsScriptEnabled(true);
                            wv_settings->put_IsStatusBarEnabled(true);
                            wv_settings->put_IsWebMessageEnabled(true);
                            wv_settings->put_IsZoomControlEnabled(true);

                            wv_controller->put_Bounds(GetMainPanelBounds(window));

                            auto args = CommandLine();

                            if (!args.first.empty())
                            {
                                wv->Navigate(args.first.c_str());
                            }

                            else
                            {
                                wv->Navigate(mainpage.c_str());
                            }

                            wv->ExecuteScript(script.c_str(), nullptr);
                            wv->AddScriptToExecuteOnDocumentCreated(script.c_str(), nullptr);

                            EventRegistrationToken msgToken;
                            EventRegistrationToken faviconChangedToken;
                            EventRegistrationToken documentTitleChangedToken;

                            wv->add_DocumentTitleChanged(
                                Callback<ICoreWebView2DocumentTitleChangedEventHandler>(
                                    [window](ICoreWebView2* sender, IUnknown* args) -> HRESULT
                                    {
                                        SetWindowTitle(window);
                                        return S_OK;
                                    })
                                    .Get(),
                                &documentTitleChangedToken);

                            wv->add_FaviconChanged(
                                Callback<ICoreWebView2FaviconChangedEventHandler>(
                                    [window](ICoreWebView2* sender, IUnknown* args) -> HRESULT
                                    {
                                        SetWindowIcon(window);
                                        return S_OK;
                                    })
                                    .Get(),
                                &faviconChangedToken);

                            wv->add_WebMessageReceived(
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

void InitializeSidePanel(HWND window, std::filesystem::path userData)
{
    using namespace Microsoft::WRL;

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
                            if (controller != nullptr)
                            {
                                wv_controller2 = controller;
                                wv_controller2->get_CoreWebView2(&wv_core2);
                            }

                            wv2 = wv_core2.try_query<ICoreWebView2_19>();

                            wv2->get_Settings(&wv_settings2);
                            wv_settings2->put_AreDefaultContextMenusEnabled(true);
                            wv_settings2->put_AreDefaultScriptDialogsEnabled(true);
                            wv_settings2->put_AreDevToolsEnabled(true);
                            wv_settings2->put_AreHostObjectsAllowed(true);
                            wv_settings2->put_IsBuiltInErrorPageEnabled(true);
                            wv_settings2->put_IsScriptEnabled(true);
                            wv_settings2->put_IsStatusBarEnabled(true);
                            wv_settings2->put_IsWebMessageEnabled(true);
                            wv_settings2->put_IsZoomControlEnabled(true);

                            wv_controller2->put_Bounds(GetSidePanelBounds(window));

                            auto args = CommandLine();

                            if (!args.second.empty())
                            {
                                wv2->Navigate(args.second.c_str());
                            }

                            else
                            {
                                wv2->Navigate(sidepage.c_str());
                            }

                            wv2->ExecuteScript(script.c_str(), nullptr);
                            wv2->AddScriptToExecuteOnDocumentCreated(script.c_str(), nullptr);

                            EventRegistrationToken msgToken;
                            EventRegistrationToken faviconChangedToken;
                            EventRegistrationToken documentTitleChangedToken;

                            wv->add_DocumentTitleChanged(
                                Callback<ICoreWebView2DocumentTitleChangedEventHandler>(
                                    [window](ICoreWebView2* sender, IUnknown* args) -> HRESULT
                                    {
                                        SetWindowTitle(window);
                                        return S_OK;
                                    })
                                    .Get(),
                                &documentTitleChangedToken);

                            wv->add_FaviconChanged(
                                Callback<ICoreWebView2FaviconChangedEventHandler>(
                                    [window](ICoreWebView2* sender, IUnknown* args) -> HRESULT
                                    {
                                        SetWindowIcon(window);
                                        return S_OK;
                                    })
                                    .Get(),
                                &faviconChangedToken);

                            wv2->add_WebMessageReceived(
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
        wv->get_DocumentTitle(&wv_title);
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
        wv2->get_DocumentTitle(&wv_title);
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
    using namespace Microsoft::WRL;

    if (!swapped)
    {
        if (wv_controller != nullptr)
        {
            wv->GetFavicon(COREWEBVIEW2_FAVICON_IMAGE_FORMAT_PNG,
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
        if (wv_controller2 != nullptr)
        {
            wv2->GetFavicon(COREWEBVIEW2_FAVICON_IMAGE_FORMAT_PNG,
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
