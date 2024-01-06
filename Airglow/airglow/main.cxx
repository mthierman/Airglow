// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#include <airglow/settings.hxx>
#include <airglow/window.hxx>

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

    airglow::MessageWindow messageWindow;
    messageWindow();

    airglow::Window mainWindow{messageWindow.m_hwnd.get(), "MainWindow"};
    mainWindow();

    airglow::settings::Window settingsWindow{messageWindow.m_hwnd.get(), "Settings"};
    // settingsWindow();

    // glow::console::create_process("server.exe");

    return glow::window::message_loop();
}
