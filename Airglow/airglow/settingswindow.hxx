// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#pragma once

#include <bit>

#include <glow/window.hxx>

#include <airglow/webviews.hxx>

using namespace glow;

struct SettingsWebView final : public window::WebView
{
    using WebView::WebView;

  private:
    auto initialized() -> void override;
};

struct SettingsWindow final : public window::Window
{
    using Window::Window;

    SettingsWindow();

    static auto EnumChildProc(HWND hWnd, LPARAM lParam) -> BOOL;

    auto handle_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT override;
    auto on_key_down(WPARAM wParam) -> int;
    auto on_size() -> int;

    std::unique_ptr<SettingsWebView> m_browser;
};
