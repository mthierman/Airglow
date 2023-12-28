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
#include <gui/webview.hxx>

#include "WebView.hxx"

namespace airglow
{

using json = nlohmann::json;

auto run() -> void;

struct Settings
{
    std::string name{PROJECT_NAME};
    std::string version{PROJECT_VERSION};
    int width{};
    int height{};
};

inline void to_json(json& j, const Settings& settings)
{
    j = json{{"name", settings.name},
             {"version", settings.version},
             {"width", settings.width},
             {"height", settings.height}};
}

inline void from_json(const json& j, Settings& settings)
{
    j.at("name").get_to(settings.name);
    j.at("version").get_to(settings.version);
    j.at("width").get_to(settings.width);
    j.at("height").get_to(settings.height);
}

struct App final : public glow::gui::MainWindow
{
    using glow::gui::MainWindow::MainWindow;

    auto save() -> void;
    auto load() -> void;

    auto handle_message(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT override;
    static auto enum_child_proc(HWND hwnd, LPARAM lParam) -> BOOL;

    auto on_key_down(WPARAM wParam) -> int;
    auto on_notify() -> int;
    auto on_size() -> int;

    // WebView wv1{m_hwnd.get(), 1};
    // WebView wv2{m_hwnd.get(), 2};
    WebView wv3{m_hwnd.get(), 3};
    Settings m_settings;
};

} // namespace airglow
