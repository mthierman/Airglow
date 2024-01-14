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

    using glow::window::BaseWindow<Window>::BaseWindow;

    Window(HWND app, std::pair<std::string, std::string> urls);

    auto default_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    auto on_close(WPARAM wParam, LPARAM lParam) -> int;
    auto on_key_down(WPARAM wParam, LPARAM lParam) -> int;
    auto on_notify(WPARAM wParam, LPARAM lParam) -> int;
    auto on_size(WPARAM wParam, LPARAM lParam) -> int;

    static auto EnumChildProc(HWND hWnd, LPARAM lParam) -> BOOL;

    auto url() -> std::string;

    constexpr static int s_border{2};
    int m_bar{0};

    HWND m_app{nullptr};
    Browsers m_browsers;

    std::pair<std::string, std::string> m_urls;
};
