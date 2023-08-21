#include "WebView.hxx"

WebView::WebView(Storage* s, HWND h, std::string n) : storage(s), appHwnd(h), name(n)
{
    create_webview();
}

WebView::~WebView() {}

winrt::IAsyncAction WebView::create_webview()
{
    auto windowRef{winrt::CoreWebView2ControllerWindowReference::CreateFromWindowHandle(
        reinterpret_cast<uint64_t>(appHwnd))};

    auto environmentOptions{winrt::CoreWebView2EnvironmentOptions()};

    environmentOptions.AdditionalBrowserArguments(L"--allow-file-access-from-files");

    environment = co_await winrt::CoreWebView2Environment::CreateWithOptionsAsync(
        L"", util::path_portable().wstring().c_str(), environmentOptions);

    controller = co_await environment.CreateCoreWebView2ControllerAsync(windowRef);

    core = controller.CoreWebView2();

    auto webviewSettings{core.Settings()};

    webviewSettings.AreBrowserAcceleratorKeysEnabled(true);
    webviewSettings.AreDefaultContextMenusEnabled(true);
    webviewSettings.AreDefaultScriptDialogsEnabled(true);
    webviewSettings.AreDevToolsEnabled(true);
    webviewSettings.AreHostObjectsAllowed(true);
    webviewSettings.IsBuiltInErrorPageEnabled(true);
    webviewSettings.IsGeneralAutofillEnabled(true);
    webviewSettings.IsPasswordAutosaveEnabled(true);
    webviewSettings.IsPinchZoomEnabled(true);
    webviewSettings.IsReputationCheckingRequired(true);
    webviewSettings.IsScriptEnabled(true);
    webviewSettings.IsStatusBarEnabled(true);
    webviewSettings.IsSwipeNavigationEnabled(true);
    webviewSettings.IsWebMessageEnabled(true);
    webviewSettings.IsZoomControlEnabled(true);

    if (name == "gui")
    {
        webviewSettings.IsZoomControlEnabled(false);

        core.Navigate(util::settings_url());

        core.WebMessageReceived(
            {[=, this](auto const&, auto const& args) { gui_web_message_received(args); }});
    }

    if (name == "bar")
    {
        webviewSettings.IsZoomControlEnabled(false);

        core.Navigate(util::bar_url());

        core.WebMessageReceived(
            {[=, this](auto const&, auto const& args) { bar_web_message_received(args); }});
    }

    if (name == "main")
    {
        webviewSettings.IsZoomControlEnabled(true);

        auto args{util::command_line()};

        if (!args.first.empty())
            core.Navigate(args.first);

        if (args.first.empty() && !storage->settings.mainHomepage.empty())
            core.Navigate(winrt::to_hstring(storage->settings.mainHomepage));

        if (args.first.empty() && storage->settings.mainHomepage.empty())
            core.Navigate(util::home_url());

        core.SourceChanged({[=, this](winrt::CoreWebView2 const& sender, auto const&)
                            {
                                storage->settings.mainCurrentPage =
                                    winrt::to_string(sender.Source());

                                SendMessageW(appHwnd, WM_NOTIFY, 0, 0);
                            }});
    }

    if (name == "side")
    {
        webviewSettings.IsZoomControlEnabled(true);

        auto args{util::command_line()};

        if (!args.second.empty())
            core.Navigate(args.second);

        if (args.second.empty() && !storage->settings.sideHomepage.empty())
            core.Navigate(winrt::to_hstring(storage->settings.sideHomepage));

        if (args.second.empty() && storage->settings.sideHomepage.empty())
            core.Navigate(util::home_url());

        core.SourceChanged({[=, this](winrt::CoreWebView2 const& sender, auto const&)
                            {
                                storage->settings.sideCurrentPage =
                                    winrt::to_string(sender.Source());

                                SendMessageW(appHwnd, WM_NOTIFY, 0, 0);
                            }});
    }

    controller.AcceleratorKeyPressed(
        {[=, this](auto const&, auto const& args) { accelerator_key_pressed(args); }});

    core.DocumentTitleChanged({[=, this](auto const&, auto const& args) { title(); }});

    core.FaviconChanged({[=, this](auto const&, auto const& args) { icon(); }});

    core.NavigationCompleted(
        {[=, this](auto const&, auto const& args) { SendMessage(appHwnd, WM_NOTIFY, 0, 0); }});
}

