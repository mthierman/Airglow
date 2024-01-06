// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#include <airglow/webview.hxx>

namespace airglow::webview
{

auto WebView::accelerator_key_pressed_handler(ICoreWebView2Controller* sender,
                                              ICoreWebView2AcceleratorKeyPressedEventArgs* args)
    -> HRESULT
{
    COREWEBVIEW2_KEY_EVENT_KIND kind{};
    args->get_KeyEventKind(&kind);

    if (kind == COREWEBVIEW2_KEY_EVENT_KIND_KEY_DOWN ||
        kind == COREWEBVIEW2_KEY_EVENT_KIND_SYSTEM_KEY_DOWN)
    {
        UINT key{0};
        args->get_VirtualKey(&key);

        switch (key)
        {
        case 19:
            args->put_Handled(TRUE);
            SendMessageA(m_parent, WM_KEYDOWN, VK_PAUSE, 0);
            break;

        case 76:
            args->put_Handled(TRUE);
            SendMessageA(m_parent, WM_KEYDOWN, 0x4C, 0);

            break;

        case 87:
            args->put_Handled(TRUE);
            SendMessageA(m_parent, WM_KEYDOWN, 0x57, 0);

            break;

        case 112:
            args->put_Handled(TRUE);
            SendMessageA(m_parent, WM_KEYDOWN, VK_F1, 0);

            break;

        case 113:
            args->put_Handled(TRUE);
            SendMessageA(m_parent, WM_KEYDOWN, VK_F2, 0);

            break;

        case 114:
            args->put_Handled(TRUE);
            SendMessageA(m_parent, WM_KEYDOWN, VK_F3, 0);

            break;

        case 115:
            args->put_Handled(TRUE);
            SendMessageA(m_parent, WM_KEYDOWN, VK_F4, 0);

            break;

        case 117:
            args->put_Handled(TRUE);
            SendMessageA(m_parent, WM_KEYDOWN, VK_F6, 0);

            break;

        case 118:
            args->put_Handled(TRUE);
            SendMessageA(m_parent, WM_KEYDOWN, VK_F7, 0);

            break;

        case 119:
            args->put_Handled(TRUE);
            SendMessageA(m_parent, WM_KEYDOWN, VK_F8, 0);

            break;

        case 120:
            args->put_Handled(TRUE);
            SendMessageA(m_parent, WM_KEYDOWN, VK_F9, 0);

            break;

        case 121:
            args->put_Handled(TRUE);
            SendMessageA(m_parent, WM_KEYDOWN, VK_F10, 0);

            break;

        case 122:
            args->put_Handled(TRUE);
            SendMessageA(m_parent, WM_KEYDOWN, VK_F11, 0);

            break;
        }
    }

    return S_OK;
}

auto WebView::zoom_factor_changed_handler(ICoreWebView2Controller* sender, IUnknown* args)
    -> HRESULT
{
    double zoomFactor;
    sender->get_ZoomFactor(&zoomFactor);
    m_webView.m_controller4->put_ZoomFactor(zoomFactor);

    return S_OK;
}

auto URL::initialized() -> void
{
    m_webView.m_settings8->put_AreDefaultContextMenusEnabled(false);
    m_webView.m_settings8->put_IsZoomControlEnabled(false);
    // m_webView.m_core20->OpenDevToolsWindow();
}

auto URL::web_message_received_handler(ICoreWebView2* sender,
                                       ICoreWebView2WebMessageReceivedEventArgs* args) -> HRESULT
{
    // https://github.com/microsoft/wil/wiki/RAII-resource-wrappers
    // https://learn.microsoft.com/en-us/microsoft-edge/webview2/concepts/win32-api-conventions
    // https://learn.microsoft.com/en-us/microsoft-edge/webview2/reference/win32/icorewebview2?view=webview2-1.0.2210.55#add_webmessagereceived
    // https://learn.microsoft.com/en-us/microsoft-edge/webview2/reference/win32/icorewebview2webmessagereceivedeventargs?view=webview2-1.0.2210.55#trygetwebmessageasstring
    // https://github.com/nlohmann/json/issues/1592

    // Create notification message structure

    // Check message source
    wil::unique_cotaskmem_string source;
    if (FAILED(args->get_Source(&source))) return S_OK;
    if (std::wstring_view(source.get()) != std::wstring_view(m_source)) return S_OK;

    // Get message as JSON
    wil::unique_cotaskmem_string messageRaw;
    auto asJson{args->get_WebMessageAsJson(&messageRaw)};
    if (asJson == E_INVALIDARG) return S_OK;
    if (FAILED(asJson)) return S_OK;

    // Parse the JSON and send the URL
    std::string message{glow::text::narrow(messageRaw.get())};
    try
    {
        auto parseMsg{nlohmann::json::parse(message)};

        NotificationMsg nMsg;
        nMsg.nmhdr.hwndFrom = m_hwnd.get();
        nMsg.nmhdr.idFrom = m_id;

        if (parseMsg.contains("mainUrl"))
        {
            auto mainUrl{parseMsg["mainUrl"].get<std::string>()};
            // glow::console::debug(mainUrl);
            nMsg.nmhdr.code = CUSTOM_POST_MAINURL;
            nMsg.message = mainUrl;
            SendMessageA(m_parent, WM_NOTIFY, nMsg.nmhdr.idFrom, std::bit_cast<LPARAM>(&nMsg));
        }

        if (parseMsg.contains("sideUrl"))
        {
            auto sideUrl{parseMsg["sideUrl"].get<std::string>()};
            // glow::console::debug(sideUrl);
            nMsg.nmhdr.code = CUSTOM_POST_SIDEURL;
            nMsg.message = sideUrl;
            SendMessageA(m_parent, WM_NOTIFY, nMsg.nmhdr.idFrom, std::bit_cast<LPARAM>(&nMsg));
        }

        if (parseMsg.contains("height"))
        {
            auto height{parseMsg["height"].get<int>()};
            // glow::console::debug(std::to_string(height));
            nMsg.nmhdr.code = CUSTOM_POST_HEIGHT;
            nMsg.message = std::to_string(height);
            SendMessageA(m_parent, WM_NOTIFY, nMsg.nmhdr.idFrom, std::bit_cast<LPARAM>(&nMsg));
        }
    }
    catch (const nlohmann::json::parse_error& e)
    {
        glow::console::debug(e.what());
    }

    return S_OK;
}

auto Main::source_changed_handler(ICoreWebView2* sender, ICoreWebView2SourceChangedEventArgs* args)
    -> HRESULT
{
    glow::console::debug("navigation completed..");
    wil::unique_cotaskmem_string uriRaw;
    sender->get_Source(&uriRaw);

    auto uri{glow::text::narrow(uriRaw.get())};
    glow::console::debug(uri);
    NotificationMsg nMsg;
    nMsg.nmhdr.hwndFrom = m_hwnd.get();
    nMsg.nmhdr.idFrom = m_id;
    nMsg.nmhdr.code = CUSTOM_RECEIVE_MAINURL;
    nMsg.message = uri;

    SendMessage(m_parent, WM_NOTIFY, 0, std::bit_cast<LPARAM>(&nMsg));

    return S_OK;
}

auto Main::navigation_starting_handler(ICoreWebView2* sender,
                                       ICoreWebView2NavigationStartingEventArgs* args) -> HRESULT
{
    glow::console::debug("navigation completed..");
    wil::unique_cotaskmem_string uriRaw;
    sender->get_Source(&uriRaw);

    auto uri{glow::text::narrow(uriRaw.get())};
    glow::console::debug(uri);
    NotificationMsg nMsg;
    nMsg.nmhdr.hwndFrom = m_hwnd.get();
    nMsg.nmhdr.idFrom = m_id;
    nMsg.nmhdr.code = CUSTOM_RECEIVE_MAINURL;
    nMsg.message = uri;

    SendMessage(m_parent, WM_NOTIFY, 0, std::bit_cast<LPARAM>(&nMsg));

    return S_OK;
}

auto Side::source_changed_handler(ICoreWebView2* sender, ICoreWebView2SourceChangedEventArgs* args)
    -> HRESULT
{
    glow::console::debug("navigation completed..");
    wil::unique_cotaskmem_string uriRaw;
    sender->get_Source(&uriRaw);

    auto uri{glow::text::narrow(uriRaw.get())};
    glow::console::debug(uri);
    NotificationMsg nMsg;
    nMsg.nmhdr.hwndFrom = m_hwnd.get();
    nMsg.nmhdr.idFrom = m_id;
    nMsg.nmhdr.code = CUSTOM_RECEIVE_SIDEURL;
    nMsg.message = uri;

    SendMessage(m_parent, WM_NOTIFY, 0, std::bit_cast<LPARAM>(&nMsg));

    return S_OK;
}

auto Side::navigation_starting_handler(ICoreWebView2* sender,
                                       ICoreWebView2NavigationStartingEventArgs* args) -> HRESULT
{
    glow::console::debug("navigation completed..");
    wil::unique_cotaskmem_string uriRaw;
    sender->get_Source(&uriRaw);

    auto uri{glow::text::narrow(uriRaw.get())};
    glow::console::debug(uri);
    NotificationMsg nMsg;
    nMsg.nmhdr.hwndFrom = m_hwnd.get();
    nMsg.nmhdr.idFrom = m_id;
    nMsg.nmhdr.code = CUSTOM_RECEIVE_SIDEURL;
    nMsg.message = uri;

    SendMessage(m_parent, WM_NOTIFY, 0, std::bit_cast<LPARAM>(&nMsg));

    return S_OK;
}

Browser::Browser(HWND app, std::string className) : glow::window::Window(className) { m_app = app; }

auto Browser::operator()(bool show) -> void
{
    glow::window::Window::operator()(show);
    dwm_caption_color(false);
    dwm_dark_mode(true);
    dwm_system_backdrop(DWM_SYSTEMBACKDROP_TYPE::DWMSBT_MAINWINDOW);

    m_browser1 = std::make_unique<airglow::webview::Main>(+Browsers::browser1, m_hwnd.get());
    (*m_browser1)();
    m_browser1->create_webview();

    m_browser2 = std::make_unique<airglow::webview::Side>(+Browsers::browser2, m_hwnd.get());
    (*m_browser2)();
    m_browser2->create_webview();

    m_url = std::make_unique<airglow::webview::URL>(+Browsers::url, m_hwnd.get(),
                                                    "https://localhost:8000/url/index.html");
    (*m_url)();
    m_url->create_webview();
}

auto Browser::handle_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    switch (uMsg)
    {
    case WM_CREATE: return on_create(hWnd, wParam, lParam);
    case WM_CLOSE: return on_close(hWnd, wParam, lParam);
    case WM_NOTIFY: return on_notify(hWnd, wParam, lParam);
    case WM_SIZE: return on_size(hWnd, wParam, lParam);
    }

