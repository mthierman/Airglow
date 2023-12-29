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
    set_system_backdrop(app.m_hwnd.get(), DWM_SYSTEMBACKDROP_TYPE::DWMSBT_TABBEDWINDOW);
    use_immersive_dark_mode(app.m_hwnd.get());

    auto color{make_colorref(256, 0, 0)};
    set_border_color(app.m_hwnd.get(), color);

    message_loop();
}
