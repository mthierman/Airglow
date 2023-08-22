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

    controller.AcceleratorKeyPressed(
        {[=, this](auto const&, auto const& args) { accelerator_key_pressed(args); }});

    core.DocumentTitleChanged({[=, this](auto const&, auto const& args) { title(); }});

    core.FaviconChanged({[=, this](auto const&, auto const& args) { icon(); }});

    core.DOMContentLoaded(
        {[=, this](auto const&, auto const&) { SendMessageW(appHwnd, WM_NOTIFY, 0, 0); }});

    if (name == "gui")
    {
        webviewSettings.IsZoomControlEnabled(false);

        core.WebMessageReceived(
            {[=, this](auto const&, auto const& args) { gui_web_message_received(args); }});
    }

    if (name == "bar")
    {
        webviewSettings.IsZoomControlEnabled(false);

        core.WebMessageReceived(
            {[=, this](auto const&, auto const& args) { bar_web_message_received(args); }});
    }

    if (name == "main")
    {
        webviewSettings.IsZoomControlEnabled(true);

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

        core.SourceChanged({[=, this](winrt::CoreWebView2 const& sender, auto const&)
                            {
                                storage->settings.sideCurrentPage =
                                    winrt::to_string(sender.Source());

                                SendMessageW(appHwnd, WM_NOTIFY, 0, 0);
                            }});
    }

    initial_navigation();
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
    if (!core || !controller)
        return;

    controller.MoveFocus(winrt::CoreWebView2MoveFocusReason::Programmatic);

    if (name == "bar")
        core.PostWebMessageAsString(L"focus");
}

void WebView::initial_navigation()
{
    auto url1{util::command_line().first};
    auto url2{util::command_line().second};

    if (name == "gui")
    {
        core.Navigate(winrt::to_hstring(util::settings_url()));
    }

    if (name == "bar")
    {
        core.Navigate(winrt::to_hstring(util::bar_url()));
    }

    if (name == "main")
    {
        if (!url1.empty())
            navigate(url1);

        if (url1.empty() && !storage->settings.mainHomepage.empty())
            navigate(storage->settings.mainHomepage);

        if (url1.empty() && storage->settings.mainHomepage.empty())
            core.Navigate(winrt::to_hstring(util::home_url()));
    }

    if (name == "side")
    {
        if (!url2.empty())
            navigate(url2);

        if (url2.empty() && !storage->settings.sideHomepage.empty())
            navigate(storage->settings.sideHomepage);

        if (url2.empty() && storage->settings.sideHomepage.empty())
            core.Navigate(winrt::to_hstring(util::home_url()));
    }
}

std::string WebView::parse_url(std::string s)
{
    if (s.empty())
        return s;

    if (s.starts_with("http://") || s.starts_with("https://"))
        return s;

    else
        return ("https://" + s);

    return s;
}

void WebView::navigate(std::string s)
{
    if (!core || s.empty())
        return;

    auto url{winrt::to_hstring(parse_url(s))};

    core.Navigate(url);

    return;
}

void WebView::gui_web_message_received(winrt::CoreWebView2WebMessageReceivedEventArgs const& args)
{
    auto webMessage{args.WebMessageAsJson()};

    if (!webMessage.empty())
    {
        auto j{nlohmann::json::parse(webMessage)};

        if (!j["mainHomepage"].empty())
        {
            storage->settings.mainHomepage = j["mainHomepage"].get<std::string>();
        }

        if (!j["sideHomepage"].empty())
        {
            storage->settings.sideHomepage = j["sideHomepage"].get<std::string>();
        }

        if (!j["clear"].empty())
        {
            if (j["clear"].get<bool>())
            {
                profile = core.Profile();
                profile.ClearBrowsingDataAsync();
            }
        }

        SendMessageW(appHwnd, WM_NOTIFY, 0, 0);
    };
}

void WebView::bar_web_message_received(winrt::CoreWebView2WebMessageReceivedEventArgs const& args)
{
    auto webMessage{args.WebMessageAsJson()};

    if (!webMessage.empty())
    {
        auto j{nlohmann::json::parse(webMessage)};

        if (!j["mainCurrentPage"].empty())
        {
            storage->settings.mainCurrentPage = j["mainCurrentPage"].get<std::string>();
            SendMessageW(appHwnd, WM_NAVIGATEMAIN, 0, 0);
        }

        if (!j["sideCurrentPage"].empty())
        {
            storage->settings.sideCurrentPage = j["sideCurrentPage"].get<std::string>();
            SendMessageW(appHwnd, WM_NAVIGATESIDE, 0, 0);
        }

        if (!j["mainDevtools"].empty())
        {
            if (j["mainDevtools"].get<bool>())
                SendMessageW(appHwnd, WM_DEVTOOLSMAIN, 0, 0);
        }

        if (!j["sideDevtools"].empty())
        {
            if (j["sideDevtools"].get<bool>())
                SendMessageW(appHwnd, WM_DEVTOOLSSIDE, 0, 0);
        }

        if (!j["mainHome"].empty())
        {
            if (j["mainHome"].get<bool>())
                SendMessageW(appHwnd, WM_HOMEMAIN, 0, 0);
        }

        if (!j["sideHome"].empty())
        {
            if (j["sideHome"].get<bool>())
                SendMessageW(appHwnd, WM_HOMESIDE, 0, 0);
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
