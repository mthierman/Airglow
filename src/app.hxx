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
    struct URL
    {
        URL();

        std::pair<std::string, std::string> current;
        std::pair<std::string, std::string> home;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(URL, current, home)
    };

    using glow::window::MessageWindow<App>::MessageWindow;

    App();

    auto operator()() -> int;

    auto data() -> std::filesystem::path;
    auto json() -> std::filesystem::path;
    auto save() -> void;
    auto load() -> void;
    auto startup() -> void;

    auto wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    auto on_notify(WPARAM wParam, LPARAM lParam) -> int;

    std::filesystem::path m_settingsFile;

    glow::gui::GdiPlus m_gdiInit;
    glow::gui::CoInitialize m_coInit;

    std::set<size_t> m_windows;
    std::unique_ptr<Window> m_windowMain;
    std::unique_ptr<Settings> m_windowSettings;

    URL m_url;
};
