// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#pragma once

#include <memory>
#include <string>

#include <wil/resource.h>

#include <glow/glow.hxx>

#include "browser.hxx"
#include "global.hxx"
#include "state.hxx"

using json = nlohmann::json;

struct Window final : public glow::Window
{
    struct Page
    {
        std::unique_ptr<Browser> browser{};
        wil::unique_hicon hicon{};
        std::string favicon{};
        std::string source{};
        std::string title{"Airglow"};

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Page, favicon, source, title)
    };

    struct Positions
    {
        glow::Position full{};
        glow::Position empty{};
        glow::Position left{};
        glow::Position right{};
        glow::Position top{};
        glow::Position bottom{};
    };

    struct Layout
    {
        int bar{0};
        int border{0};
        std::string focus{"first"};
        bool split{false};
        bool swap{false};
        bool vertical{true};

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Layout, bar, border, focus, split, swap, vertical)
    };

    Window(::HWND app, State& state, size_t id);

    auto WndProc(::UINT uMsg, ::WPARAM wParam, ::LPARAM lParam) -> ::LRESULT override;
    auto on_create(::WPARAM wParam, ::LPARAM lParam) -> int override;
    auto on_activate(::WPARAM wParam, ::LPARAM lParam) -> int;
    auto on_destroy(::WPARAM wParam, ::LPARAM lParam) -> int;
    auto on_get_min_max_info(::WPARAM wParam, ::LPARAM lParam) -> int;
    auto on_key_down(::WPARAM wParam, ::LPARAM lParam) -> int;
    auto on_notify(::WPARAM wParam, ::LPARAM lParam) -> int;
    auto on_setting_change(::WPARAM wParam, ::LPARAM lParam) -> int;
    auto on_size(::WPARAM wParam, ::LPARAM lParam) -> int;
    auto on_sys_key_down(::WPARAM wParam, ::LPARAM lParam) -> int;

    auto update_caption() -> void;

    ::HWND m_app{nullptr};
    State& m_state;
    Page m_first{};
    Page m_second{};
    Page m_url{};
    Positions m_positions{};
    Layout m_layout{};
    bool m_init{};
    Keys m_keys{};

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Window, m_window, m_dpi, m_scale, m_maximize, m_fullscreen,
                                   m_topmost, m_state, m_first, m_second, m_url, m_layout)

  private:
    static auto EnumChildProc(::HWND hWnd, ::LPARAM lParam) -> ::BOOL;
};
