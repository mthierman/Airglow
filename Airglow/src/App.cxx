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

    app->show_normal();

    app->m_browser1 = std::make_unique<Browser>(app->m_hwnd.get(), 1);
    app->m_browser2 = std::make_unique<Browser>(app->m_hwnd.get(), 2);
    app->m_browser3 = std::make_unique<Browser>(app->m_hwnd.get(), 3);
    app->m_browser4 = std::make_unique<Browser>(app->m_hwnd.get(), 4);

    app->m_browser1->show_normal();
    app->m_browser2->show_normal();
    app->m_browser3->show_normal();
    app->m_browser4->show_normal();

    // nullptr here, need to set a virtual initialization function for each browser
    // app.m_browser3.m_settings8->put_IsZoomControlEnabled(false);
    // app.m_browser4.m_settings8->put_IsZoomControlEnabled(false);

    app->m_settingsWindow = std::make_unique<SettingsWindow>();
    app->m_settingsWindow->show_normal();

    return message_loop();
}

auto CALLBACK App::EnumChildProc(HWND hWnd, LPARAM lParam) -> BOOL
{
    auto gwlId{GetWindowLongPtrA(hWnd, GWL_ID)};
    auto rectParent{*std::bit_cast<LPRECT>(lParam)};

    auto position{rect_to_position(rectParent)};
    auto panelHeight{32};
    auto border{2};
    auto width{(position.width / 2) - border};
    auto height{(position.height) - panelHeight};
    auto rightX{width + (border * 2)};
    auto panelY{position.height - panelHeight};

    if (gwlId == 1) { SetWindowPos(hWnd, nullptr, 0, 0, width, height, SWP_NOZORDER); }

    if (gwlId == 2) { SetWindowPos(hWnd, nullptr, rightX, 0, width, height, SWP_NOZORDER); }

    if (gwlId == 3) { SetWindowPos(hWnd, nullptr, 0, panelY, width, panelHeight, SWP_NOZORDER); }

    if (gwlId == 4) SetWindowPos(hWnd, nullptr, rightX, panelY, width, panelHeight, SWP_NOZORDER);

    return TRUE;
}

auto App::handle_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    switch (uMsg)
    {
    case WM_PARENTNOTIFY: return on_parent_notify(wParam);
    case WM_KEYDOWN: return on_key_down(wParam);
    case WM_SIZE: return on_size();
    }

    return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

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

auto App::on_size() -> int
{
    RECT rect{0};
    GetClientRect(m_hwnd.get(), &rect);
    EnumChildWindows(m_hwnd.get(), EnumChildProc, std::bit_cast<LPARAM>(&rect));
    Sleep(1);

    return 0;
}
