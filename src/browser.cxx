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
    wil::com_ptr<ICoreWebView2AcceleratorKeyPressedEventArgs2> args2;
    args->QueryInterface(IID_PPV_ARGS(&args2));

    COREWEBVIEW2_KEY_EVENT_KIND kind;
    args2->get_KeyEventKind(&kind);

    if (args2)
    {
        if (kind == COREWEBVIEW2_KEY_EVENT_KIND_KEY_DOWN ||
            kind == COREWEBVIEW2_KEY_EVENT_KIND_SYSTEM_KEY_DOWN)
        {
            unsigned int key{};
            args2->get_VirtualKey(&key);

            switch (key)
            {
            case key::pause:
            {
                args2->put_Handled(TRUE);
                PostMessageA(m_parent, WM_KEYDOWN, key, 0);
                break;
            }

            case key::l:
            {
                args2->put_Handled(TRUE);
                PostMessageA(m_parent, WM_KEYDOWN, key, 0);
                break;
            }

            case key::w:
            {
                args2->put_Handled(TRUE);
                PostMessageA(m_parent, WM_KEYDOWN, key, 0);
                break;
            }

            case key::f1:
            {
                args2->put_Handled(TRUE);
                PostMessageA(m_parent, WM_KEYDOWN, key, 0);
                break;
            }

            case key::f2:
            {
                args2->put_Handled(TRUE);
                PostMessageA(m_parent, WM_KEYDOWN, key, 0);
                break;
            }

            case key::f3:
            {
                args2->put_Handled(TRUE);
                args2->put_IsBrowserAcceleratorKeyEnabled(FALSE);
                PostMessageA(m_parent, WM_KEYDOWN, key, 0);
                break;
            }

            case key::f4:
            {
                args2->put_Handled(TRUE);
                PostMessageA(m_parent, WM_KEYDOWN, key, 0);
                break;
            }

                // case key::f5:
                // {
                //     args2->put_Handled(FALSE);
                //     args2->put_IsBrowserAcceleratorKeyEnabled(TRUE);
                //     PostMessageA(m_parent, WM_KEYDOWN, VK_F5, 0);
                //     break;
                // }

            case key::f6:
            {
                args2->put_Handled(TRUE);
                PostMessageA(m_parent, WM_KEYDOWN, key, 0);
                break;
            }

            case key::f7:
            {
                args2->put_Handled(TRUE);
                PostMessageA(m_parent, WM_KEYDOWN, key, 0);
                break;
            }

            case key::f8:
            {
                args2->put_Handled(TRUE);
                PostMessageA(m_parent, WM_KEYDOWN, key, 0);
                break;
            }

            case key::f9:
            {
                args2->put_Handled(TRUE);
                PostMessageA(m_parent, WM_KEYDOWN, key, 0);
                break;
            }

                // case key::f10:
                // {
                //     args2->put_Handled(TRUE);
                //     PostMessageA(m_parent, WM_KEYDOWN, VK_F10, 0);
                //     break;
                // }

            case key::f11:
            {
                args2->put_Handled(TRUE);
                PostMessageA(m_parent, WM_KEYDOWN, VK_F11, 0);
                break;
            }

                // case key::f12:
                // {
                //     args2->put_Handled(FALSE);
                //     args2->put_IsBrowserAcceleratorKeyEnabled(TRUE);
                //     PostMessageA(m_parent, WM_KEYDOWN, VK_F12, 0);
                //     break;
                // }
            }
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
    navigate(url());
}

auto URLBrowser::navigation_completed_handler(ICoreWebView2* sender,
                                              ICoreWebView2NavigationCompletedEventArgs* args)
    -> HRESULT
{
    notify(m_parent, msg::url_created);

    return S_OK;
}

auto URLBrowser::web_message_received_handler(ICoreWebView2* sender,
                                              ICoreWebView2WebMessageReceivedEventArgs* args)
    -> HRESULT
{
    wil::unique_cotaskmem_string source;
    if (FAILED(args->get_Source(&source))) return S_OK;
    if (std::wstring_view(source.get()) != glow::text::widen(url())) return S_OK;

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

auto URLBrowser::url() -> std::string
{
#if _DEBUG
    std::string path{"https://localhost:8000/url/index.html"};
#else
    auto path{"file:///" + filesystem::known_folder().string() + "\\Airglow\\gui\\url\\index.html"};
#endif

    return path;
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

auto SideBrowser::source_changed_handler(ICoreWebView2* sender,
                                         ICoreWebView2SourceChangedEventArgs* args) -> HRESULT
{
    wil::unique_cotaskmem_string uriRaw;
    if (FAILED(sender->get_Source(&uriRaw))) return S_OK;

    auto uri{glow::text::narrow(uriRaw.get())};
    notify(m_parent, msg::receive_sideurl, uri);

    return S_OK;
}

auto SettingsBrowser::initialized() -> void
{
    m_webView.settings8->put_AreDefaultContextMenusEnabled(false);
    m_webView.settings8->put_IsZoomControlEnabled(false);
    // m_webView.core20->OpenDevToolsWindow();
    navigate(url());
}

auto SettingsBrowser::url() -> std::string
{
#if _DEBUG
    std::string path{"https://localhost:8000/settings/index.html"};
#else
    auto path{"file:///" + filesystem::known_folder().string() +
              "\\Airglow\\gui\\settings\\index.html"};
#endif

    return path;
}
