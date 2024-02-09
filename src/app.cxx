// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#include "app.hxx"

App::App()
{
    SetEnvironmentVariableA("WEBVIEW2_DEFAULT_BACKGROUND_COLOR", "0");
    SetEnvironmentVariableA("WEBVIEW2_ADDITIONAL_BROWSER_ARGUMENTS",
                            "--allow-file-access-from-files");

    m_settings = std::make_unique<Settings>(hwnd(), m_url, m_colors);
}

auto App::wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    switch (uMsg)
    {
        case WM_NOTIFY: return on_notify(wParam, lParam);
    }

    return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

auto App::on_notify(WPARAM wParam, LPARAM lParam) -> int
{
    auto notification{reinterpret_cast<glow::Notification*>(lParam)};

    auto& id{notification->id};
    auto& code{notification->code};

    switch (code)
    {
        using enum CODE;

        case SETTING_CHANGE:
        {
            m_colors.update();

            break;
        }

        case SETTINGS_TOGGLE:
        {
            m_settings->visible() ? m_settings->hide() : m_settings->show();

            if (m_settings->visible())
            {
                m_settings->m_browser->focus(COREWEBVIEW2_MOVE_FOCUS_REASON_PROGRAMMATIC);
            }

            break;
        }

        case SETTINGS_SAVE:
        {
            m_settings->save();

            break;
        }

        case WINDOW_NEW:
        {
            new_window();

            break;
        }

        case WINDOW_CLOSE:
        {
            m_windows.erase(id);

            if (m_windows.empty())
            {
                m_settings->save();

                return close();
            }

            break;
        }
    }

    return 0;
}

auto App::new_window() -> void
{
    auto id{glow::random<uintptr_t>()};
    m_windows[id] = std::make_unique<Window>(hwnd(), m_url, m_colors, id);
    m_windows[id]->reveal();
}
