// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#pragma once

#include <bit>

#include <config/airglow.hxx>
#include <gui/window.hxx>

#include "Browser.hxx"

using namespace glow::gui;

struct SettingsWindow final : public Window
{
    using Window::Window;

    SettingsWindow();

    auto handle_message(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT override;
    static auto enum_child_proc(HWND hWnd, LPARAM lParam) -> BOOL;

    auto on_key_down(WPARAM wParam) -> int;
    auto on_size() -> int;

    std::unique_ptr<Browser> m_browser1;
};
