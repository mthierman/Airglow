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
        glow::console::debug(e.what());
        std::terminate();
    }
}

auto run() -> int
{
    SetEnvironmentVariableA("WEBVIEW2_DEFAULT_BACKGROUND_COLOR", "0");
    SetEnvironmentVariableA("WEBVIEW2_ADDITIONAL_BROWSER_ARGUMENTS",
                            "--allow-file-access-from-files");

    airglow::App app;
    app(false);
    app.dwm_caption_color(false);
    app.dwm_dark_mode(true);
    app.dwm_system_backdrop(DWM_SYSTEMBACKDROP_TYPE::DWMSBT_MAINWINDOW);

    airglow::Window window1{app.m_hwnd.get(), "window1"};
    window1();

    airglow::Window window2{app.m_hwnd.get(), "window2"};
    window2();

    glow::console::debug(std::to_string(app.m_dpi));
    glow::console::debug(std::to_string(app.m_scale));

    // glow::console::debug(std::to_string(window1.m_dpi));
    // glow::console::debug(std::to_string(window2.m_scale));

    // MainWindow mainWindow{"Airglow"};
    // mainWindow();
    // mainWindow.dwm_caption_color(false);
    // mainWindow.dwm_dark_mode(true);
    // mainWindow.dwm_system_backdrop(DWM_SYSTEMBACKDROP_TYPE::DWMSBT_MAINWINDOW);
    // mainWindow.m_dpi = mainWindow.dpi();
    // mainWindow.m_scale = mainWindow.scale();

    return glow::window::message_loop();
}
