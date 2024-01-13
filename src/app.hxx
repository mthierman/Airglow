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
#include "window.hxx"

struct App : public glow::window::MessageWindow<App>
{
    using glow::window::MessageWindow<App>::MessageWindow;

    static auto run() -> int;

    auto wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    auto on_notify(WPARAM wParam, LPARAM lParam) -> int;

    auto data_path() -> std::filesystem::path;
    auto json_path() -> std::filesystem::path;

    glow::gui::GdiPlus m_gdiInit;
    glow::gui::CoInitialize m_coInit;

    std::string m_mainUrl{"about:blank"};
    std::string m_sideUrl{"about:blank"};

    std::vector<std::unique_ptr<MainWindow>> m_windowVector;
    std::set<int64_t> m_windowSet;
};
