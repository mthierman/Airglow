// clang-format off
// Airglow - https://github.com/mthierman/Airglow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#pragma once

#include <memory>

#include <glow/window.hxx>
#include <glow/webview.hxx>

#include <nlohmann/json.hpp>

// #include "browser.hxx"
// #include "global.hxx"
// #include "state.hxx"

namespace airglow {
using json = nlohmann::json;

struct Settings final : glow::window::Window {
    Settings(glow::webview::WebViewEnvironment& webViewEnvironment);

    // std::unique_ptr<Browser> m_browser {};
    // Keys m_keys {};

    glow::webview::WebViewEnvironment& m_webViewEnvironment;
    glow::webview::WebView m_webView;

private:
    // static auto EnumChildProc(::HWND hWnd, ::LPARAM lParam) -> ::BOOL;

    // NLOHMANN_DEFINE_TYPE_INTRUSIVE(Settings, m_state)
};
}; // namespace airglow
