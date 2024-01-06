// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#pragma once

#include <bit>

#include <glow/console.hxx>
#include <glow/text.hxx>
#include <glow/window.hxx>

#include <airglow/definitions.hxx>

namespace airglow::webview
{

struct WebView;
struct URL;
struct Main;
struct Side;
struct Settings;

struct WebView : public glow::window::WebView
{
    using glow::window::WebView::WebView;

    virtual auto accelerator_key_pressed_handler(ICoreWebView2Controller* sender,
                                                 ICoreWebView2AcceleratorKeyPressedEventArgs* args)
        -> HRESULT override;
    virtual auto zoom_factor_changed_handler(ICoreWebView2Controller* sender, IUnknown* args)
        -> HRESULT override;
};

struct URL final : public WebView
{
    using WebView::WebView;

    auto initialized() -> void override;

    auto web_message_received_handler(ICoreWebView2* sender,
                                      ICoreWebView2WebMessageReceivedEventArgs* args)
        -> HRESULT override;

    std::wstring m_source{L"https://localhost:8000/url/index.html"};
};

struct Main final : public WebView
{
    using WebView::WebView;

    virtual auto source_changed_handler(ICoreWebView2* sender,
                                        ICoreWebView2SourceChangedEventArgs* args)
        -> HRESULT override;
    virtual auto navigation_starting_handler(ICoreWebView2* sender,
                                             ICoreWebView2NavigationStartingEventArgs* args)
        -> HRESULT override;
};

struct Side final : public WebView
{
    using WebView::WebView;

    virtual auto source_changed_handler(ICoreWebView2* sender,
                                        ICoreWebView2SourceChangedEventArgs* args)
        -> HRESULT override;
    virtual auto navigation_starting_handler(ICoreWebView2* sender,
                                             ICoreWebView2NavigationStartingEventArgs* args)
        -> HRESULT override;
};

struct Settings final : public WebView
{
    using WebView::WebView;
};

} // namespace airglow::webview
