// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#include <airglow/url.hxx>

namespace airglow
{

auto URL::initialized() -> void
{
    m_webView.m_settings8->put_AreDefaultContextMenusEnabled(false);
    m_webView.m_settings8->put_IsZoomControlEnabled(true);
    m_webView.m_core20->OpenDevToolsWindow();
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

} // namespace airglow
