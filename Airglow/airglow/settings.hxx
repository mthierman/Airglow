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
    // STORE POSITION HERE?
    std::string name{config::name};
    std::string version{config::version};
    bool dark_mode{true};
    int64_t dpi{};
    float scale{};
    bool maximized{false};
    bool fullscreen{false};
    bool topmost{false};
    bool gui{true};
    bool split{true};
    bool swapped{false};
    bool horizontal{false};
    std::string mainHome{};
    std::string sideHome{};
    std::string mainCurrent{};
    std::string sideCurrent{};
};

// struct Colors
// {
//     std::string
//     accent{util::system_color(winrt::Windows::UI::ViewManagement::UIColorType::Accent)};
//     std::string accentDark1{
//         util::system_color(winrt::Windows::UI::ViewManagement::UIColorType::AccentDark1)};
//     std::string accentDark2{
//         util::system_color(winrt::Windows::UI::ViewManagement::UIColorType::AccentDark2)};
//     std::string accentDark3{
//         util::system_color(winrt::Windows::UI::ViewManagement::UIColorType::AccentDark3)};
//     std::string accentLight1{
//         util::system_color(winrt::Windows::UI::ViewManagement::UIColorType::AccentLight1)};
//     std::string accentLight2{
//         util::system_color(winrt::Windows::UI::ViewManagement::UIColorType::AccentLight2)};
//     std::string accentLight3{
//         util::system_color(winrt::Windows::UI::ViewManagement::UIColorType::AccentLight3)};
//     std::string background{
//         util::system_color(winrt::Windows::UI::ViewManagement::UIColorType::Background)};
//     std::string foreground{
//         util::system_color(winrt::Windows::UI::ViewManagement::UIColorType::Foreground)};
// };

// struct Paths
// {
//     std::filesystem::path data{util::path_portable()};
//     std::filesystem::path gui{util::path_gui()};
//     std::filesystem::path json{util::path_json()};
//     std::filesystem::path db{util::path_db()};
// };

void to_json(nlohmann::json& j, const Settings& settings);
void from_json(const nlohmann::json& j, Settings& settings);

auto save_settings(Settings settings) -> void;
auto load_settings(Settings settings) -> void;

// std::filesystem::path path_appdata();
// std::filesystem::path path_portable();
// std::filesystem::path path_gui();
// std::filesystem::path path_home();
// std::filesystem::path path_settings();
// std::filesystem::path path_bar();
// std::filesystem::path path_json();
// std::filesystem::path path_db();

// std::string home_url();
// std::string settings_url();
// std::string bar_url();

} // namespace airglow::settings
