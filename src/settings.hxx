// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#pragma once

#include <memory>

#include <glow/glow.hxx>

#include "browser.hxx"
#include "global.hxx"
#include "state.hxx"

using json = nlohmann::json;

struct Settings : public glow::Window
{
    Settings(::HWND app, State& state);

    auto WndProc(::UINT uMsg, ::WPARAM wParam, ::LPARAM lParam) -> ::LRESULT override;
    auto on_create(::WPARAM wParam, ::LPARAM lParam) -> int override;
    auto on_close(::WPARAM wParam, ::LPARAM lParam) -> int override;
    auto on_get_min_max_info(::WPARAM wParam, ::LPARAM lParam) -> int;
    auto on_key_down(::WPARAM wParam, ::LPARAM lParam) -> int;
    auto on_notify(::WPARAM wParam, ::LPARAM lParam) -> int;
    auto on_setting_change(::WPARAM wParam, ::LPARAM lParam) -> int;
    auto on_show_window(::WPARAM wParam, ::LPARAM lParam) -> int;
    auto on_size(::WPARAM wParam, ::LPARAM lParam) -> int;

    ::HWND m_app{nullptr};
    State& m_state;
    std::unique_ptr<Browser> m_browser{};
    Keys m_keys{};

  private:
    static auto EnumChildProc(::HWND hWnd, ::LPARAM lParam) -> ::BOOL;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Settings, m_state)
};
