// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#pragma once

#include <fstream>
#include <set>

#include <nlohmann/json.hpp>

#include <glow/config.hxx>
#include <glow/console.hxx>
#include <glow/filesystem.hxx>
#include <glow/text.hxx>
#include <glow/window.hxx>

#include <airglow/config.hxx>
#include <airglow/definitions.hxx>
#include <airglow/webview.hxx>

namespace airglow
{

struct App;
struct Browser;
struct Settings;

auto data_path() -> std::filesystem::path;
auto json_path() -> std::filesystem::path;

struct App final : public glow::window::Window
{
    using glow::window::Window::Window;

    auto run() -> int;

    virtual auto create_window() -> void override;

    virtual auto handle_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
        -> LRESULT override;

    auto on_notify(HWND hWnd, WPARAM wParam, LPARAM lParam) -> int;

    glow::window::GdiPlus m_gdiInit;
    glow::window::CoInitialize m_coInit;

    std::unique_ptr<Browser> m_browser;
    std::unique_ptr<Settings> m_settings;

    std::set<HWND> m_windows;
};

struct Browser final : public glow::window::Window
{
    using glow::window::Window::Window;

    Browser(HWND app, std::string className);

    virtual auto operator()(bool show = true) -> void override;

    virtual auto handle_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
        -> LRESULT override;

    auto on_create(HWND hWnd, WPARAM wParam, LPARAM lParam) -> int;
    auto on_close(HWND hWnd, WPARAM wParam, LPARAM lParam) -> int override;
    auto on_key_down(HWND hWnd, WPARAM wParam, LPARAM lParam) -> int;
    auto on_notify(HWND hWnd, WPARAM wParam, LPARAM lParam) -> int;
    auto on_size(HWND hWnd, WPARAM wParam, LPARAM lParam) -> int;

    static auto EnumChildProc(HWND hWnd, LPARAM lParam) -> BOOL;

    auto url_path() -> std::string;

    auto save_settings() -> void;
    auto load_settings() -> void;

    constexpr static int s_border{2};
    int m_bar{100};

    HWND m_app{nullptr};

    std::unique_ptr<airglow::webview::Main> m_main;
    std::unique_ptr<airglow::webview::Side> m_side;
    std::unique_ptr<airglow::webview::URL> m_url;

    glow::window::Position m_position;
    glow::window::Colors m_colors;
    bool m_dark_mode{true};
    bool m_maximized{false};
    bool m_fullscreen{false};
    bool m_topmost{false};
    bool m_gui{true};
    bool m_split{true};
    bool m_swapped{false};
    bool m_horizontal{false};
    std::string m_mainHome{};
    std::string m_sideHome{};
    std::string m_mainCurrent{};
    std::string m_sideCurrent{};
};

void to_json(nlohmann::json& j, const Browser& app);
void from_json(const nlohmann::json& j, Browser& app);

struct Settings final : public glow::window::Window
{
    using glow::window::Window::Window;

    Settings(HWND app, std::string className);

    virtual auto operator()(bool show = true) -> void override;

    virtual auto handle_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
        -> LRESULT override;
    auto on_create(HWND hWnd, WPARAM wParam, LPARAM lParam) -> int;
    auto on_close(HWND hWnd, WPARAM wParam, LPARAM lParam) -> int override;
    auto on_size(HWND hWnd, WPARAM wParam, LPARAM lParam) -> int;

    static auto EnumChildProc(HWND hWnd, LPARAM lParam) -> BOOL;

    HWND m_app{nullptr};

    std::unique_ptr<airglow::webview::Settings> m_browser;
};

} // namespace airglow
