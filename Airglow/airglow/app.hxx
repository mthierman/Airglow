// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#pragma once

#include <bit>
#include <memory>

#include <nlohmann/json.hpp>

#include <glow/console.hxx>
#include <glow/filesystem.hxx>
#include <glow/window.hxx>

#include <airglow/webviews.hxx>

using namespace glow;

struct App final : public window::MainWindow
{
    using window::MainWindow::MainWindow;

    static auto EnumChildProc(HWND hWnd, LPARAM lParam) -> BOOL;

    virtual auto handle_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
        -> LRESULT override;
    auto on_size(HWND hWnd) -> int;

    window::GdiPlus m_gdiInit;
    window::CoInitialize m_coInit;
};
