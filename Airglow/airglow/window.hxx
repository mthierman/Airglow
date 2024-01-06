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
#include <airglow/webview.hxx>
#include <airglow/definitions.hxx>

namespace airglow
{

struct Window final : public glow::window::Window
{
    using glow::window::Window::Window;

    Window(HWND app, std::string className);

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

    std::unique_ptr<MainBrowser> m_browser1;
    std::unique_ptr<SideBrowser> m_browser2;
    std::unique_ptr<URL> m_url;
};

} // namespace airglow
