bool GetAppDataPath()
{
    std::wstring path;
    PWSTR buffer;
    auto getKnownFolderPath = SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, nullptr, &buffer);

    if (getKnownFolderPath != S_OK)
    {
        CoTaskMemFree(buffer);
        return false;
    }

    path = buffer;
    userData = path + std::filesystem::path::preferred_separator + L"Airglow";
    OutputDebugStringW(userData.c_str());
    CoTaskMemFree(buffer);
    return true;
}

void InitializeWebView1(HWND window, std::filesystem::path userData)
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
                                Callback<ICoreWebView2DocumentTitleChangedEventHandler>(
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
                                Callback<ICoreWebView2FaviconChangedEventHandler>(
                                    [window](ICoreWebView2* sender, IUnknown* args) -> HRESULT
                                    {
                                        LPWSTR faviconUri;
                                        wv->get_FaviconUri(&faviconUri);
                                        OutputDebugStringW(faviconUri);
                                        wv->GetFavicon(
                                            COREWEBVIEW2_FAVICON_IMAGE_FORMAT_PNG,
                                            Callback<ICoreWebView2GetFaviconCompletedHandler>(
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
                                                    }
                                                    return S_OK;
                                                })
                                                .Get());
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
                                        if ((std::wstring)message.get() ==
                                            std::wstring(L"F2").c_str())
                                        {
                                            isMaximized = KeyMaximize(window);
                                        }
                                        if ((std::wstring)message.get() ==
                                            std::wstring(L"F4").c_str())
                                        {
                                            isTopmost = KeyTop(window);
                                        }
                                        if ((std::wstring)message.get() ==
                                            std::wstring(L"F11").c_str())
                                        {
                                            isFullscreen = KeyFullscreen(window);
                                        }
                                        if ((std::wstring)message.get() ==
                                            std::wstring(L"F1").c_str())
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

void InitializeWebView2(HWND window, std::filesystem::path userData)
{
    CreateCoreWebView2EnvironmentWithOptions(
        nullptr, userData.c_str(), nullptr,
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
                            RECT wv_bounds2 = {
                                bounds.right / 2,
                                bounds.top,
                                bounds.right,
                                bounds.bottom,
                            };
                            if (isSplit)
                                wv_controller2->put_Bounds(wv_bounds2);
                            wv2->Navigate(url2.c_str());
                            EventRegistrationToken token;
                            wv2->ExecuteScript(wvScript.c_str(), nullptr);
                            wv2->AddScriptToExecuteOnDocumentCreated(wvScript.c_str(), nullptr);
                            wv2->add_WebMessageReceived(
                                Microsoft::WRL::Callback<
                                    ICoreWebView2WebMessageReceivedEventHandler>(
                                    [window](
                                        ICoreWebView2* webview,
                                        ICoreWebView2WebMessageReceivedEventArgs* args) -> HRESULT
                                    {
                                        wil::unique_cotaskmem_string message;
                                        args->TryGetWebMessageAsString(&message);
                                        if ((std::wstring)message.get() ==
                                            std::wstring(L"F2").c_str())
                                        {
                                            isMaximized = KeyMaximize(window);
                                        }
                                        if ((std::wstring)message.get() ==
                                            std::wstring(L"F4").c_str())
                                        {
                                            isTopmost = KeyTop(window);
                                        }
                                        if ((std::wstring)message.get() ==
                                            std::wstring(L"F11").c_str())
                                        {
                                            isFullscreen = KeyFullscreen(window);
                                        }
                                        if ((std::wstring)message.get() ==
                                            std::wstring(L"F1").c_str())
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
