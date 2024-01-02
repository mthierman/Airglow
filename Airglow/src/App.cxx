// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#include "App.hxx"

auto App::run() -> int
{
    SetEnvironmentVariableA("WEBVIEW2_DEFAULT_BACKGROUND_COLOR", "0");
    SetEnvironmentVariableA("WEBVIEW2_ADDITIONAL_BROWSER_ARGUMENTS",
                            "--allow-file-access-from-files");

    auto app{std::make_unique<App>()};

    set_title(app->m_hwnd.get(), "Airglow");
    enable_caption_color(app->m_hwnd.get(), false);
    set_system_backdrop(app->m_hwnd.get(), DWM_SYSTEMBACKDROP_TYPE::DWMSBT_MAINWINDOW);
    use_immersive_dark_mode(app->m_hwnd.get());

    // app->show_normal();

    app->dpi = glow::gui::get_dpi(app->m_hwnd.get());
    app->scale = glow::gui::get_scale(app->m_hwnd.get());

    app->m_browser1 = std::make_unique<Browser>(app->m_hwnd.get(), +WebViews::browser1);
    app->m_browser2 = std::make_unique<Browser>(app->m_hwnd.get(), +WebViews::browser2);
    // app->m_frame1 = std::make_unique<Frame>();
    // app->m_frame2 = std::make_unique<Frame>();
    app->m_addressBar1 = std::make_unique<AddressBar>(
        app->m_hwnd.get(), +WebViews::bar1, "https://localhost:8000/addressbar/index.html");
    app->m_addressBar2 = std::make_unique<AddressBar>(
        app->m_hwnd.get(), +WebViews::bar2, "https://localhost:8000/addressbar/index.html");

    app->m_browser1->show_normal();
    app->m_browser2->show_normal();
    app->m_addressBar1->show_normal();
    app->m_addressBar2->show_normal();

    app->m_settingsWindow = std::make_unique<SettingsWindow>();
    app->m_settingsWindow->show_normal();

    return message_loop();
}

auto App::run_server() -> int
{
    std::println("Starting server...");

    STARTUPINFOA si{sizeof(STARTUPINFOA)};
    PROCESS_INFORMATION pi{};

    auto server{(glow::filesystem::get_pgmptr() / "server.exe").string()};
    auto pServer{server.data()};

    std::println("Server path: {}", server);

    CreateProcessA(pServer, nullptr, nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi);
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}

auto CALLBACK App::EnumChildProc(HWND hWnd, LPARAM lParam) -> BOOL
{
    auto gwlId{static_cast<int64_t>(GetWindowLongPtrA(hWnd, GWL_ID))};
    auto rectParent{*std::bit_cast<LPRECT>(lParam)};

    auto position{rect_to_position(rectParent)};
    auto panelHeight{100};
    auto border{2};
    auto width{(position.width / 2) - border};
    auto height{(position.height) - panelHeight};
    auto rightX{width + (border * 2)};
    auto panelY{position.height - panelHeight};

    if (gwlId == +WebViews::browser1)
        SetWindowPos(hWnd, nullptr, 0, 0, width, height, SWP_NOZORDER);

    if (gwlId == +WebViews::browser2)
        SetWindowPos(hWnd, nullptr, rightX, 0, width, height, SWP_NOZORDER);

    if (gwlId == +WebViews::bar1)
        SetWindowPos(hWnd, nullptr, 0, panelY, width, panelHeight, SWP_NOZORDER);

    if (gwlId == +WebViews::bar2)
        SetWindowPos(hWnd, nullptr, rightX, panelY, width, panelHeight, SWP_NOZORDER);

    return TRUE;
}

auto App::handle_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    switch (uMsg)
    {
    // case WM_ACTIVATE: return on_activate(wParam);
    case WM_PARENTNOTIFY: return on_parent_notify(wParam);
    case WM_KEYDOWN: return on_key_down(wParam);
    case WM_MOVE: return on_move();
    case WM_SIZE: return on_size();
    }

    return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

// auto App::on_activate(WPARAM wParam) -> int
// {
//     if (m_frame1 && m_frame2)
//     {
//         if ((LOWORD(wParam) == WA_ACTIVE) || (LOWORD(wParam) == WA_CLICKACTIVE))
//         {
//             glow::gui::cloak(m_frame1->m_hwnd.get(), false);
//             glow::gui::cloak(m_frame2->m_hwnd.get(), false);
//             position_frame();
//             return 0;
//         }

//         if (LOWORD(wParam) == WA_INACTIVE)
//         {
//             glow::gui::cloak(m_frame1->m_hwnd.get(), true);
//             glow::gui::cloak(m_frame2->m_hwnd.get(), true);
//             return 0;
//         }
//         else return 0;
//     }

//     else return 0;
// }

auto App::on_key_down(WPARAM wParam) -> int
{
    switch (wParam)
    {
    case VK_PAUSE: OutputDebugStringA("PAUSE"); break;
    case 0x4C:
        if (GetKeyState(VK_CONTROL) & 0x8000) OutputDebugStringA("L");
        break;
    case 0x57:
        if (GetKeyState(VK_CONTROL) & 0x8000)
        {
            OutputDebugStringA("W");
            PostMessageA(m_hwnd.get(), WM_CLOSE, 0, 0);
        }
        break;
    case VK_F1: OutputDebugStringA("F1"); break;
    case VK_F2: OutputDebugStringA("F2"); break;
    case VK_F3: OutputDebugStringA("F3"); break;
    case VK_F4: OutputDebugStringA("F4"); break;
    case VK_F6: OutputDebugStringA("F6"); break;
    case VK_F8: OutputDebugStringA("F8"); break;
    case VK_F11: OutputDebugStringA("F11"); break;
    }

    return 0;
}

auto App::on_parent_notify(WPARAM wParam) -> int
{
    if (LOWORD(wParam) == WM_CREATE) on_size();

    return 0;
}

auto App::on_move() -> int
{
    // position_frame();

    return 0;
}

auto App::on_size() -> int
{
    RECT rect{0};
    GetClientRect(m_hwnd.get(), &rect);
    EnumChildWindows(m_hwnd.get(), EnumChildProc, std::bit_cast<LPARAM>(&rect));
    Sleep(1);

    // position_frame();

    return 0;
}

// auto App::position_frame() -> void
// {
//     if (m_frame1)
//     {
//         RECT wRect{};
//         GetWindowRect(m_browser1->m_hwnd.get(), &wRect);
//         auto position{rect_to_position(wRect)};
//         SetWindowPos(m_frame1->m_hwnd.get(), 0, position.x, position.y, position.width,
//                      position.height, SWP_NOACTIVATE);
//     }

//     if (m_frame2)
//     {
//         RECT wRect{};
//         GetWindowRect(m_browser2->m_hwnd.get(), &wRect);
//         auto position{rect_to_position(wRect)};
//         SetWindowPos(m_frame2->m_hwnd.get(), 0, position.x, position.y, position.width,
//                      position.height, SWP_NOACTIVATE);
//     }
// }
