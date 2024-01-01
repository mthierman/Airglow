// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#pragma once

#include <bit>

#include <config/airglow.hxx>
#include <gui/window.hxx>
#include <gui/webview.hxx>

using namespace glow::gui;

struct SettingsWebView final : public WebView2
{
    using WebView2::WebView2;

  private:
    auto initialized() -> void override;
};

struct SettingsWindow final : public Window
{
    using Window::Window;

    SettingsWindow();

    static auto EnumChildProc(HWND hWnd, LPARAM lParam) -> BOOL;

    auto handle_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT override;
    auto on_key_down(WPARAM wParam) -> int;
    auto on_size() -> int;

    std::unique_ptr<SettingsWebView> m_browser;
};
