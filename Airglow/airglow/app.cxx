// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#include <airglow/app.hxx>

auto run() -> int
{
    SetEnvironmentVariableA("WEBVIEW2_DEFAULT_BACKGROUND_COLOR", "0");
    SetEnvironmentVariableA("WEBVIEW2_ADDITIONAL_BROWSER_ARGUMENTS",
                            "--allow-file-access-from-files");

    MainWindow mainWindow{"Airglow"};
    mainWindow();
    mainWindow.dwm_caption_color(false);
    mainWindow.dwm_dark_mode(true);
    mainWindow.dwm_system_backdrop(DWM_SYSTEMBACKDROP_TYPE::DWMSBT_MAINWINDOW);
    mainWindow.m_dpi = mainWindow.dpi();
    mainWindow.m_scale = mainWindow.scale();

    mainWindow.m_browser1 = std::make_unique<Browser>(+WebViews::browser1, mainWindow.m_hwnd.get());
    mainWindow.m_browser2 = std::make_unique<Browser>(+WebViews::browser2, mainWindow.m_hwnd.get());
    (*mainWindow.m_browser1)();
    (*mainWindow.m_browser2)();

    // mainWindow.m_frame1 = std::make_unique<Frame>();
    // mainWindow.m_frame2 = std::make_unique<Frame>();

    mainWindow.m_addressBar1 = std::make_unique<AddressBar>(
        +WebViews::bar1, mainWindow.m_hwnd.get(), "https://localhost:8000/addressbar/index.html");
    mainWindow.m_addressBar2 = std::make_unique<AddressBar>(
        +WebViews::bar2, mainWindow.m_hwnd.get(), "https://localhost:8000/addressbar/index.html");
    (*mainWindow.m_addressBar1)();
    (*mainWindow.m_addressBar2)();

    mainWindow.m_settingsWindow = std::make_unique<SettingsWindow>("Settings");
    (*mainWindow.m_settingsWindow)();

    mainWindow.m_settingsWindow->dwm_caption_color(false);
    mainWindow.m_settingsWindow->dwm_dark_mode(true);
    mainWindow.m_settingsWindow->dwm_system_backdrop(DWM_SYSTEMBACKDROP_TYPE::DWMSBT_MAINWINDOW);

    mainWindow.m_settingsWindow->m_browser = std::make_unique<SettingsWebView>(
        +WebViews::settings, mainWindow.m_settingsWindow->m_hwnd.get());

    return window::message_loop();
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
