// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#pragma once

#include <bit>

#include <nlohmann/json.hpp>

#include <config/airglow.hxx>
#include <filesystem/filesystem.hxx>
#include <gui/gui.hxx>
#include <gui/mainwindow.hxx>

#include "Browser.hxx"
#include "Settings.hxx"
#include "SettingsWindow.hxx"

using namespace glow::gui;

struct App final : public MainWindow
{
    using MainWindow::MainWindow;

    static auto run() -> int;
    auto save() -> void;
    auto load() -> void;

    auto handle_message(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT override;
    static auto enum_child_proc(HWND hWnd, LPARAM lParam) -> BOOL;

    auto on_key_down(WPARAM wParam) -> int;
    auto on_size() -> int;

    Browser m_browser1{m_hwnd.get(), 1, "https://www.google.com/"};
    Browser m_browser2{m_hwnd.get(), 2, "https://www.google.com/"};
    // Browser m_browser3{m_hwnd.get(), 3, "https://localhost:8000/"};
    // Browser m_browser4{m_hwnd.get(), 4, "https://localhost:8000/"};

    Settings m_settings;
};
