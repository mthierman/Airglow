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
#include <airglow/settings.hxx>
#include <airglow/webview.hxx>

namespace airglow
{

struct App final : public glow::window::Window
{
    using glow::window::Window::Window;

    auto run() -> int;

    virtual auto create_window() -> void override;

    virtual auto handle_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
        -> LRESULT override;
    auto on_key_down(HWND hWnd, WPARAM wParam, LPARAM lParam) -> int;
    auto on_notify(HWND hWnd, WPARAM wParam, LPARAM lParam) -> int;

    glow::window::GdiPlus m_gdiInit;
    glow::window::CoInitialize m_coInit;

    std::set<HWND> m_windows;
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

    std::unique_ptr<airglow::webview::Main> m_browser1;
    std::unique_ptr<airglow::webview::Side> m_browser2;
    std::unique_ptr<airglow::webview::URL> m_url;
};

struct Settings final : public glow::window::Window
{
    using glow::window::Window::Window;

    Settings(HWND app, std::string className);

    virtual auto operator()(bool show = true) -> void override;

    virtual auto handle_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
        -> LRESULT override;
    auto on_create(HWND hWnd, WPARAM wParam, LPARAM lParam) -> int;
    auto on_close(HWND hWnd, WPARAM wParam, LPARAM lParam) -> int override;
    auto on_size(HWND hWnd, WPARAM wParam, LPARAM lParam) -> int;

    static auto EnumChildProc(HWND hWnd, LPARAM lParam) -> BOOL;

    HWND m_app{nullptr};

    std::unique_ptr<airglow::webview::Settings> m_browser;
};

} // namespace airglow
