// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#pragma once

#include <set>

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

} // namespace airglow
