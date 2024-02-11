// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#include "window.hxx"

Window::Window(HWND parent, State& state, intptr_t id)
    : glow::Window<Window>("Airglow", id), m_parent{parent}, m_state{state}
{
    dwm_caption_color(false);
    dwm_system_backdrop(DWMSBT_MAINWINDOW);
    theme();

    m_first.browser = std::make_unique<Browser>(hwnd());
    m_first.browser->reveal();

    m_second.browser = std::make_unique<Browser>(hwnd());
    m_second.browser->reveal();

    m_url.browser = std::make_unique<Browser>(hwnd());
    m_url.browser->reveal();
}

auto CALLBACK Window::EnumChildProc(HWND hWnd, LPARAM lParam) -> BOOL
{
    auto self{reinterpret_cast<Window*>(lParam)};

    if (!self) { return true; }

    auto gwlId{static_cast<intptr_t>(GetWindowLongPtrA(hWnd, GWL_ID))};
    auto& rect{self->m_clientRect};
    auto& layout{self->m_layout};

    auto& width{rect.right};
    auto& height{rect.bottom};
    auto halfWidth{width / 2};
    auto halfHeight{height / 2};

    auto border{static_cast<int>(layout.border * self->m_scale)};
    auto barHeight{static_cast<int>(layout.bar * self->m_scale)};

    auto& full = self->m_positions.full;
    auto& empty = self->m_positions.empty;
    auto& left = self->m_positions.left;
    auto& right = self->m_positions.right;
    auto& top = self->m_positions.top;
    auto& bottom = self->m_positions.bottom;

    auto& first = self->m_first.browser->m_position;
    auto& second = self->m_second.browser->m_position;
    auto& url = self->m_url.browser->m_position;

    full.x = 0;
    full.y = 0;
    full.width = width;
    full.height = height - barHeight;

    left.x = 0;
    left.y = 0;
    left.width = halfWidth - border;
    left.height = height - barHeight;

    right.x = halfWidth + border;
    right.y = 0;
    right.width = halfWidth - border;
    right.height = height - barHeight;

    top.x = 0;
    top.y = 0;
    top.width = width;
    top.height = halfHeight - (barHeight / 2) - border;

    bottom.x = 0;
    bottom.y = halfHeight - (barHeight / 2) + border;
    bottom.width = width;
    bottom.height = halfHeight - (barHeight / 2) - border;

    if (layout.split)
    {
        if (layout.vertical)
        {
            if (!layout.swap)
            {
                first = left;
                second = right;
            }

            else
            {
                first = right;
                second = left;
            }
        }

        else
        {
            if (!layout.swap)
            {
                first = top;
                second = bottom;
            }

            else
            {
                first = bottom;
                second = top;
            }
        }
    }

    else
    {
        if (!layout.swap)
        {
            first = full;
            second = empty;
        }

        else
        {
            first = empty;
            second = full;
        }
    }

    url.x = 0;
    url.y = rect.bottom - barHeight;
    url.width = width;
    url.height = barHeight;

    auto hdwp{BeginDeferWindowPos(3)};

    if (gwlId == self->m_first.browser->id())
    {
        if (hdwp && self->m_first.browser)
        {
            hdwp = DeferWindowPos(hdwp, hWnd, nullptr, first.x, first.y, first.width, first.height,
                                  SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOREDRAW |
                                      SWP_NOCOPYBITS);
        }
    }

    if (gwlId == self->m_second.browser->id())
    {
        if (hdwp && self->m_second.browser)
        {
            hdwp = DeferWindowPos(
                hdwp, hWnd, nullptr, second.x, second.y, second.width, second.height,
                SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOREDRAW | SWP_NOCOPYBITS);
        }
    }

    if (gwlId == self->m_url.browser->id())
    {
        if (hdwp && self->m_url.browser)
        {
            hdwp = DeferWindowPos(hdwp, hWnd, nullptr, url.x, url.y, url.width, url.height,
                                  SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOREDRAW |
                                      SWP_NOCOPYBITS);
        }
    }

    if (hdwp) { EndDeferWindowPos(hdwp); }

    return true;
}

