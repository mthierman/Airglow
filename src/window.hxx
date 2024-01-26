// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#pragma once

#include <utility>

#include <glow/glow.hxx>

#include "global.hxx"
#include "browser.hxx"

struct Browsers
{
    std::unique_ptr<MainBrowser> first;
    std::unique_ptr<SideBrowser> second;
    std::unique_ptr<URLBrowser> url;
};

struct Window : public glow::window::BaseWindow<Window>
{
    using glow::window::BaseWindow<Window>::BaseWindow;

    Window(HWND app, URL& url);

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

    HWND m_app;
    URL& m_url;

    std::string m_firstTitle{"Airglow"};
    std::string m_secondTitle{"Airglow"};

    Browsers m_browsers;
    Positions m_positions;
    Layout m_layout;
    glow::gui::SystemColors m_systemColors;
    bool m_initialized;
};
