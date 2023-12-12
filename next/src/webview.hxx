// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#pragma once

#include <config/airglow.hxx>

#include <gui/gui.hxx>
#include <gui/webview.hxx>

//==============================================================================
namespace airglow
{

//==============================================================================
struct WebView final : public glow::gui::WebView
{
    using glow::gui::WebView::WebView;

  private:
    virtual auto web_message_received_handler() -> void override;
    virtual auto accelerator_key_pressed_handler(ICoreWebView2AcceleratorKeyPressedEventArgs* args) -> void override;
};

//==============================================================================
} // namespace airglow
