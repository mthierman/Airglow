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
struct Colors;

struct App final : public glow::window::Window
{
    using glow::window::Window::Window;

    auto run() -> int;

    virtual auto create_window() -> void override;

    virtual auto handle_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
        -> LRESULT override;
    auto on_key_down(HWND hWnd, WPARAM wParam, LPARAM lParam) -> int;
    auto on_notify(HWND hWnd, WPARAM wParam, LPARAM lParam) -> int;

    auto data_path() -> std::filesystem::path;
    auto json_path() -> std::filesystem::path;
    auto gui_path() -> std::filesystem::path;

    auto save_settings() -> void;
    auto load_settings() -> void;

    glow::window::GdiPlus m_gdiInit;
    glow::window::CoInitialize m_coInit;

    std::unique_ptr<Browser> m_browser;
    std::unique_ptr<Settings> m_settings;

    std::set<HWND> m_windows;
    std::filesystem::path m_dataPath{data_path()};
    std::filesystem::path m_jsonPath{json_path()};
    std::filesystem::path m_guiPath{gui_path()};

    glow::window::Colors m_colors;

    std::string m_name{"Airglow"};
    std::string m_version{AIRGLOW_VERSION};
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

void to_json(nlohmann::json& j, const App& app);
void from_json(const nlohmann::json& j, App& app);

struct Browser final : public glow::window::Window
{
    using glow::window::Window::Window;

    Browser(HWND app, std::string className);

    virtual auto operator()(bool show = true) -> void override;

    virtual auto handle_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
        -> LRESULT override;
    auto on_create(HWND hWnd, WPARAM wParam, LPARAM lParam) -> int;
    auto on_close(HWND hWnd, WPARAM wParam, LPARAM lParam) -> int override;
    auto on_notify(HWND hWnd, WPARAM wParam, LPARAM lParam) -> int;
    auto on_size(HWND hWnd, WPARAM wParam, LPARAM lParam) -> int;

    static auto EnumChildProc(HWND hWnd, LPARAM lParam) -> BOOL;
    constexpr static int s_border{2};
    constexpr static int s_bar{65};
    int m_bar{};

    HWND m_app{nullptr};

    std::unique_ptr<airglow::webview::Main> m_browser1;
    std::unique_ptr<airglow::webview::Side> m_browser2;
    std::unique_ptr<airglow::webview::URL> m_url;
};

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
