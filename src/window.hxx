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
    auto on_close(HWND hWnd, WPARAM wParam, LPARAM lParam) -> int;
    auto on_key_down(HWND hWnd, WPARAM wParam, LPARAM lParam) -> int;
    auto on_notify(HWND hWnd, WPARAM wParam, LPARAM lParam) -> int;
    auto on_size(HWND hWnd, WPARAM wParam, LPARAM lParam) -> int;

    static auto EnumChildProc(HWND hWnd, LPARAM lParam) -> BOOL;

    HWND m_app{nullptr};
    std::pair<std::string, std::string> m_urls;

    WindowBrowsers m_browsers;

    constexpr static int s_border{2};
    int m_bar{0};
};