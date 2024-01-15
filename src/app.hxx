// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#pragma once

#include <fstream>
#include <set>
#include <utility>

#include <glow/glow.hxx>

#include "global.hxx"
#include "settings.hxx"
#include "window.hxx"

struct App : public glow::window::MessageWindow<App>
{
    using glow::window::MessageWindow<App>::MessageWindow;

    auto operator()() -> int;

    auto env() -> void;
    auto args() -> std::pair<std::string, std::string>;

    auto wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    auto on_notify(WPARAM wParam, LPARAM lParam) -> int;

    auto data() -> std::filesystem::path;
    auto json() -> std::filesystem::path;
    auto save() -> void;
    auto load() -> void;

    glow::gui::GdiPlus m_gdiInit;
    glow::gui::CoInitialize m_coInit;

    std::filesystem::path m_settings{json()};
    std::pair<std::string, std::string> m_urls{args()};

    std::set<size_t> m_windows;
    std::unique_ptr<Window> m_windowMain;
    std::unique_ptr<Settings> m_windowSettings;
};
