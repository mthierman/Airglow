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

struct App : public glow::App<App>
{
    using glow::App<App>::App;

    App(int argc, char* argv[]);

    auto operator()() -> int;

    auto data() -> std::filesystem::path;
    auto json() -> std::filesystem::path;
    auto save() -> void;
    auto load() -> void;

    auto window() -> void;

    auto wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    auto on_notify(WPARAM wParam, LPARAM lParam) -> int;

    std::filesystem::path m_settingsFile;

    glow::GdiPlus m_gdiInit;
    glow::CoInitialize m_coInit;
    std::vector<std::string> m_argv;
    URL m_url;
    std::unique_ptr<Settings> m_windowSettings;
    std::unordered_map<uintptr_t, std::unique_ptr<Window>> m_windows;
};
