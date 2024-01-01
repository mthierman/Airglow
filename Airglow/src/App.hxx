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

#include <config/airglow.hxx>
#include <filesystem/filesystem.hxx>
#include <gui/gui.hxx>
#include <gui/mainwindow.hxx>
#include <logging/logging.hxx>

#include "AddressBar.hxx"
#include "Browser.hxx"
#include "Enums.hxx"
#include "Settings.hxx"
#include "SettingsWindow.hxx"

using namespace glow::gui;

struct App final : public MainWindow
{
    using MainWindow::MainWindow;

    static auto run() -> int;
    static auto run_server() -> int;

    auto save() -> void;
    auto load() -> void;

    static auto EnumChildProc(HWND hWnd, LPARAM lParam) -> BOOL;

    auto handle_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT override;
    auto on_key_down(WPARAM wParam) -> int;
    auto on_parent_notify(WPARAM wParam) -> int;
    auto on_size() -> int;

    std::unique_ptr<Browser> m_browser1;
    std::unique_ptr<Browser> m_browser2;
    std::unique_ptr<AddressBar> m_addressBar1;
    std::unique_ptr<AddressBar> m_addressBar2;

    std::unique_ptr<Settings> m_settings;
    std::unique_ptr<SettingsWindow> m_settingsWindow;
};
