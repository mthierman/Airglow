// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#pragma once

#include <glow/window.hxx>

namespace airglow
{

struct AddressBar final : public glow::window::WebView
{
    using glow::window::WebView::WebView;

    auto initialized() -> void override;

    auto web_message_received_handler() -> void override;
    auto accelerator_key_pressed_handler(ICoreWebView2AcceleratorKeyPressedEventArgs* args)
        -> void override;
};

} // namespace airglow
