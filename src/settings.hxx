// clang-format off
// Airglow - https://github.com/mthierman/Airglow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#pragma once

#include <memory>

#include <glow/window.hxx>
#include <glow/webview.hxx>
#include <glow/input.hxx>

#include <nlohmann/json.hpp>

#include "global.hxx"

namespace airglow {
using json = nlohmann::json;

struct Settings final : glow::window::Window {
    Settings(glow::webview::WebViewEnvironment& webViewEnvironment);

    glow::webview::WebViewEnvironment& m_webViewEnvironment;
    glow::webview::WebView m_webView;
};
}; // namespace airglow