auto Window::default_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    switch (uMsg)
    {
        case WM_CLOSE: return on_close(wParam, lParam);
        case WM_DPICHANGED: return on_dpi_changed(wParam, lParam);
        case WM_GETMINMAXINFO: return on_get_min_max_info(wParam, lParam);
        case WM_KEYDOWN: return on_key_down(wParam, lParam);
        case WM_NOTIFY: return on_notify(wParam, lParam);
        case WM_SETTINGCHANGE: return on_setting_change(wParam, lParam);
        case WM_SIZE: return on_size(wParam, lParam);
        case WM_SYSKEYDOWN: return on_sys_key_down(wParam, lParam);
    }

    return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

auto Window::on_close(WPARAM wParam, LPARAM lParam) -> int
{
    notify(m_parent, CODE::WINDOW_CLOSE);

    return close();
}

auto Window::on_dpi_changed(WPARAM wParam, LPARAM lParam) -> int
{
    dpi();
    scale();

    return 0;
}

auto Window::on_get_min_max_info(WPARAM wParam, LPARAM lParam) -> int
{
    auto minmax{reinterpret_cast<MINMAXINFO*>(lParam)};

    minmax->ptMinTrackSize.x = 500;
    minmax->ptMinTrackSize.y = 500;

    return 0;
}

auto Window::on_key_down(WPARAM wParam, LPARAM lParam) -> int
{
    auto key{static_cast<unsigned int>(wParam)};

    if ((HIWORD(lParam) & KF_REPEAT) == KF_REPEAT) { return 0; }

    if (m_keys.set.contains(key))
    {
        switch (key)
        {
            case VK_PAUSE:
            {
                notify(m_parent, CODE::SETTINGS_TOGGLE);

                break;
            }

            case 0x4C:
            {
                if (GetKeyState(VK_CONTROL) & 0x8000)
                {
                    m_url.browser->focus(COREWEBVIEW2_MOVE_FOCUS_REASON_PROGRAMMATIC);
                    m_url.browser->post_json(json{{"focus", m_layout.focus}});
                }

                break;
            }

            case 0x4E:
            {
                if (GetKeyState(VK_CONTROL) & 0x8000) { notify(m_parent, CODE::WINDOW_NEW); }

                break;
            }

            case 0x57:
            {
                if (GetKeyState(VK_CONTROL) & 0x8000)
                {
                    SendMessageA(hwnd(), WM_CLOSE, 0, 0);

                    return 0;
                }

                break;
            }

            case VK_F1:
            {
                m_layout.swap = !m_layout.swap;

                if (!m_layout.split)
                {
                    if (!m_layout.swap)
                    {
                        m_first.browser->focus(COREWEBVIEW2_MOVE_FOCUS_REASON_PROGRAMMATIC);
                    }

                    else if (m_layout.swap)
                    {
                        m_second.browser->focus(COREWEBVIEW2_MOVE_FOCUS_REASON_PROGRAMMATIC);
                    }
                }

                notify(hwnd(), CODE::LAYOUT_CHANGE);

                break;
            }

            case VK_F2:
            {
                m_layout.split = !m_layout.split;

                notify(hwnd(), CODE::LAYOUT_CHANGE);

                break;
            }

            case VK_F3:
            {
                if (m_layout.split) { m_layout.vertical = !m_layout.vertical; }

                notify(hwnd(), CODE::LAYOUT_CHANGE);

                break;
            }

            case VK_F4:
            {
                if (GetKeyState(VK_MENU) & 0x8000)
                {
                    SendMessageA(hwnd(), WM_CLOSE, 0, 0);

                    return 0;
                }

                else
                {
                    m_position.maximize = !m_position.maximize;
                    maximize();

                    notify(hwnd(), CODE::LAYOUT_CHANGE);
                }

                break;
            }

            case VK_F6:
            {
                break;
            }

            case VK_F7:
            {
                break;
            }

            case VK_F8:
            {
                break;
            }

            case VK_F9:
            {
                m_position.topmost = !m_position.topmost;
                topmost();

                notify(hwnd(), CODE::LAYOUT_CHANGE);

                break;
            }

            case VK_F11:
            {
                m_position.fullscreen = !m_position.fullscreen;
                fullscreen();

                notify(hwnd(), CODE::LAYOUT_CHANGE);

                break;
            }

            case VK_F12:
            {
                break;
            }
        }
    }

    return 0;
}

