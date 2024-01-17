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

struct Window : public glow::window::BaseWindow<Window>
{
    struct Browsers
    {
        std::unique_ptr<MainBrowser> first;
        std::unique_ptr<SideBrowser> second;
        std::unique_ptr<URLBrowser> url;
    };

    struct Positions
    {
        glow::gui::Position full;
        glow::gui::Position empty;
        glow::gui::Position left;
        glow::gui::Position right;
        glow::gui::Position top;
        glow::gui::Position bottom;
    };

    using glow::window::BaseWindow<Window>::BaseWindow;

    Window(HWND app, std::pair<std::string, std::string> urls);

    static auto EnumChildProc(HWND hWnd, LPARAM lParam) -> BOOL;
    auto default_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    auto on_close(WPARAM wParam, LPARAM lParam) -> int;
    auto on_key_down(WPARAM wParam, LPARAM lParam) -> int;
    auto on_notify(WPARAM wParam, LPARAM lParam) -> int;
    auto on_size(WPARAM wParam, LPARAM lParam) -> int;

    HWND m_app{nullptr};
    std::pair<std::string, std::string> m_urls;
    Browsers m_browsers;
    Positions m_positions;
    bool m_split{false};
    bool m_swapped{false};
    bool m_horizontal{false};
    constexpr static int s_border{2};
    int m_bar{0};
};
