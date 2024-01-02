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

#include <glow/filesystem.hxx>
#include <glow/gui.hxx>
#include <glow/mainwindow.hxx>
#include <glow/log.hxx>

#include <airglow/addressbar.hxx>
#include <airglow/browser.hxx>
#include <airglow/frame.hxx>
#include <airglow/settings.hxx>
#include <airglow/settingswindow.hxx>
#include <airglow/webviews.hxx>

using namespace glow::gui;

struct App final : public MainWindow
{
    using MainWindow::MainWindow;

    static auto run() -> int;
    static auto run_server() -> int;

    static auto EnumChildProc(HWND hWnd, LPARAM lParam) -> BOOL;

    auto handle_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT override;
    auto on_activate(WPARAM wParam) -> int;
    auto on_key_down(WPARAM wParam) -> int;
    auto on_parent_notify(WPARAM wParam) -> int;
    auto on_move() -> int;
    auto on_size() -> int;

    auto position_frame() -> void;

    glow::gui::GdiPlus gdiInit;
    glow::gui::CoInitialize coInit;

    UINT dpi{};
    float scale{};

    std::unique_ptr<Browser> m_browser1;
    std::unique_ptr<Browser> m_browser2;
    // std::unique_ptr<Frame> m_frame1;
    // std::unique_ptr<Frame> m_frame2;
    std::unique_ptr<AddressBar> m_addressBar1;
    std::unique_ptr<AddressBar> m_addressBar2;

    std::unique_ptr<SettingsWindow> m_settingsWindow;

    std::unique_ptr<Settings> m_settings;
};
