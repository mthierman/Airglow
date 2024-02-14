// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#pragma once

#include <glow/glow.hxx>

#include "global.hxx"

struct Browser final : public glow::WebView
{
    using glow::WebView::WebView;

    virtual auto accelerator_key_pressed_handler(ICoreWebView2Controller* sender,
                                                 ICoreWebView2AcceleratorKeyPressedEventArgs* args)
        -> HRESULT override;
    virtual auto web_message_received_handler(ICoreWebView2* sender,
                                              ICoreWebView2WebMessageReceivedEventArgs* args)
        -> HRESULT override;
    virtual auto source_changed_handler(ICoreWebView2* sender,
                                        ICoreWebView2SourceChangedEventArgs* args)
        -> HRESULT override;
    virtual auto document_title_changed_handler(ICoreWebView2* sender, IUnknown* args)
        -> HRESULT override;
    virtual auto favicon_changed_handler(ICoreWebView2* sender, IUnknown* args) -> HRESULT override;
    virtual auto got_focus_handler(ICoreWebView2Controller* sender, IUnknown* args)
        -> HRESULT override;
    virtual auto lost_focus_handler(ICoreWebView2Controller* sender, IUnknown* args)
        -> HRESULT override;
    virtual auto move_focus_requested_handler(ICoreWebView2Controller* sender,
                                              ICoreWebView2MoveFocusRequestedEventArgs* args)
        -> ::HRESULT override;
    virtual auto zoom_factor_changed_handler(ICoreWebView2Controller* sender, IUnknown* args)
        -> HRESULT override;

    auto url(std::string compare) -> std::string;

    wil::unique_hicon m_hicon{};
    std::string m_favicon{};
    std::string m_source{};
    std::string m_title{};
    Keys m_keys{};
};