void WebView::gui_web_message_received(winrt::CoreWebView2WebMessageReceivedEventArgs const& args)
{
    auto webMessage{args.WebMessageAsJson()};

    if (!webMessage.empty())
    {
        auto j{nlohmann::json::parse(webMessage)};

        if (!j["mainHomepage"].empty())
        {
            auto s{j["mainHomepage"].get<std::string>()};

            if (s.starts_with("http://") || s.starts_with("https://"))
                storage->settings.mainHomepage = s;
            else
                storage->settings.mainHomepage = "https://" + s;
        }

        if (!j["sideHomepage"].empty())
        {
            auto s{j["sideHomepage"].get<std::string>()};

            if (s.starts_with("http://") || s.starts_with("https://"))
                storage->settings.sideHomepage = s;
            else
                storage->settings.sideHomepage = "https://" + s;
        }

        SendMessageW(appHwnd, WM_NOTIFY, 0, 0);
    };
}

void WebView::bar_web_message_received(winrt::CoreWebView2WebMessageReceivedEventArgs const& args)
{
    auto webMessage{args.WebMessageAsJson()};

    util::println(winrt::to_string(webMessage));

    if (!webMessage.empty())
    {
        auto j{nlohmann::json::parse(webMessage)};

        if (!j["mainCurrentPage"].empty())
        {
            auto s{j["mainCurrentPage"].get<std::string>()};

            if (s.starts_with("http://") || s.starts_with("https://"))
                storage->settings.mainCurrentPage = s;

            else
                storage->settings.mainCurrentPage = "https://" + s;

            SendMessageW(appHwnd, WM_NAVIGATEMAIN, 0, 0);
        }

        if (!j["sideCurrentPage"].empty())
        {
            auto s{j["sideCurrentPage"].get<std::string>()};

            if (s.starts_with("http://") || s.starts_with("https://"))
                storage->settings.sideCurrentPage = s;

            else
                storage->settings.sideCurrentPage = "https://" + s;

            SendMessageW(appHwnd, WM_NAVIGATESIDE, 0, 0);
        }

        if (!j["mainDevtools"].empty())
        {
            auto s{j["mainDevtools"].get<bool>()};

            if (s == true)
                SendMessageW(appHwnd, WM_DEVTOOLSMAIN, 0, 0);
        }

        if (!j["sideDevtools"].empty())
        {
            auto s{j["sideDevtools"].get<bool>()};

            if (s == true)
                SendMessageW(appHwnd, WM_DEVTOOLSSIDE, 0, 0);
        }
    };

    return;
}

void WebView::accelerator_key_pressed(winrt::CoreWebView2AcceleratorKeyPressedEventArgs const& args)
{
    auto keyEventKind = args.KeyEventKind();

    if (keyEventKind == winrt::CoreWebView2KeyEventKind::KeyDown ||
        keyEventKind == winrt::CoreWebView2KeyEventKind::SystemKeyDown)
    {
        auto virtualKey{args.VirtualKey()};

        switch (virtualKey)
        {
        case 19:
            args.Handled(true);
            SendMessageW(appHwnd, WM_KEYDOWN, VK_PAUSE, 0);

            break;

        case 76:
            args.Handled(true);
            SendMessageW(appHwnd, WM_KEYDOWN, 0x4C, 0);

            break;

        case 87:
            args.Handled(true);
            SendMessageW(appHwnd, WM_KEYDOWN, 0x57, 0);

            break;

        case 112:
            args.Handled(true);
            SendMessageW(appHwnd, WM_KEYDOWN, VK_F1, 0);

            break;

        case 113:
            args.Handled(true);
            SendMessageW(appHwnd, WM_KEYDOWN, VK_F2, 0);

            break;

        case 114:
            args.Handled(true);
            SendMessageW(appHwnd, WM_KEYDOWN, VK_F3, 0);

            break;

        case 115:
            args.Handled(true);
            SendMessageW(appHwnd, WM_KEYDOWN, VK_F4, 0);

            break;

        case 117:
            args.Handled(true);
            SendMessageW(appHwnd, WM_KEYDOWN, VK_F6, 0);

            break;

        case 118:
            args.Handled(true);
            SendMessageW(appHwnd, WM_KEYDOWN, VK_F7, 0);

            break;

        case 119:
            args.Handled(true);
            SendMessageW(appHwnd, WM_KEYDOWN, VK_F8, 0);

            break;

        case 120:
            args.Handled(true);
            SendMessageW(appHwnd, WM_KEYDOWN, VK_F9, 0);

            break;

        case 121:
            args.Handled(true);
            SendMessageW(appHwnd, WM_KEYDOWN, VK_F10, 0);

            break;

        case 122:
            args.Handled(true);
            SendMessageW(appHwnd, WM_KEYDOWN, VK_F11, 0);

            break;
        }
    }
}

