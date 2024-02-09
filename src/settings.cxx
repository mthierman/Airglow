// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#include "settings.hxx"

Settings::Settings(HWND app, URL& url, glow::Colors& colors)
    : glow::Window<Settings>("Airglow - Settings"), m_app{app}, m_url{url}, m_colors{colors},
      m_file{file()}
{
    auto& current{m_url.current};
    auto& home{m_url.home};

    if (!std::filesystem::exists(m_file)) { save(); }

    else { load(); }

    auto args{glow::cmd_to_argv()};

    if (args.size() == 2)
    {
        current.first = args.at(1);
        current.second = home.second;
    }

    else if (args.size() > 2)
    {
        current.first = args.at(1);
        current.second = args.at(2);
    }

    else
    {
        current.first = home.first;
        current.second = home.second;
    }

    dwm_caption_color(false);
    dwm_system_backdrop(DWM_SYSTEMBACKDROP_TYPE::DWMSBT_TRANSIENTWINDOW);
    theme();

    SetWindowPos(hwnd(), nullptr, 0, 0, 500, 500, SWP_NOMOVE);

    m_browser = std::make_unique<SettingsBrowser>(hwnd());
    m_browser->reveal();
}

auto CALLBACK Settings::EnumChildProc(HWND hWnd, LPARAM lParam) -> BOOL
{
    auto self{reinterpret_cast<Settings*>(lParam)};

    if (self)
    {
        auto gwlId{static_cast<uintptr_t>(GetWindowLongPtrA(hWnd, GWL_ID))};

        auto rect{&self->m_clientRect};

        auto width{rect->right - rect->left};
        auto height{rect->bottom - rect->top};

        auto hdwp{BeginDeferWindowPos(1)};

        if (gwlId == self->m_browser->id())
        {
            if (hdwp && self->m_browser)
            {
                hdwp = DeferWindowPos(hdwp, hWnd, nullptr, 0, 0, width, height,
                                      SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOOWNERZORDER |
                                          SWP_NOREDRAW | SWP_NOCOPYBITS);
            }
        }

        if (hdwp) { EndDeferWindowPos(hdwp); }
    }

    return TRUE;
}

auto Settings::default_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    switch (uMsg)
    {
        case WM_CLOSE: return on_close(wParam, lParam);
        case WM_DPICHANGED: return on_dpi_changed(wParam, lParam);
        case WM_KEYDOWN: return on_key_down(wParam, lParam);
        case WM_GETMINMAXINFO: return on_get_min_max_info(wParam, lParam);
        case WM_NOTIFY: return on_notify(wParam, lParam);
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

auto Settings::on_dpi_changed(WPARAM wParam, LPARAM lParam) -> int
{
    dpi();
    scale();

    notify(m_app, msg::dpi_change);

    return 0;
}

auto Settings::on_notify(WPARAM wParam, LPARAM lParam) -> int
{
    if (!m_browser) { return 0; }

    auto notification{reinterpret_cast<glow::Notification*>(lParam)};

    auto& code{notification->nmhdr.code};
    auto& message{notification->message};

    switch (code)
    {
        case msg::web_message_received:
        {
            auto webMessage{json::parse(message)};

            if (webMessage.contains("initialized"))
            {
                if (!m_init) { m_init = true; }

                m_browser->post_json(json(*this));
            }

            else if (webMessage.contains("first"))
            {
                m_url.home.first = webMessage["first"].get<std::string>();
                notify(m_app, msg::settings_save);
            }

            else if (webMessage.contains("second"))
            {
                m_url.home.second = webMessage["second"].get<std::string>();
                notify(m_app, msg::settings_save);
            }

            break;
        }
    }

    return 0;
}

auto Settings::on_get_min_max_info(WPARAM wParam, LPARAM lParam) -> int
{
    auto minmax{reinterpret_cast<LPMINMAXINFO>(lParam)};

    minmax->ptMinTrackSize.x = 500;
    minmax->ptMinTrackSize.y = 500;

    return 0;
}

auto Settings::on_key_down(WPARAM wParam, LPARAM lParam) -> int
{
    auto key{static_cast<unsigned int>(wParam)};
    Keys keys;

    if ((HIWORD(lParam) & KF_REPEAT) == KF_REPEAT) { return 0; }

    if (keys.set.contains(key))
    {
        switch (key)
        {
            case VK_PAUSE:
            {
                notify(m_app, msg::settings_toggle);

                break;
            }

            case 0x57:
            {
                if (GetKeyState(VK_CONTROL) & 0x8000) { notify(m_app, msg::settings_toggle); }

                break;
            }

            case VK_F4:
            {
                if (GetKeyState(VK_MENU) & 0x8000) { notify(m_app, msg::settings_toggle); }

                break;
            }
        }
    }

    return 0;
}

auto Settings::on_setting_change(WPARAM wParam, LPARAM lParam) -> int
{
    theme();
    // if (m_browser) { m_browser->post_json(json(*this)); }

    notify(m_app, msg::setting_change);

    return 0;
}

auto Settings::on_show_window(WPARAM wParam, LPARAM lParam) -> int
{
    auto& controller{m_browser->m_webView.controller4};

    if (controller)
    {
        switch (wParam)
        {
            case TRUE:
            {
                controller->put_IsVisible(TRUE);

                break;
            }

            case FALSE:
            {
                controller->put_IsVisible(FALSE);

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
    // Sleep(1);

    return 0;
}

auto Settings::data() -> std::filesystem::path
{
    auto path{glow::known_folder() / "Airglow"};

    if (!std::filesystem::exists(path)) { std::filesystem::create_directory(path); }

    return path;
}

auto Settings::file() -> std::filesystem::path
{
    return std::filesystem::path{{glow::app_path() / "Airglow.json"}};
}

auto Settings::save() -> void
{
    try
    {
        std::ofstream f(m_file);
        f << std::setw(4) << nlohmann::json(m_url) << std::endl;
        f.close();
    }
    catch (const std::exception& e)
    {
        return;
    }
}

auto Settings::load() -> void
{
    try
    {
        std::ifstream f(m_file);
        m_url = nlohmann::json::parse(f, nullptr, false, true);
        f.close();
    }
    catch (const std::exception& e)
    {
        return;
    }
}
