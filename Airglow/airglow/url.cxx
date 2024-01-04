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

auto URL::operator()(bool show) -> void
{
    glow::window::Window::operator()(show);
}

auto URL::initialized() -> void
{
    m_webView.m_settings8->put_AreDefaultContextMenusEnabled(false);
    m_webView.m_settings8->put_IsZoomControlEnabled(false);
}

auto URL::web_message_received_handler(ICoreWebView2* sender,
                                       ICoreWebView2WebMessageReceivedEventArgs* args) -> HRESULT
{
    // https://github.com/microsoft/wil/wiki/RAII-resource-wrappers
    // https://learn.microsoft.com/en-us/microsoft-edge/webview2/concepts/win32-api-conventions
    // https://learn.microsoft.com/en-us/microsoft-edge/webview2/reference/win32/icorewebview2?view=webview2-1.0.2210.55#add_webmessagereceived
    // https://learn.microsoft.com/en-us/microsoft-edge/webview2/reference/win32/icorewebview2webmessagereceivedeventargs?view=webview2-1.0.2210.55#trygetwebmessageasstring

    NMHDR nmhdr;
    nmhdr.code = CUSTOM_MAINURL;
    nmhdr.hwndFrom = m_hwnd.get();
    nmhdr.idFrom = m_id;

    wil::unique_cotaskmem_string source;
    auto getSource{args->get_Source(&source)};
    if (FAILED(getSource)) return S_OK;
    if (std::wstring_view(source.get()) != std::wstring_view(m_source)) return S_OK;

    wil::unique_cotaskmem_string messageRaw;
    auto asJson{args->get_WebMessageAsJson(&messageRaw)};
    // Was not a string message. Ignore.
    // if (asJson == E_INVALIDARG) return S_OK;
    if (FAILED(asJson)) return S_OK;

    std::string message{glow::text::narrow(messageRaw.get())};
    // glow::console::debug(message);
    auto parse{nlohmann::json::parse(message)};
    auto mainUrl{parse["mainUrl"].get<std::string>()};
    glow::console::debug(mainUrl);
    // m_webView.m_core20->Navigate(glow::text::widen(mainUrl).c_str());
    // SendMessageA(m_parent, WM_NOTIFY, 0, mainUrl);
    // SendMessageA(m_app, WM_NOTIFY, nmhdr.idFrom, std::bit_cast<LPARAM>(&nmhdr));

    // std::wstring message{messageRaw.get()};
    // if (std::wstring_view(messageRaw.get()) == std::wstring_view(L"mainUrl"))
    // {
    //     auto parse{nlohmann::json::parse(glow::text::narrow(messageRaw.get()))};
    //     if (parse["mainUrl"].empty()) return S_OK;
    //     auto mainUrl{parse["mainUrl"].get<std::string>()};
    //     m_webView.m_core20->Navigate(glow::text::widen(mainUrl).c_str());
    // }

    return S_OK;
}

} // namespace airglow
