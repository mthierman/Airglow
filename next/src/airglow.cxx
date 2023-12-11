// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#include "app.hxx"

#include <config/airglow.hxx>
#include <gui/gui.hxx>
#include <gui/webview.hxx>

//==============================================================================
auto WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow)
    -> int
{
    airglow::App app(PROJECT_NAME);

    glow::gui::WebView wv("WebView", app.get_hwnd(), 1);

    glow::gui::msg_loop();
}
