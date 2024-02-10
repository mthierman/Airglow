// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#pragma once

#include <memory>
#include <utility>
#include <string>

#include <wil/resource.h>

#include <glow/glow.hxx>

#include "browser.hxx"
#include "global.hxx"
#include "state.hxx"

using json = nlohmann::json;

struct Window : public glow::Window<Window>
{
    struct Browsers
    {
        std::unique_ptr<Browser> first;
        std::unique_ptr<Browser> second;
        std::unique_ptr<Browser> url;
    };

    struct Positions
    {
        glow::Position full;
        glow::Position empty;
        glow::Position left;
        glow::Position right;
        glow::Position top;
        glow::Position bottom;
    };

    struct Layout
    {
        int bar{};
        int border{0};
        bool horizontal{};
        bool split{};
        bool swapped{};

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Layout, bar, border, horizontal, split, swapped)
    };

    using glow::Window<Window>::Window;

    Window(HWND parent, State& state, intptr_t id);

    static auto EnumChildProc(HWND hWnd, LPARAM lParam) -> BOOL;
    auto default_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    auto on_close(WPARAM wParam, LPARAM lParam) -> int;
    auto on_dpi_changed(WPARAM wParam, LPARAM lParam) -> int;
    auto on_get_min_max_info(WPARAM wParam, LPARAM lParam) -> int;
    auto on_key_down(WPARAM wParam, LPARAM lParam) -> int;
    auto on_notify(WPARAM wParam, LPARAM lParam) -> int;
    auto on_setting_change(WPARAM wParam, LPARAM lParam) -> int;
    auto on_size(WPARAM wParam, LPARAM lParam) -> int;
    auto on_sys_key_down(WPARAM wParam, LPARAM lParam) -> int;

    auto update_caption() -> void;

    HWND m_parent;
    State& m_state;

    Browsers m_browsers;
    // std::unique_ptr<Browser> m_first;
    // std::unique_ptr<Browser> m_second;
    // std::unique_ptr<Browser> m_url;
    Positions m_positions;
    Layout m_layout;
    std::pair<std::string, std::string> m_title{"Airglow", "Airglow"};
    std::pair<wil::unique_hicon, wil::unique_hicon> m_favicon;
    std::pair<std::string, std::string> m_faviconUrl;
    std::string m_focus;
    bool m_init{};

    Keys m_keys;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Window, m_faviconUrl, m_focus, m_layout)
};
