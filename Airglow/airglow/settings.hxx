// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#pragma once

#include <fstream>
#include <print>

#include <nlohmann/json.hpp>

#include <glow/filesystem.hxx>
#include <glow/window.hxx>

#include <airglow/browser.hxx>
#include <airglow/config.hxx>
#include <airglow/definitions.hxx>

namespace airglow::settings
{

struct Window final : public glow::window::Window
{
    using glow::window::Window::Window;

    Window(HWND app, std::string className);

    virtual auto operator()(bool show = true) -> void override;

    virtual auto handle_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
        -> LRESULT override;
    auto on_create(HWND hWnd, WPARAM wParam, LPARAM lParam) -> int;
    auto on_close(HWND hWnd, WPARAM wParam, LPARAM lParam) -> int override;
    auto on_size(HWND hWnd, WPARAM wParam, LPARAM lParam) -> int;

    static auto EnumChildProc(HWND hWnd, LPARAM lParam) -> BOOL;

    HWND m_app{nullptr};
    const int64_t m_id{glow::text::random_int64()};

    std::unique_ptr<airglow::Browser> m_browser;
};

struct Settings
{
    std::string m_name{config::name};
    std::string m_version{config::version};
    int m_width{};
    int m_height{};
};

void to_json(nlohmann::json& j, const Settings& settings);
void from_json(const nlohmann::json& j, Settings& settings);

auto save_settings(Settings settings) -> void;
auto load_settings(Settings settings) -> void;

} // namespace airglow::settings
