// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#include <airglow/app.hxx>
#include <airglow/definitions.hxx>
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

    airglow::App app;
    app();

    airglow::Window window1{app.m_hwnd.get(), "window1"};
    window1();
    // window1.m_browser1();

    // airglow::Window window2{app.m_hwnd.get(), "window2"};
    // window2();

    return glow::window::message_loop();
}
