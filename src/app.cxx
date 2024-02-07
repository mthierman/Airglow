// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#include "app.hxx"

auto App::operator()(int argc, char* argv[]) -> int
{
    SetEnvironmentVariableA("WEBVIEW2_DEFAULT_BACKGROUND_COLOR", "0");
    SetEnvironmentVariableA("WEBVIEW2_ADDITIONAL_BROWSER_ARGUMENTS",
                            "--allow-file-access-from-files");

    m_settings = std::make_unique<Settings>(hwnd(), m_url);

    auto args{glow::argv(argc, argv)};

    if (args.size() == 2)
    {
        m_url.current["first"] = args.at(1);
        m_url.current["second"] = m_url.home["second"];
    }

    else if (args.size() > 2)
    {
        m_url.current["first"] = args.at(1);
        m_url.current["second"] = args.at(2);
    }

    else
    {
        m_url.current["first"] = m_url.home["first"];
        m_url.current["second"] = m_url.home["second"];
    }

    window();

    return glow::message_loop();
}

auto App::window() -> void
{
    auto id{glow::random<uintptr_t>()};
    m_windows[id] = std::make_unique<Window>(hwnd(), m_url, id);
    m_windows[id]->reveal();
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

    auto& id = notification->nmhdr.idFrom;
    auto& code = notification->nmhdr.code;

    switch (code)
    {
        case msg::toggle_settings:
        {
            m_settings->visible() ? m_settings->hide() : m_settings->show();

            break;
        }

        case msg::save_settings:
        {
            m_settings->save();

            break;
        }

        case msg::close_window:
        {
            m_windows.erase(id);

            if (m_windows.empty())
            {
                m_settings->save();

                return close();
            }
        }
    }

    return 0;
}
