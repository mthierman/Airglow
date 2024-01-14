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

struct WindowBrowsers
{
    std::unique_ptr<MainBrowser> first;
    std::unique_ptr<SideBrowser> second;
    std::unique_ptr<URLBrowser> url;
};

struct Window : public glow::window::BaseWindow<Window>
{
    using glow::window::BaseWindow<Window>::BaseWindow;

    Window(HWND app, std::pair<std::string, std::string> urls);

    auto default_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    auto on_close(WPARAM wParam, LPARAM lParam) -> int;
    auto on_key_down(WPARAM wParam, LPARAM lParam) -> int;
    auto on_notify(WPARAM wParam, LPARAM lParam) -> int;
    auto on_size(WPARAM wParam, LPARAM lParam) -> int;

    static auto EnumChildProc(HWND hWnd, LPARAM lParam) -> BOOL;

    auto url() -> std::string;

    HWND m_app{nullptr};
    std::pair<std::string, std::string> m_urls;

    WindowBrowsers m_browsers;

    constexpr static int s_border{2};
    int m_bar{0};
};
