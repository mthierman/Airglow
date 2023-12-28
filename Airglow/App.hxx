// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#pragma once

#include <bit>
#include <fstream>
#include <print>

#include <nlohmann/json.hpp>

#include <config/airglow.hxx>
#include <filesystem/filesystem.hxx>
#include <gui/gui.hxx>
#include <gui/mainwindow.hxx>

#include "Browser.hxx"

namespace Airglow
{
using namespace glow::gui;
using json = nlohmann::json;

struct Settings
{
    std::string m_name{PROJECT_NAME};
    std::string m_version{PROJECT_VERSION};
    int m_width{};
    int m_height{};
};

inline void to_json(json& j, const Settings& settings)
{
    j = json{{"name", settings.m_name},
             {"version", settings.m_version},
             {"width", settings.m_width},
             {"height", settings.m_height}};
}

inline void from_json(const json& j, Settings& settings)
{
    j.at("name").get_to(settings.m_name);
    j.at("version").get_to(settings.m_version);
    j.at("width").get_to(settings.m_width);
    j.at("height").get_to(settings.m_height);
}

struct App final : public glow::gui::MainWindow
{
    using MainWindow::MainWindow;

    static auto run() -> void;
    auto save() -> void;
    auto load() -> void;

    auto handle_message(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT override;
    static auto enum_child_proc(HWND hWnd, LPARAM lParam) -> BOOL;

    auto on_key_down(WPARAM wParam) -> int;
    auto on_size() -> int;

    Browser m_browser1{m_hwnd.get(), 1};
    Browser m_browser2{m_hwnd.get(), 2};
    Browser m_browser3{m_hwnd.get(), 3};
    Settings m_settings;
};

} // namespace Airglow