auto Window::on_notify(WPARAM wParam, LPARAM lParam) -> int
{
    auto notification{reinterpret_cast<glow::Notification*>(lParam)};

    switch (notification->code)
    {
        using enum CODE;

        case BROWSER_INIT:
        {
            if (notification->id == m_url.browser->id())
            {
                m_url.browser->devtools();
                m_url.browser->navigate(m_url.browser->url("url"));
            }

            break;
        }

        case WEB_MESSAGE_RECEIVED:
        {
            auto webMessage{json::parse(notification->message)};

            logger(webMessage.dump());

            if (webMessage.contains("initialized"))
            {
                m_url.browser->post_json(json(*this));

                if (!m_init)
                {
                    m_init = true;
                    m_url.browser->post_json(json{{"navigate", m_state.args}});
                }
            }

            else if (webMessage.contains("height"))
            {
                m_layout.bar = webMessage["height"].get<int>();
                SendMessageA(hwnd(), WM_SIZE, 0, 0);
            }

            else if (webMessage.contains("first"))
            {
                m_first.browser->navigate(webMessage["first"].get<std::string>());
            }

            else if (webMessage.contains("second"))
            {
                m_second.browser->navigate(webMessage["second"].get<std::string>());
            }

            break;
        }

        case SOURCE_CHANGED:
        {
            if (notification->id == m_first.browser->id())
            {
                m_first.source.assign(m_first.browser->m_source);
            }

            else if (notification->id == m_second.browser->id())
            {
                m_second.source.assign(m_second.browser->m_source);
            }

            m_url.browser->post_json(json(*this));

            notify(m_parent, SETTINGS_SAVE);

            break;
        }

        case FAVICON_CHANGED:
        {
            if (notification->id == m_first.browser->id())
            {
                m_first.hicon.reset(m_first.browser->m_hicon.get());
                m_first.favicon.assign(m_first.browser->m_favicon);
            }

            else if (notification->id == m_second.browser->id())
            {
                m_second.hicon.reset(m_second.browser->m_hicon.get());
                m_second.favicon.assign(m_second.browser->m_favicon);
            }

            m_url.browser->post_json(json(*this));

            update_caption();

            break;
        }

        case TITLE_CHANGED:
        {
            if (notification->id == m_first.browser->id())
            {
                m_first.title.assign(m_first.browser->m_title);
            }

            else if (notification->id == m_second.browser->id())
            {
                m_second.title.assign(m_second.browser->m_title);
            }

            m_url.browser->post_json(json(*this));

            update_caption();

            break;
        }

        case FOCUS_CHANGED:
        {
            if (notification->id == m_first.browser->id()) { m_layout.focus = "first"; }

            else if (notification->id == m_second.browser->id()) { m_layout.focus = "second"; }

            // else if (notification->id == m_url.browser->id()) { m_layout.focus = "url"; }

            m_url.browser->post_json(json(*this));

            break;
        }

        case LAYOUT_CHANGE:
        {
            PostMessageA(hwnd(), WM_SIZE, 0, 0);
            update_caption();

            m_url.browser->post_json(json(*this));

            break;
        }
    }

    return 0;
}

auto Window::on_setting_change(WPARAM wParam, LPARAM lParam) -> int
{
    theme();
    // if (m_url.browser) { m_url.browser->post_json(json(*this)); }

    return 0;
}

auto Window::on_size(WPARAM wParam, LPARAM lParam) -> int
{
    client_rect();
    EnumChildWindows(hwnd(), EnumChildProc, reinterpret_cast<intptr_t>(this));

    return 0;
}

auto Window::on_sys_key_down(WPARAM wParam, LPARAM lParam) -> int
{
    auto key{static_cast<unsigned int>(wParam)};

    if ((HIWORD(lParam) & KF_REPEAT) == KF_REPEAT) return 0;

    if (m_keys.set.contains(key))
    {
        switch (key)
        {
            case VK_F10:
            {
                break;
            }
        }
    }

    return 0;
}

auto Window::update_caption() -> void
{
    if (!m_position.fullscreen)
    {
        if (!m_layout.swap) { icon(m_first.hicon.get(), true, false); }

        else { icon(m_second.hicon.get(), true, false); }

        icon(m_hicon.get(), false, true);
    }

    if (!m_layout.swap) { title(m_first.title); }

    else { title(m_second.title); }
}
