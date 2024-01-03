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

#include <glow/config.hxx>
#include <glow/console.hxx>
#include <glow/filesystem.hxx>
#include <glow/text.hxx>
#include <glow/window.hxx>

#include <airglow/definitions.hxx>

namespace airglow
{

struct App final : public glow::window::Window
{
    using glow::window::Window::Window;

    virtual auto create_window() -> void override;

    static auto EnumChildProc(HWND hWnd, LPARAM lParam) -> BOOL;

    virtual auto handle_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
        -> LRESULT override;
    auto on_notify(LPARAM lParam) -> int;
    auto on_size(HWND hWnd) -> int;

    glow::window::GdiPlus m_gdiInit;
    glow::window::CoInitialize m_coInit;

    std::set<HWND> m_windows;
};

struct Window final : public glow::window::Window
{
    using glow::window::Window::Window;

    Window(HWND app, std::string name);

    virtual auto operator()(bool show = true) -> void override;

    virtual auto handle_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
        -> LRESULT override;
    auto on_create() -> int;
    auto on_close() -> int;

    HWND m_app{nullptr};
    int64_t m_id{glow::text::random_int()};
};

} // namespace airglow
