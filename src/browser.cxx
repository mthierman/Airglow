// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#include "browser.hxx"

auto Browser::web_message_received_handler(ICoreWebView2* sender,
                                           ICoreWebView2WebMessageReceivedEventArgs* args)
    -> HRESULT
{
    wil::unique_cotaskmem_string source;
    args->get_Source(&source);

    if (!(std::wstring_view(source.get()) == glow::text::to_utf16(url("url")) ||
          std::wstring_view(source.get()) == glow::text::to_utf16(url("settings"))))
    {
        log(glow::text::to_utf8(source.get()));
    }

    wil::unique_cotaskmem_string message;
    if (FAILED(args->get_WebMessageAsJson(&message))) { return S_OK; }

    notify(m_parent, msg::web_message_received, glow::text::to_utf8(message.get()));

    return S_OK;
}

auto Browser::accelerator_key_pressed_handler(ICoreWebView2Controller* sender,
                                              ICoreWebView2AcceleratorKeyPressedEventArgs* args)
    -> HRESULT
{
    wil::com_ptr<ICoreWebView2AcceleratorKeyPressedEventArgs2> args2;
    args->QueryInterface(IID_PPV_ARGS(&args2));
    if (!args2) { return S_OK; }

    COREWEBVIEW2_KEY_EVENT_KIND kind;
    args2->get_KeyEventKind(&kind);

    if (kind == COREWEBVIEW2_KEY_EVENT_KIND_KEY_DOWN ||
        kind == COREWEBVIEW2_KEY_EVENT_KIND_SYSTEM_KEY_DOWN)
    {
        Keys keys;

        unsigned int key{};
        args2->get_VirtualKey(&key);

        int lParam{};
        args2->get_KeyEventLParam(&lParam);

        if (keys.set.contains(key))
        {
            args2->put_Handled(TRUE);
            args2->put_IsBrowserAcceleratorKeyEnabled(FALSE);

            if (key == VK_F10) { PostMessageA(m_parent, WM_SYSKEYDOWN, key, lParam); }

            else if (key == 0x30)
            {
                if (GetKeyState(VK_CONTROL) & 0x8000)
                {
                    m_webView.controller4->put_ZoomFactor(1.0);
                }
            }

            else { PostMessageA(m_parent, WM_KEYDOWN, key, lParam); }
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

auto Browser::document_title_changed_handler(ICoreWebView2* sender, IUnknown* args) -> HRESULT
{
    wil::unique_cotaskmem_string documentTitle;
    if (FAILED(m_webView.core20->get_DocumentTitle(&documentTitle))) { return S_OK; }

    m_documentTitle = glow::text::to_utf8(documentTitle.get());

    notify(m_parent, msg::title_changed);

    return S_OK;
}

auto Browser::favicon_changed_handler(ICoreWebView2* sender, IUnknown* args) -> HRESULT
{
    wil::unique_cotaskmem_string favicon;
    if (FAILED(m_webView.core20->get_FaviconUri(&favicon))) { return S_OK; }

    m_faviconUrl.assign(glow::text::to_utf8(favicon.get()));

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

auto Browser::url(std::string page) -> std::string
{
#if defined(_DEBUG)
    if (page.contains("url")) { return "https://localhost:8000/url/index.html"; }

    else if (page.contains("settings")) { return "https://localhost:8000/settings/index.html"; }
#else
    if (page.contains("url"))
    {
        return "file:///" + glow::filesystem::app_path().generic_string() + "/gui/url/index.html";
    }

    else if (page.contains("settings"))
    {
        return "file:///" + glow::filesystem::app_path().generic_string() +
               "/gui/settings/index.html";
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

auto MainBrowser::source_changed_handler(ICoreWebView2* sender,
                                         ICoreWebView2SourceChangedEventArgs* args) -> HRESULT
{
    wil::unique_cotaskmem_string source;
    if (FAILED(sender->get_Source(&source))) { return S_OK; }

    notify(m_parent, msg::source_changed,
           nlohmann::json{{"first", glow::text::to_utf8(source.get())}}.dump());

    return S_OK;
}

auto SideBrowser::source_changed_handler(ICoreWebView2* sender,
                                         ICoreWebView2SourceChangedEventArgs* args) -> HRESULT
{
    wil::unique_cotaskmem_string source;
    if (FAILED(sender->get_Source(&source))) { return S_OK; }

    notify(m_parent, msg::source_changed,
           nlohmann::json{{"second", glow::text::to_utf8(source.get())}}.dump());

    return S_OK;
}
