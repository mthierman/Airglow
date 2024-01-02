// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#include <airglow/app.hxx>

auto run() -> int;

auto main() -> int
{
    try
    {
        return run();
    }
    catch (std::exception& e)
    {
        console::debug(e.what());
        std::terminate();
    }
}

auto run() -> int
{
    SetEnvironmentVariableA("WEBVIEW2_DEFAULT_BACKGROUND_COLOR", "0");
    SetEnvironmentVariableA("WEBVIEW2_ADDITIONAL_BROWSER_ARGUMENTS",
                            "--allow-file-access-from-files");

    // MainWindow mainWindow{"Airglow"};
    // mainWindow();
    // mainWindow.dwm_caption_color(false);
    // mainWindow.dwm_dark_mode(true);
    // mainWindow.dwm_system_backdrop(DWM_SYSTEMBACKDROP_TYPE::DWMSBT_MAINWINDOW);
    // mainWindow.m_dpi = mainWindow.dpi();
    // mainWindow.m_scale = mainWindow.scale();

    return window::message_loop();
}
