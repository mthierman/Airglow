// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#pragma once

#include <map>

#include <glow/glow.hxx>

#include "global.hxx"
#include "browser.hxx"

struct Settings : public glow::window::BaseWindow<Settings>
{
    using glow::window::BaseWindow<Settings>::BaseWindow;

    Settings(HWND app, URL& url);

    static auto EnumChildProc(HWND hWnd, LPARAM lParam) -> BOOL;
    auto default_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    auto on_close(WPARAM wParam, LPARAM lParam) -> int;
    auto on_dpi_changed(WPARAM wParam, LPARAM lParam) -> int;
    auto on_get_min_max_info(WPARAM wParam, LPARAM lParam) -> int;
    auto on_key_down(WPARAM wParam, LPARAM lParam) -> int;
    auto on_notify(WPARAM wParam, LPARAM lParam) -> int;
    auto on_setting_change(WPARAM wParam, LPARAM lParam) -> int;
    auto on_show_window(WPARAM wParam, LPARAM lParam) -> int;
    auto on_size(WPARAM wParam, LPARAM lParam) -> int;

    HWND m_app;
    URL& m_url;

    std::unique_ptr<SettingsBrowser> m_browser;
    Dimensions m_dimensions;
    glow::SystemColors m_systemColors;
};
