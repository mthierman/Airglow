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
    App(glow::system::Event& singleInstance);
    ~App();

    auto operator()() -> int;

    auto data() -> std::filesystem::path;
    auto file() -> std::filesystem::path;
    auto save() -> void;
    auto load() -> void;

    auto parse_args(int argc, char* argv[]) -> void;
    // auto new_window() -> void;

    State m_state;
    // std::unique_ptr<Settings> m_settings {};
    // std::map<size_t, std::unique_ptr<Window>> m_windows {};
    // size_t m_active {};

    glow::system::Event& m_singleInstance;
    ::ULONG_PTR& m_gdiToken;
    glow::window::WindowManager<Window> m_windows;
    std::unordered_map<char, bool> m_keys { { 'N', false }, { 'W', false } };
    glow::webview::WebViewEnvironment m_webViewEnvironment;
};
