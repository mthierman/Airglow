// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#pragma once

#include <Windows.h>

#include <filesystem>
#include <fstream>
#include <map>
#include <memory>

#include <glow/glow.hxx>

#include "global.hxx"
#include "settings.hxx"
#include "state.hxx"
#include "window.hxx"

using json = nlohmann::json;

struct App : public glow::App<App>
{
    App();

    auto wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    auto on_notify(WPARAM wParam, LPARAM lParam) -> int;

    auto parse_args() -> void;
    auto new_window() -> void;
    auto data() -> std::filesystem::path;
    auto file() -> std::filesystem::path;
    auto save() -> void;
    auto load() -> void;

    glow::GdiPlus m_gdiInit;
    glow::CoInitialize m_coInit;

    State m_state;

    std::unique_ptr<Settings> m_settings;
    std::map<intptr_t, std::unique_ptr<Window>> m_windows;
    intptr_t m_active{};
};
