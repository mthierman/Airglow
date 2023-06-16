bool CommandLineUrl()
{
    int number;
    auto cmd = GetCommandLineW();
    auto args = CommandLineToArgvW(cmd, &number);
    if (number == 2)
    {
        url1 = args[1];
        url2 = args[1];
        split = false;
        return true;
    }
    if (number == 3)
    {
        url1 = args[1];
        url2 = args[2];
        split = true;
        return true;
    }
    LocalFree(args);
    return false;
}

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
    CoTaskMemFree(buffer);
    return true;
}

void WebViewMessages(HWND window, PWSTR message)
{
    if ((std::wstring)message == std::wstring(L"F1").c_str())
    {
        split = PanelSplit(window);
    }
    if ((std::wstring)message == std::wstring(L"F2").c_str())
    {
        swapped = PanelSwap(window);
    }
    if ((std::wstring)message == std::wstring(L"F4").c_str())
    {
        menu = PanelHideMenu(window);
    }
    if ((std::wstring)message == std::wstring(L"F6").c_str())
    {
        maximized = WindowMaximize(window);
    }
    if ((std::wstring)message == std::wstring(L"F11").c_str())
    {
        fullscreen = WindowFullscreen(window);
    }
    if ((std::wstring)message == std::wstring(L"F9").c_str())
    {
        ontop = WindowTop(window);
    }
    if ((std::wstring)message == std::wstring(L"close").c_str())
    {
        SendMessageW(window, WM_CLOSE, 0, 0);
    }
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
                            wv_controller->put_Bounds(GetWebView1Bounds(window));
                            wv->Navigate(url1.c_str());
                            wv->ExecuteScript(wvScript.c_str(), nullptr);
                            wv->AddScriptToExecuteOnDocumentCreated(wvScript.c_str(), nullptr);
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

void InitializeWebView2(HWND window, std::filesystem::path userData)
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
                            wv_controller2->put_Bounds(GetWebView2Bounds(window));
                            wv2->Navigate(url2.c_str());
                            wv2->ExecuteScript(wvScript.c_str(), nullptr);
                            wv2->AddScriptToExecuteOnDocumentCreated(wvScript.c_str(), nullptr);
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
                                                            // SendMessageW(window, WM_SETICON,
                                                            //              ICON_BIG,
                                                            //              (LPARAM)favicon.get());
                                                        }
                                                    }
                                                    return S_OK;
                                                })
                                                .Get());
                                        return S_OK;
                                    })
                                    .Get(),
                                &faviconChangedToken);
                            wv2->add_WebMessageReceived(
                                Microsoft::WRL::Callback<
                                    ICoreWebView2WebMessageReceivedEventHandler>(
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

void InitializeWebView3(HWND window, std::filesystem::path userData)
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
                            wv3->Navigate(url3.c_str());
                            EventRegistrationToken msgToken;
                            wv3->ExecuteScript(wvScriptBottom.c_str(), nullptr);
                            wv3->AddScriptToExecuteOnDocumentCreated(wvScriptBottom.c_str(),
                                                                     nullptr);
                            wv3->add_WebMessageReceived(
                                Microsoft::WRL::Callback<
                                    ICoreWebView2WebMessageReceivedEventHandler>(
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
