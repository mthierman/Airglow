// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#include "settings.hxx"

Settings::Settings(HWND app) : BaseWindow("Airglow - Settings")
{
    m_app = app;

    dwm_caption_color(false);
    dwm_system_backdrop(DWM_SYSTEMBACKDROP_TYPE::DWMSBT_TRANSIENTWINDOW);
    theme();

    m_main = std::make_unique<SettingsBrowser>(hwnd());
    m_main->reveal();
}

auto CALLBACK Settings::EnumChildProc(HWND hWnd, LPARAM lParam) -> BOOL
{
    auto self{reinterpret_cast<Settings*>(lParam)};

    if (self)
    {
        auto gwlId{static_cast<intptr_t>(GetWindowLongPtrA(hWnd, GWL_ID))};

        auto r{&self->m_clientRect};
        auto width{r->right - r->left};
        auto height{r->bottom - r->top};

        auto hdwp{BeginDeferWindowPos(1)};

        if (gwlId == self->m_main->id())
            if (hdwp && self->m_main)
                hdwp = DeferWindowPos(hdwp, hWnd, nullptr, 0, 0, width, height,
                                      SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOOWNERZORDER |
                                          SWP_NOREDRAW | SWP_NOCOPYBITS);

        if (hdwp) EndDeferWindowPos(hdwp);
    }

    return TRUE;
}

auto Settings::default_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    switch (uMsg)
    {
    case WM_CLOSE: return on_close(wParam, lParam);
    case WM_KEYDOWN: return on_key_down(wParam, lParam);
    case WM_SETTINGCHANGE: return on_setting_change(wParam, lParam);
    case WM_SHOWWINDOW: return on_show_window(wParam, lParam);
    case WM_SIZE: return on_size(wParam, lParam);
    }

    return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

auto Settings::on_close(WPARAM wParam, LPARAM lParam) -> int
{
    hide();

    return 0;
}

auto Settings::on_key_down(WPARAM wParam, LPARAM lParam) -> int
{
    auto key{static_cast<unsigned int>(wParam)};
    Keys keys;

    if ((HIWORD(lParam) & KF_REPEAT) == KF_REPEAT) return 0;

    if (keys.set.contains(key))
    {
        switch (key)
        {
        case VK_PAUSE:
        {
            notify(m_app, msg::toggle_settings);

            break;
        }

        case 0x57:
        {
            if (GetKeyState(VK_CONTROL) & 0x8000) notify(m_app, msg::toggle_settings);

            break;
        }

        case VK_F4:
        {
            if (GetKeyState(VK_MENU) & 0x8000) notify(m_app, msg::toggle_settings);

            break;
        }
        }
    }

    return 0;
}

auto Settings::on_setting_change(WPARAM wParam, LPARAM lParam) -> int
{
    theme();

    return 0;
}

auto Settings::on_show_window(WPARAM wParam, LPARAM lParam) -> int
{
    if (m_main->m_webView.controller4)
    {
        switch (wParam)
        {
        case TRUE:
        {
            m_visible = true;
            m_main->m_webView.controller4->put_IsVisible(TRUE);

            break;
        }

        case FALSE:
        {
            m_visible = false;
            m_main->m_webView.controller4->put_IsVisible(FALSE);

            break;
        }
        }
    }

    return 0;
}

auto Settings::on_size(WPARAM wParam, LPARAM lParam) -> int
{
    client_rect();
    EnumChildWindows(hwnd(), EnumChildProc, reinterpret_cast<intptr_t>(this));
    Sleep(1);

    return 0;
}
