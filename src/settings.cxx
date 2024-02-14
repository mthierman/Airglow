// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#include "settings.hxx"

Settings::Settings(::HWND app, State& state)
    : glow::Window("Airglow - Settings"), m_app{app}, m_state{state}
{
    dwm_caption_color(false);
    dwm_system_backdrop(DWMSBT_TRANSIENTWINDOW);
    theme();

    ::SetWindowPos(m_hwnd.get(), nullptr, 0, 0, 500, 500, SWP_NOMOVE);

    std::function<HRESULT()> settingsCallback{[=, this]()
                                              {
                                                  m_browser->navigate(m_browser->url("settings"));

                                                  return S_OK;
                                              }};

    m_browser = std::make_unique<Browser>(m_hwnd.get(), settingsCallback);
    m_browser->reveal();
}

auto CALLBACK Settings::EnumChildProc(HWND hWnd, LPARAM lParam) -> BOOL
{
    auto self{reinterpret_cast<Settings*>(lParam)};

    if (!self) { return true; }

    auto gwlId{static_cast<size_t>(::GetWindowLongPtrA(hWnd, GWL_ID))};
    auto& rect{self->m_client.rect};
    auto& width{rect.right};
    auto& height{rect.bottom};

    auto hdwp{::BeginDeferWindowPos(1)};

    if (gwlId == self->m_browser->m_id)
    {
        if (hdwp && self->m_browser)
        {
            hdwp = ::DeferWindowPos(hdwp, hWnd, nullptr, 0, 0, width, height,
                                    SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOOWNERZORDER |
                                        SWP_NOREDRAW | SWP_NOCOPYBITS);
        }
    }

    if (hdwp) { ::EndDeferWindowPos(hdwp); }

    return true;
}

auto Settings::wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    switch (uMsg)
    {
        case WM_GETMINMAXINFO: return on_get_min_max_info(wParam, lParam);
        case WM_KEYDOWN: return on_key_down(wParam, lParam);
        case WM_NOTIFY: return on_notify(wParam, lParam);
        case WM_SETTINGCHANGE: return on_setting_change(wParam, lParam);
        case WM_SHOWWINDOW: return on_show_window(wParam, lParam);
        case WM_SIZE: return on_size(wParam, lParam);
    }

    return ::DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

auto Settings::on_close(WPARAM wParam, LPARAM lParam) -> int
{
    hide();

    return 0;
}

auto Settings::on_get_min_max_info(WPARAM wParam, LPARAM lParam) -> int
{
    auto minmax{reinterpret_cast<MINMAXINFO*>(lParam)};

    minmax->ptMinTrackSize.x = 500;
    minmax->ptMinTrackSize.y = 500;

    return 0;
}

auto Settings::on_key_down(WPARAM wParam, LPARAM lParam) -> int
{
    auto key{static_cast<unsigned int>(wParam)};

    if ((HIWORD(lParam) & KF_REPEAT) == KF_REPEAT) { return 0; }

    if (m_keys.set.contains(key))
    {
        switch (key)
        {
            case VK_PAUSE:
            {
                notify(m_app, CODE::SETTINGS_TOGGLE);

                break;
            }

            case 0x57:
            {
                if (::GetKeyState(VK_CONTROL) & 0x8000) { notify(m_app, CODE::SETTINGS_TOGGLE); }

                break;
            }

            case VK_F4:
            {
                if (::GetKeyState(VK_MENU) & 0x8000) { notify(m_app, CODE::SETTINGS_TOGGLE); }

                break;
            }
        }
    }

    return 0;
}

auto Settings::on_notify(WPARAM wParam, LPARAM lParam) -> int
{
    auto notification{reinterpret_cast<glow::Notification*>(lParam)};

    switch (notification->code)
    {
        using enum CODE;

        case WEB_MESSAGE_RECEIVED:
        {
            auto webMessage{json::parse(notification->message)};

            if (webMessage.contains("initialized")) { m_browser->post_json(json(*this)); }

            else if (webMessage.contains("m_state"))
            {
                m_state = webMessage["m_state"].get<State>();
                notify(m_app, CODE::SETTINGS_SAVE);
                m_browser->post_json(json(*this));
            }

            break;
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
    switch (wParam)
    {
        case true:
        {
            m_browser->put_is_visible(true);

            break;
        }

        case false:
        {
            m_browser->put_is_visible(false);

            break;
        }
    }

    return 0;
}

auto Settings::on_size(WPARAM wParam, LPARAM lParam) -> int
{
    position();
    ::EnumChildWindows(m_hwnd.get(), EnumChildProc, reinterpret_cast<size_t>(this));

    return 0;
}
