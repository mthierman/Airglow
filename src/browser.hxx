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

struct Browser : public glow::WebView<Browser>
{
    using glow::WebView<Browser>::WebView;

    virtual auto web_message_received_handler(ICoreWebView2* sender,
                                              ICoreWebView2WebMessageReceivedEventArgs* args)
        -> HRESULT override;
    virtual auto accelerator_key_pressed_handler(ICoreWebView2Controller* sender,
                                                 ICoreWebView2AcceleratorKeyPressedEventArgs* args)
        -> HRESULT override;
    virtual auto zoom_factor_changed_handler(ICoreWebView2Controller* sender, IUnknown* args)
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

    auto url(std::string compare) -> std::string;
};

struct URLBrowser final : public Browser
{
    using Browser::Browser;

    virtual auto initialized() -> void override;
};

struct SettingsBrowser final : public Browser
{
    using Browser::Browser;

    virtual auto initialized() -> void override;
};

struct MainBrowser final : public Browser
{
    using Browser::Browser;
};

struct SideBrowser final : public Browser
{
    using Browser::Browser;
};