    return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

auto Browser::on_create(HWND hWnd, WPARAM wParam, LPARAM lParam) -> int
{
    NMHDR nmhdr;
    nmhdr.code = CUSTOM_CREATE_WINDOW;
    nmhdr.hwndFrom = m_hwnd.get();
    nmhdr.idFrom = m_id;
    SendMessageA(m_app, WM_NOTIFY, nmhdr.idFrom, std::bit_cast<LPARAM>(&nmhdr));

    return 0;
}

auto Browser::on_close(HWND hWnd, WPARAM wParam, LPARAM lParam) -> int
{
    NMHDR nmhdr;
    nmhdr.code = CUSTOM_CLOSE_WINDOW;
    nmhdr.hwndFrom = m_hwnd.get();
    nmhdr.idFrom = m_id;
    SendMessageA(m_app, WM_NOTIFY, nmhdr.idFrom, std::bit_cast<LPARAM>(&nmhdr));

    m_hwnd.reset();

    return 0;
}

auto Browser::on_notify(HWND hWnd, WPARAM wParam, LPARAM lParam) -> int
{
    auto nMsg{*std::bit_cast<NotificationMsg*>(lParam)};

    switch (nMsg.nmhdr.code)
    {

    case CUSTOM_POST_HEIGHT:
    {
        m_bar = std::stoi(nMsg.message);
        SendMessageA(hWnd, WM_SIZE, 0, 0);
        break;
    }

    case CUSTOM_POST_MAINURL:
    {
        if (m_browser1) m_browser1->navigate(nMsg.message);
        break;
    }

    case CUSTOM_POST_SIDEURL:
    {
        if (m_browser2) m_browser2->navigate(nMsg.message);
        break;
    }

    case CUSTOM_RECEIVE_MAINURL:
    {
        nlohmann::json j{{"mainUrl", nMsg.message}};
        if (m_url) m_url->post_json(glow::text::widen(j.dump()).c_str());
        break;
    }

    case CUSTOM_RECEIVE_SIDEURL:
    {
        nlohmann::json j{{"sideUrl", nMsg.message}};
        if (m_url) m_url->post_json(glow::text::widen(j.dump()).c_str());
        break;
    }
    }

    return 0;
}

auto Browser::on_size(HWND hWnd, WPARAM wParam, LPARAM lParam) -> int
{
    WindowDimensions dimensions;
    GetClientRect(hWnd, &dimensions.rect);
    dimensions.scale = scale();
    dimensions.hwnd = hWnd;
    EnumChildWindows(hWnd, EnumChildProc, std::bit_cast<LPARAM>(&dimensions));
    Sleep(1);

    // https://stackoverflow.com/questions/7771142/non-blocking-sleep-timer-in-c
    // https://learn.microsoft.com/en-us/windows/win32/sync/using-waitable-timer-objects?redirectedfrom=MSDN
    // https://learn.microsoft.com/en-us/windows/win32/sync/using-a-waitable-timer-with-an-asynchronous-procedure-call?redirectedfrom=MSDN
    // https://learn.microsoft.com/en-us/windows/win32/winmsg/using-timers
    // SetTimer(hWnd, 0, 100, TIMERPROC(0));

    return 0;
}

auto CALLBACK Browser::EnumChildProc(HWND hWnd, LPARAM lParam) -> BOOL
{
    auto dimensions{*std::bit_cast<WindowDimensions*>(lParam)};
    auto self{glow::window::instance<Browser>(dimensions.hwnd)};

    if (self)
    {
        auto gwlId{static_cast<int64_t>(GetWindowLongPtrA(hWnd, GWL_ID))};

        auto r = &dimensions.rect;
        auto width{r->right - r->left};
        auto height{r->bottom - r->top};

        auto border{static_cast<int>(s_border * dimensions.scale)};
        // auto bar{static_cast<int>(s_bar * dimensions.scale)};
        auto bar{static_cast<int>(self->m_bar * dimensions.scale)};

        auto defer{true};

        auto hdwp{BeginDeferWindowPos(4)};

        if (gwlId == +Browsers::browser1)
            if (hdwp)
                hdwp = DeferWindowPos(hdwp, hWnd, nullptr, 0, 0, (width / 2) - border, height - bar,
                                      SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOOWNERZORDER |
                                          SWP_NOREDRAW | SWP_NOCOPYBITS);

        if (gwlId == +Browsers::browser2)
            if (hdwp)
                hdwp = DeferWindowPos(hdwp, hWnd, nullptr, (width / 2) + border, 0,
                                      (width / 2) - border, height - bar,
                                      SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOOWNERZORDER |
                                          SWP_NOREDRAW | SWP_NOCOPYBITS);

        if (gwlId == +Browsers::url)
            if (hdwp)
                hdwp = DeferWindowPos(hdwp, hWnd, nullptr, 0, r->bottom - bar, width, bar,
                                      SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOOWNERZORDER |
                                          SWP_NOREDRAW | SWP_NOCOPYBITS);

        if (hdwp) EndDeferWindowPos(hdwp);
    }

    return TRUE;
}

} // namespace airglow::webview
