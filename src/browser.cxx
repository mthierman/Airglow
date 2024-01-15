// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#include "browser.hxx"

auto Browser::accelerator_key_pressed_handler(ICoreWebView2Controller* sender,
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

auto Browser::zoom_factor_changed_handler(ICoreWebView2Controller* sender, IUnknown* args)
    -> HRESULT
{
    double zoomFactor;
    sender->get_ZoomFactor(&zoomFactor);
    m_webView.controller4->put_ZoomFactor(zoomFactor);

    return S_OK;
}

auto Browser::context_menu_requested_handler(ICoreWebView2* sender,
                                             ICoreWebView2ContextMenuRequestedEventArgs* args)
    -> HRESULT
{
    // https://learn.microsoft.com/en-us/microsoft-edge/webview2/how-to/context-menus?tabs=cpp#example-adding-a-custom-context-menu

    return S_OK;
}

auto URLBrowser::initialized() -> void
{
    m_webView.settings8->put_AreDefaultContextMenusEnabled(false);
    m_webView.settings8->put_IsZoomControlEnabled(false);
    // m_webView.core20->OpenDevToolsWindow();
}

auto URLBrowser::web_message_received_handler(ICoreWebView2* sender,
                                              ICoreWebView2WebMessageReceivedEventArgs* args)
    -> HRESULT
{
    wil::unique_cotaskmem_string source;
    if (FAILED(args->get_Source(&source))) return S_OK;
    if (std::wstring_view(source.get()) != glow::text::widen(m_url)) return S_OK;

    wil::unique_cotaskmem_string messageRaw;
    auto asJson{args->get_WebMessageAsJson(&messageRaw)};
    if (asJson == E_INVALIDARG) return S_OK;
    if (FAILED(asJson)) return S_OK;

    std::string narrowMessage{glow::text::narrow(messageRaw.get())};
    auto json{nlohmann::json::parse(narrowMessage)};

    if (json.contains("height"))
    {
        auto message{json["height"].get<int>()};
        notify(m_parent, msg::url_height, std::to_string(message));
    }

    if (json.contains("mainUrl"))
    {
        auto message{json["mainUrl"].get<std::string>()};
        notify(m_parent, msg::post_mainurl, message);
    }

    if (json.contains("sideUrl"))
    {
        auto message{json["sideUrl"].get<std::string>()};
        notify(m_parent, msg::post_sideurl, message);
    }

    return S_OK;
}

auto MainBrowser::source_changed_handler(ICoreWebView2* sender,
                                         ICoreWebView2SourceChangedEventArgs* args) -> HRESULT
{
    wil::unique_cotaskmem_string uriRaw;
    if (FAILED(sender->get_Source(&uriRaw))) return S_OK;

    auto uri{glow::text::narrow(uriRaw.get())};
    notify(m_parent, msg::receive_mainurl, uri);

    return S_OK;
}

auto MainBrowser::navigation_starting_handler(ICoreWebView2* sender,
                                              ICoreWebView2NavigationStartingEventArgs* args)
    -> HRESULT
{
    wil::unique_cotaskmem_string uriRaw;
    if (FAILED(sender->get_Source(&uriRaw))) return S_OK;

    auto uri{glow::text::narrow(uriRaw.get())};
    notify(m_parent, msg::receive_mainurl, uri);

    return S_OK;
}

auto SideBrowser::source_changed_handler(ICoreWebView2* sender,
                                         ICoreWebView2SourceChangedEventArgs* args) -> HRESULT
{
    wil::unique_cotaskmem_string uriRaw;
    if (FAILED(sender->get_Source(&uriRaw))) return S_OK;

    auto uri{glow::text::narrow(uriRaw.get())};
    notify(m_parent, msg::receive_sideurl, uri);

    return S_OK;
}

auto SideBrowser::navigation_starting_handler(ICoreWebView2* sender,
                                              ICoreWebView2NavigationStartingEventArgs* args)
    -> HRESULT
{
    wil::unique_cotaskmem_string uriRaw;
    if (FAILED(sender->get_Source(&uriRaw))) return S_OK;

    auto uri{glow::text::narrow(uriRaw.get())};
    notify(m_parent, msg::receive_sideurl, uri);

    return S_OK;
}
