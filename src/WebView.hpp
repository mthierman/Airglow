std::pair<std::wstring, std::wstring> CommandLine()
{
    std::pair<std::wstring, std::wstring> pair;
    int number;
    auto cmd = GetCommandLineW();
    auto args = CommandLineToArgvW(cmd, &number);
    if (number == 2)
    {
        pair.first = args[1];
        pair.second = args[1];
    }
    if (number == 3)
    {
        pair.first = args[1];
        pair.second = args[2];
    }
    LocalFree(args);
    return pair;
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
        SetWindowTitle(window);
        SetWindowIcon(window);
    }
    if ((std::wstring)message == std::wstring(L"F4").c_str())
    {
        menu = PanelHideMenu(window);
        SetWindowTitle(window);
        SetWindowIcon(window);
    }
    if ((std::wstring)message == std::wstring(L"F10").c_str())
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
        SetWindowTitle(window);
        SetWindowIcon(window);
    }
    if ((std::wstring)message == std::wstring(L"close").c_str())
    {
        SendMessageW(window, WM_CLOSE, 0, 0);
    }
}

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
                                        SetWindowIcon(window);
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
