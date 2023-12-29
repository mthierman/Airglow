// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#include "Airglow.hxx"

auto Airglow::run() -> void
{
    App app;

    set_title(app.m_hwnd.get(), "Airglow");
    enable_caption_color(app.m_hwnd.get(), false);
    set_system_backdrop(app.m_hwnd.get(), DWM_SYSTEMBACKDROP_TYPE::DWMSBT_MAINWINDOW);
    use_immersive_dark_mode(app.m_hwnd.get());

    message_loop();
}
