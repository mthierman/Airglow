// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#pragma once

#include <set>

#include <glow/glow.hxx>

#include "global.hxx"
#include "settings.hxx"
#include "window.hxx"

struct App : public glow::window::MessageWindow<App>
{
    using glow::window::MessageWindow<App>::MessageWindow;

    auto operator()() -> int;

    auto wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    auto on_notify(WPARAM wParam, LPARAM lParam) -> int;

    auto data_path() -> std::filesystem::path;
    auto json_path() -> std::filesystem::path;

    // auto save_settings() -> void;
    // auto load_settings() -> void;

    glow::gui::GdiPlus m_gdiInit;
    glow::gui::CoInitialize m_coInit;

    std::string m_mainUrl{"about:blank"};
    std::string m_sideUrl{"about:blank"};

    std::unique_ptr<Window> m_mainWindow;
    std::unique_ptr<Settings> m_settingsWindow;
    std::vector<std::unique_ptr<Window>> m_windowVector;
    std::set<int64_t> m_windowSet;
};
