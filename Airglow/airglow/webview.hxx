// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#pragma once

#include <bit>
#include <set>

#include <glow/console.hxx>
#include <glow/filesystem.hxx>
#include <glow/text.hxx>
#include <glow/window.hxx>

#include <airglow/definitions.hxx>
#include <airglow/webview.hxx>

namespace airglow::webview
{

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

struct Browser final : public glow::window::Window
{
    using glow::window::Window::Window;

    Browser(HWND app, std::string className);

    virtual auto operator()(bool show = true) -> void override;

    virtual auto handle_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
        -> LRESULT override;
    auto on_create(HWND hWnd, WPARAM wParam, LPARAM lParam) -> int;
    auto on_close(HWND hWnd, WPARAM wParam, LPARAM lParam) -> int override;
    auto on_notify(HWND hWnd, WPARAM wParam, LPARAM lParam) -> int;
    auto on_size(HWND hWnd, WPARAM wParam, LPARAM lParam) -> int;

    static auto EnumChildProc(HWND hWnd, LPARAM lParam) -> BOOL;
    constexpr static int s_border{2};
    constexpr static int s_bar{65};
    int m_bar{};

    HWND m_app{nullptr};

    std::unique_ptr<Main> m_browser1;
    std::unique_ptr<Side> m_browser2;
    std::unique_ptr<URL> m_url;
};

} // namespace airglow