void WebView::post_settings(nlohmann::json j)
{
    if (!core)
        return;

    core.PostWebMessageAsJson(util::to_wide(j.dump()));
}

void WebView::title()
{
    if (!core)
        return;

    auto title{core.DocumentTitle()};

    if (storage->settings.webviewGui)
    {
        if (name != "gui")
            return;

        if (!storage->settings.windowTopmost)
            SetWindowTextW(appHwnd, title.c_str());

        else
        {
            winrt::hstring add = title + winrt::hstring(L" [On Top]");
            SetWindowTextW(appHwnd, add.c_str());
        }
    }

    if (!storage->settings.webviewGui && !storage->settings.webviewSwapped)
    {
        if (name != "main")
            return;

        if (!storage->settings.windowTopmost)
            SetWindowTextW(appHwnd, title.c_str());

        else
        {
            winrt::hstring add = title + winrt::hstring(L" [On Top]");
            SetWindowTextW(appHwnd, add.c_str());
        }
    }

    if (!storage->settings.webviewGui && storage->settings.webviewSwapped)
    {
        if (name != "side")
            return;

        if (!storage->settings.windowTopmost)
            SetWindowTextW(appHwnd, title.c_str());

        else
        {
            winrt::hstring add = title + winrt::hstring(L" [On Top]");
            SetWindowTextW(appHwnd, add.c_str());
        }
    }
}

winrt::IAsyncAction WebView::icon()
{
    if (!core)
        co_return;

    auto icon{co_await core.GetFaviconAsync(winrt::CoreWebView2FaviconImageFormat::Png)};
    IUnknown* favicon = (IUnknown*)winrt::get_abi(icon);

    winrt::com_ptr<IStream> stream{nullptr};
    winrt::check_hresult(CreateStreamOverRandomAccessStream(favicon, IID_PPV_ARGS(stream.put())));

    Gdiplus::Bitmap iconBitmap(stream.get());
    HICON hIcon{};

    if (storage->settings.webviewGui)
    {
        if (name != "gui")
            co_return;

        SetClassLongPtrW(appHwnd, GCLP_HICONSM, (LONG_PTR)storage->application.hIcon);
        SetClassLongPtrW(appHwnd, GCLP_HICON, (LONG_PTR)storage->application.hIcon);
    }

    if (!storage->settings.webviewGui && !storage->settings.webviewSwapped)
    {
        if (name != "main")
            co_return;

        if (storage->settings.mainCurrentPage.contains("Airglow/gui") ||
            storage->settings.mainCurrentPage == "https://localhost:8000/")
        {
            SetClassLongPtrW(appHwnd, GCLP_HICONSM, (LONG_PTR)storage->application.hIcon);
            SetClassLongPtrW(appHwnd, GCLP_HICON, (LONG_PTR)storage->application.hIcon);

            co_return;
        }

        if (iconBitmap.GetHICON(&hIcon) == Gdiplus::Status::Ok)
        {
            SetClassLongPtrW(appHwnd, GCLP_HICONSM, (LONG_PTR)hIcon);
            SetClassLongPtrW(appHwnd, GCLP_HICON, (LONG_PTR)storage->application.hIcon);
        };
    }

    if (!storage->settings.webviewGui && storage->settings.webviewSwapped)
    {
        if (name != "side")
            co_return;

        if (storage->settings.sideCurrentPage.contains("Airglow/gui") ||
            storage->settings.sideCurrentPage == "https://localhost:8000/")
        {
            SetClassLongPtrW(appHwnd, GCLP_HICONSM, (LONG_PTR)storage->application.hIcon);
            SetClassLongPtrW(appHwnd, GCLP_HICON, (LONG_PTR)storage->application.hIcon);

            co_return;
        }

        if (iconBitmap.GetHICON(&hIcon) == Gdiplus::Status::Ok)
        {
            SetClassLongPtrW(appHwnd, GCLP_HICONSM, (LONG_PTR)hIcon);
            SetClassLongPtrW(appHwnd, GCLP_HICON, (LONG_PTR)storage->application.hIcon);
        };
    }

    DestroyIcon(hIcon);
}

void WebView::focus()
{
    if (!controller)
        return;

    controller.MoveFocus(winrt::CoreWebView2MoveFocusReason::Programmatic);
}
