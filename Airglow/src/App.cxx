// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#include "App.hxx"

auto App::run() -> void
{
    App app;

    set_title(app.m_hwnd.get(), "Airglow");
    enable_caption_color(app.m_hwnd.get(), false);
    set_system_backdrop(app.m_hwnd.get(), DWM_SYSTEMBACKDROP_TYPE::DWMSBT_MAINWINDOW);
    use_immersive_dark_mode(app.m_hwnd.get());

    // nullptr here, need to set a virtual initialization function for each browser
    // app.m_browser3.m_settings8->put_IsZoomControlEnabled(false);
    // app.m_browser4.m_settings8->put_IsZoomControlEnabled(false);

    message_loop();
}

auto App::handle_message(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    switch (uMsg)
    {
    case WM_KEYDOWN: return on_key_down(wParam);
    case WM_SIZE: return on_size();
    }

    return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

auto CALLBACK App::enum_child_proc(HWND hWnd, LPARAM lParam) -> BOOL
{
    auto self{InstanceFromEnumChildProc<App>(hWnd, lParam)};

    if (self)
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

        if (gwlId == 3)
        {
            SetWindowPos(hWnd, nullptr, 0, panelY, width, panelHeight, SWP_NOZORDER);
        }

        if (gwlId == 4)
            SetWindowPos(hWnd, nullptr, rightX, panelY, width, panelHeight, SWP_NOZORDER);
    }

    return TRUE;
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

auto App::on_size() -> int
{
    RECT clientRect{0};
    GetClientRect(m_hwnd.get(), &clientRect);
    EnumChildWindows(m_hwnd.get(), enum_child_proc, std::bit_cast<LPARAM>(&clientRect));
    Sleep(1);

    return 0;
}
