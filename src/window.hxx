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

#include "global.hxx"
#include "browser.hxx"

using json = nlohmann::json;

struct Browsers
{
    std::unique_ptr<MainBrowser> first;
    std::unique_ptr<SideBrowser> second;
    std::unique_ptr<URLBrowser> url;
};

struct Window : public glow::Window<Window>
{
    using glow::Window<Window>::Window;

    Window(HWND app, URL& url, glow::Colors& colors, uintptr_t id);

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
    glow::Colors& m_colors;

    Browsers m_browsers;
    Positions m_positions;
    Layout m_layout;
    // glow::Colors m_colors;

    std::pair<std::string, std::string> m_title{"Airglow", "Airglow"};
    std::pair<wil::unique_hicon, wil::unique_hicon> m_favicon;
    std::pair<std::string, std::string> m_faviconUrl;
    std::string m_focus;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Window, m_colors, m_faviconUrl, m_focus, m_layout, m_url)
};
