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
    if (FAILED(args->QueryInterface(IID_PPV_ARGS(&args2)))) { return S_OK; }
    if (!args2) { return S_OK; }

    COREWEBVIEW2_KEY_EVENT_KIND kind;
    if (FAILED(args2->get_KeyEventKind(&kind))) { return S_OK; }

    if (kind == COREWEBVIEW2_KEY_EVENT_KIND_KEY_DOWN ||
        kind == COREWEBVIEW2_KEY_EVENT_KIND_SYSTEM_KEY_DOWN)
    {
        Keys keys;

        unsigned int key{};
        if (FAILED(args2->get_VirtualKey(&key))) { return S_OK; }

        int lParam{};
        if (FAILED(args2->get_KeyEventLParam(&lParam))) { return S_OK; }

        if (keys.set.contains(key))
        {
            if (FAILED(args2->put_Handled(TRUE))) { return S_OK; }
            if (FAILED(args2->put_IsBrowserAcceleratorKeyEnabled(FALSE))) { return S_OK; }

            if (key == VK_F10) { PostMessageA(m_parent, WM_SYSKEYDOWN, key, lParam); }

            else if (key == 0x30)
            {
                if (GetKeyState(VK_CONTROL) & 0x8000) { zoom(1.0); }
            }

            else { PostMessageA(m_parent, WM_KEYDOWN, key, lParam); }
        }
    }

    return S_OK;
}

auto Browser::web_message_received_handler(ICoreWebView2* sender,
                                           ICoreWebView2WebMessageReceivedEventArgs* args)
    -> HRESULT
{
    wil::unique_cotaskmem_string source;
    if (FAILED(args->get_Source(&source))) { return S_OK; }

    if (!(std::wstring_view(source.get()) == glow::wstring(url("url")) ||
          std::wstring_view(source.get()) == glow::wstring(url("settings"))))
    {
        return S_OK;
    }

    wil::unique_cotaskmem_string message;
    if (FAILED(args->get_WebMessageAsJson(&message))) { return S_OK; }

    notify(m_parent, msg::web_message_received, glow::string(message.get()));

    return S_OK;
}

auto Browser::source_changed_handler(ICoreWebView2* sender,
                                     ICoreWebView2SourceChangedEventArgs* args) -> HRESULT
{
    wil::unique_cotaskmem_string source;
    if (FAILED(sender->get_Source(&source))) { return S_OK; }

    m_source.assign(glow::string(source.get()));

    notify(m_parent, msg::source_changed);

    return S_OK;
}

auto Browser::document_title_changed_handler(ICoreWebView2* sender, IUnknown* args) -> HRESULT
{
    wil::unique_cotaskmem_string documentTitle;
    if (FAILED(m_webView.core20->get_DocumentTitle(&documentTitle))) { return S_OK; }

    m_documentTitle.assign(glow::string(documentTitle.get()));

    notify(m_parent, msg::title_changed);

    return S_OK;
}

auto Browser::favicon_changed_handler(ICoreWebView2* sender, IUnknown* args) -> HRESULT
{
    wil::unique_cotaskmem_string favicon;
    if (FAILED(m_webView.core20->get_FaviconUri(&favicon))) { return S_OK; }

    m_faviconUrl.assign(glow::string(favicon.get()));

    if (FAILED(m_webView.core20->GetFavicon(
            COREWEBVIEW2_FAVICON_IMAGE_FORMAT_PNG,
            Microsoft::WRL::Callback<ICoreWebView2GetFaviconCompletedHandler>(
                [=, this](HRESULT errorCode, IStream* iconStream) -> HRESULT
                {
                    if (FAILED(errorCode)) { return S_OK; }

                    Gdiplus::Bitmap iconBitmap(iconStream);

                    iconBitmap.GetHICON(&m_favicon);

                    notify(m_parent, msg::favicon_changed);

                    return S_OK;
                })
                .Get())))
    {
        return S_OK;
    }

    return S_OK;
}

auto Browser::got_focus_handler(ICoreWebView2Controller* sender, IUnknown* args) -> HRESULT
{
    notify(m_parent, msg::focus_changed);

    return S_OK;
}

auto Browser::lost_focus_handler(ICoreWebView2Controller* sender, IUnknown* args) -> HRESULT
{
    return S_OK;
}

auto Browser::move_focus_requested_handler(ICoreWebView2Controller* sender,
                                           ICoreWebView2MoveFocusRequestedEventArgs* args)
    -> HRESULT
{
    COREWEBVIEW2_MOVE_FOCUS_REASON reason;
    if (FAILED(args->get_Reason(&reason))) { return S_OK; }

    if (reason == COREWEBVIEW2_MOVE_FOCUS_REASON_NEXT)
    {
        focus(COREWEBVIEW2_MOVE_FOCUS_REASON_NEXT);
    }

    else if (reason == COREWEBVIEW2_MOVE_FOCUS_REASON_PREVIOUS)
    {
        focus(COREWEBVIEW2_MOVE_FOCUS_REASON_PREVIOUS);
    }

    args->put_Handled(TRUE);

    return S_OK;
}

auto Browser::zoom_factor_changed_handler(ICoreWebView2Controller* sender, IUnknown* args)
    -> HRESULT
{
    double zoomFactor;
    if (FAILED(sender->get_ZoomFactor(&zoomFactor))) { return S_OK; }

    zoom(zoomFactor);

    return S_OK;
}

auto Browser::url(std::string page) -> std::string
{
#if defined(_DEBUG)
    if (page.contains("url")) { return "https://localhost:8000/url/index.html"; }

    else if (page.contains("settings")) { return "https://localhost:8000/settings/index.html"; }
#else
    if (page.contains("url"))
    {
        return "file:///" + glow::app_path().generic_string() + "/gui/url/index.html";
    }

    else if (page.contains("settings"))
    {
        return "file:///" + glow::app_path().generic_string() + "/gui/settings/index.html";
    }
#endif

    else { return {}; }
}

auto URLBrowser::initialized() -> void
{
    // m_webView.core20->OpenDevToolsWindow();
    navigate(url("url"));
}

auto SettingsBrowser::initialized() -> void
{
    // m_webView.core20->OpenDevToolsWindow();
    navigate(url("settings"));
}
