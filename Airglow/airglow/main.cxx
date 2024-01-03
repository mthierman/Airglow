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
auto run_server() -> int;

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

    airglow::Window window2{app.m_hwnd.get(), "window2"};
    window2();

    return glow::window::message_loop();
}

auto run_server() -> int
{
    std::println("Starting server...");

    STARTUPINFOA si{sizeof(STARTUPINFOA)};
    PROCESS_INFORMATION pi{};

    auto server{(glow::filesystem::portable() / "server.exe").string()};
    auto pServer{server.data()};

    std::println("Server path: {}", server);

    CreateProcessA(pServer, nullptr, nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi);
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}
