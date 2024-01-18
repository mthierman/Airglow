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

struct Browser : public glow::window::WebView<Browser>
{
    using glow::window::WebView<Browser>::WebView;

    virtual auto web_message_received_handler(ICoreWebView2* sender,
                                              ICoreWebView2WebMessageReceivedEventArgs* args)
        -> HRESULT override;

    virtual auto accelerator_key_pressed_handler(ICoreWebView2Controller* sender,
                                                 ICoreWebView2AcceleratorKeyPressedEventArgs* args)
        -> HRESULT override;
    virtual auto zoom_factor_changed_handler(ICoreWebView2Controller* sender, IUnknown* args)
        -> HRESULT override;
    virtual auto context_menu_requested_handler(ICoreWebView2* sender,
                                                ICoreWebView2ContextMenuRequestedEventArgs* args)
        -> HRESULT override;
};

struct URLBrowser final : public Browser
{
    using Browser::Browser;

    virtual auto initialized() -> void override;

    // virtual auto web_message_received_handler(ICoreWebView2* sender,
    //                                           ICoreWebView2WebMessageReceivedEventArgs* args)
    //     -> HRESULT override;
    virtual auto navigation_completed_handler(ICoreWebView2* sender,
                                              ICoreWebView2NavigationCompletedEventArgs* args)
        -> HRESULT override;

    auto url() -> std::string;
};

struct MainBrowser final : public Browser
{
    using Browser::Browser;

    virtual auto source_changed_handler(ICoreWebView2* sender,
                                        ICoreWebView2SourceChangedEventArgs* args)
        -> HRESULT override;
};

struct SideBrowser final : public Browser
{
    using Browser::Browser;

    virtual auto source_changed_handler(ICoreWebView2* sender,
                                        ICoreWebView2SourceChangedEventArgs* args)
        -> HRESULT override;
};

struct SettingsBrowser final : public Browser
{
    using Browser::Browser;

    virtual auto initialized() -> void override;

    auto url() -> std::string;
};
