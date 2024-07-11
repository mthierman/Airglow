// clang-format off
// Airglow - https://github.com/mthierman/Airglow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#pragma once

// #include <Windows.h>

// #include <map>
// #include <memory>

#include <glow/filesystem.hxx>
#include <glow/input.hxx>
#include <glow/messages.hxx>
#include <glow/webview.hxx>
#include <glow/window.hxx>

#include <nlohmann/json.hpp>

// #include "settings.hxx"
#include "state.hxx"
// #include "window.hxx"

using json = nlohmann::json;

struct App final : glow::window::Window {
    App(std::vector<std::string>& args, glow::system::Event& singleInstance);
    ~App();

    auto operator()() -> int;

    auto save_settings() -> void;
    auto load_settings() -> void;

    // auto new_window() -> void;

    State m_state;
    // std::unique_ptr<Settings> m_settings {};
    // std::map<size_t, std::unique_ptr<Window>> m_windows {};
    // size_t m_active {};

    std::vector<std::string>& m_args;
    glow::system::Event& m_singleInstance;
    ::ULONG_PTR& m_gdiToken;
    std::filesystem::path m_appData { glow::filesystem::known_folder(FOLDERID_LocalAppData,
                                                                     { "Airglow" }) };
    std::filesystem::path m_settingsFile { glow::filesystem::known_folder(
        FOLDERID_LocalAppData, { "Airglow", "Airglow.json" }) };
    glow::window::WindowManager<Window> m_windows;
    std::unordered_map<char, bool> m_keys { { 'N', false }, { 'W', false } };
    glow::webview::WebViewEnvironment m_webViewEnvironment;
};
