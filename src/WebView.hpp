void CreateWebView2Env(HWND window)
{
    using namespace Microsoft::WRL;

    CreateCoreWebView2EnvironmentWithOptions(
        nullptr, nullptr, nullptr,
        Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [window](HRESULT result, ICoreWebView2Environment* env) -> HRESULT
            {
                if (env != nullptr)
                {
                    wv_env3 = env;
                }
                return S_OK;
            })
            .Get());

    wv_env3->CreateCoreWebView2Controller(
        window, Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                    [window](HRESULT result, ICoreWebView2Controller* controller) -> HRESULT
                    {
                        if (controller != nullptr)
                        {
                            wv_controller3 = controller;
                            wv_controller3->get_CoreWebView2(&wv_core3);
                        }
                        wv_core3->Navigate(L"https://www.google.com/");
                        // wv3 = wv_core3.try_query<ICoreWebView2_19>();
                        GetClientRect(window, &bounds);
                        wv_controller3->put_Bounds(bounds);
                        // wv3->get_Settings(&wv_settings3);
                        // wv3->Navigate(L"https://www.google.com/");
                        return S_OK;
                    })
                    .Get());

    // wv3->Navigate(L"https://www.bing.com/");
    // wv3 = wv_core3.try_query<ICoreWebView2_19>();
    // GetClientRect(window, &bounds);
    // wv_controller3->put_Bounds(bounds);
    // wv3->get_Settings(&wv_settings3);
    // wv_core3->Navigate(L"https://www.google.com/");
}

void CreateWebView2Env2(HWND window)
{
    CreateCoreWebView2EnvironmentWithOptions(
        nullptr, nullptr, nullptr,
        Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [window](HRESULT result, ICoreWebView2Environment* env) -> HRESULT
            {
                env->CreateCoreWebView2Controller(
                    window,
                    Microsoft::WRL::Callback<
                        ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
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
                            RECT wv_bounds = {
                                bounds.left,
                                bounds.top,
                                bounds.right / 2,
                                bounds.bottom,
                            };
                            if (!isSplit)
                                wv_controller->put_Bounds(bounds);
                            if (isSplit)
                                wv_controller->put_Bounds(wv_bounds);
                            wv->Navigate(url1.c_str());
                            wv->ExecuteScript(wvScript.c_str(), nullptr);
                            wv->AddScriptToExecuteOnDocumentCreated(wvScript.c_str(), nullptr);
                            EventRegistrationToken token;
                            EventRegistrationToken faviconChangedToken;
                            EventRegistrationToken documentTitleChangedToken;
                            wv->add_DocumentTitleChanged(
                                Microsoft::WRL::Callback<
                                    ICoreWebView2DocumentTitleChangedEventHandler>(
                                    [window](ICoreWebView2* sender, IUnknown* args) -> HRESULT
                                    {
                                        sender->get_DocumentTitle(&title);
                                        auto documentTitle = title.get();
                                        SetWindowTextW(window, documentTitle);
                                        return S_OK;
                                    })
                                    .Get(),
                                &documentTitleChangedToken);
                            wv->add_FaviconChanged(
                                Microsoft::WRL::Callback<ICoreWebView2FaviconChangedEventHandler>(
                                    [window](ICoreWebView2* sender, IUnknown* args) -> HRESULT
                                    {
                                        wv->GetFavicon(
                                            COREWEBVIEW2_FAVICON_IMAGE_FORMAT_PNG,
                                            Microsoft::WRL::Callback<
                                                ICoreWebView2GetFaviconCompletedHandler>(
                                                [window](HRESULT result,
                                                         IStream* iconStream) -> HRESULT
                                                {
                                                    if (iconStream != nullptr)
                                                    {
                                                        Gdiplus::Bitmap iconBitmap(iconStream);
                                                        wil::unique_hicon icon;
                                                        if (iconBitmap.GetHICON(&icon) ==
                                                            Gdiplus::Status::Ok)
                                                        {
                                                            auto favicon = std::move(icon);
                                                            SendMessageW(window, WM_SETICON,
                                                                         ICON_BIG,
                                                                         (LPARAM)favicon.get());
                                                        }
                                                        else
                                                        {
                                                            SendMessageW(window, WM_SETICON,
                                                                         ICON_SMALL,
                                                                         (LPARAM)IDC_NO);
                                                        }
                                                    }
                                                    return S_OK;
                                                })
                                                .Get());
                                        return S_OK;
                                    })
                                    .Get(),
                                &faviconChangedToken);
                            wv->add_WebMessageReceived(
                                Microsoft::WRL::Callback<
                                    ICoreWebView2WebMessageReceivedEventHandler>(
                                    [window](
                                        ICoreWebView2* webview,
                                        ICoreWebView2WebMessageReceivedEventArgs* args) -> HRESULT
                                    {
                                        wil::unique_cotaskmem_string message;
                                        args->TryGetWebMessageAsString(&message);
                                        if ((std::wstring)message.get() ==
                                            std::wstring(L"F1").c_str())
                                        {
                                            isMaximized = KeyMaximize(window);
                                        }
                                        if ((std::wstring)message.get() ==
                                            std::wstring(L"F2").c_str())
                                        {
                                            isTopmost = KeyTop(window);
                                        }
                                        if ((std::wstring)message.get() ==
                                            std::wstring(L"F11").c_str())
                                        {
                                            isFullscreen = KeyFullscreen(window);
                                        }
                                        if ((std::wstring)message.get() ==
                                            std::wstring(L"split").c_str())
                                        {
                                            isSplit = KeySplit(window);
                                        }
                                        if ((std::wstring)message.get() ==
                                            std::wstring(L"close").c_str())
                                        {
                                            KeyClose(window);
                                        }
                                        webview->PostWebMessageAsString(message.get());
                                        return S_OK;
                                    })
                                    .Get(),
                                &token);
                            return S_OK;
                        })
                        .Get());
                return S_OK;
            })
            .Get());
}
