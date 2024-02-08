// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#include "window.hxx"

Window::Window(HWND app, URL& url, glow::Colors& colors, uintptr_t id)
    : glow::Window<Window>("Airglow", id), m_app{app}, m_url{url}, m_colors{colors}
{
    dwm_caption_color(false);
    dwm_system_backdrop(DWM_SYSTEMBACKDROP_TYPE::DWMSBT_MAINWINDOW);
    theme();

    m_browsers.first = std::make_unique<MainBrowser>(hwnd());
    m_browsers.first->reveal();

    m_browsers.second = std::make_unique<SideBrowser>(hwnd());
    m_browsers.second->reveal();

    m_browsers.url = std::make_unique<URLBrowser>(hwnd());
    m_browsers.url->reveal();
}

auto CALLBACK Window::EnumChildProc(HWND hWnd, LPARAM lParam) -> BOOL
{
    auto self{reinterpret_cast<Window*>(lParam)};

    if (self)
    {
        auto gwlId{static_cast<uintptr_t>(GetWindowLongPtrA(hWnd, GWL_ID))};

        auto rect{&self->m_clientRect};

        auto width{rect->right - rect->left};
        auto halfWidth{(rect->right - rect->left) / 2};

        auto height{rect->bottom - rect->top};
        auto halfHeight{(rect->bottom - rect->top) / 2};

        auto border{static_cast<int>(self->m_layout.border * self->m_scale)};
        auto barHeight{static_cast<int>(self->m_layout.bar * self->m_scale)};

        auto full = &self->m_positions.full;
        auto empty = &self->m_positions.empty;
        auto left = &self->m_positions.left;
        auto right = &self->m_positions.right;
        auto top = &self->m_positions.top;
        auto bottom = &self->m_positions.bottom;

        auto first = &self->m_browsers.first->m_position;
        auto second = &self->m_browsers.second->m_position;
        auto url = &self->m_browsers.url->m_position;

        full->x = 0;
        full->y = 0;
        full->width = width;
        full->height = height - barHeight;

        left->x = 0;
        left->y = 0;
        left->width = halfWidth - border;
        left->height = height - barHeight;

        right->x = halfWidth + border;
        right->y = 0;
        right->width = halfWidth - border;
        right->height = height - barHeight;

        top->x = 0;
        top->y = 0;
        top->width = width;
        top->height = halfHeight - (barHeight / 2) - border;

        bottom->x = 0;
        bottom->y = halfHeight - (barHeight / 2) + border;
        bottom->width = width;
        bottom->height = halfHeight - (barHeight / 2) - border;

        if (self->m_layout.split)
        {
            if (!self->m_layout.horizontal)
            {
                if (!self->m_layout.swapped)
                {
                    *first = *left;
                    *second = *right;
                }

                if (self->m_layout.swapped)
                {
                    *first = *right;
                    *second = *left;
                }
            }

            if (self->m_layout.horizontal)
            {
                if (!self->m_layout.swapped)
                {
                    *first = *top;
                    *second = *bottom;
                }

                if (self->m_layout.swapped)
                {
                    *first = *bottom;
                    *second = *top;
                }
            }
        }

        else
        {
            if (!self->m_layout.swapped)
            {
                *first = *full;
                *second = *empty;
            }

            if (self->m_layout.swapped)
            {
                *first = *empty;
                *second = *full;
            }
        }

        url->x = 0;
        url->y = rect->bottom - barHeight;
        url->width = width;
        url->height = barHeight;

        auto hdwp{BeginDeferWindowPos(3)};

        if (gwlId == self->m_browsers.first->id())
        {
            if (hdwp && self->m_browsers.first)
            {
                hdwp = DeferWindowPos(hdwp, hWnd, nullptr, first->x, first->y, first->width,
                                      first->height,
                                      SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOOWNERZORDER |
                                          SWP_NOREDRAW | SWP_NOCOPYBITS);
            }
        }

        if (gwlId == self->m_browsers.second->id())
        {
            if (hdwp && self->m_browsers.second)
            {
                hdwp = DeferWindowPos(hdwp, hWnd, nullptr, second->x, second->y, second->width,
                                      second->height,
                                      SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOOWNERZORDER |
                                          SWP_NOREDRAW | SWP_NOCOPYBITS);
            }
        }

        if (gwlId == self->m_browsers.url->id())
        {
            if (hdwp && self->m_browsers.url)
            {
                hdwp = DeferWindowPos(hdwp, hWnd, nullptr, url->x, url->y, url->width, url->height,
                                      SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOOWNERZORDER |
                                          SWP_NOREDRAW | SWP_NOCOPYBITS);
            }
        }

        if (hdwp) { EndDeferWindowPos(hdwp); }
    }

    return TRUE;
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
    notify(m_app, msg::close_window);

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
    auto minmax{reinterpret_cast<LPMINMAXINFO>(lParam)};

    minmax->ptMinTrackSize.x = 500;
    minmax->ptMinTrackSize.y = 500;

    return 0;
}

auto Window::on_key_down(WPARAM wParam, LPARAM lParam) -> int
{
    if (!m_browsers.url) { return 0; }

    auto key{static_cast<unsigned int>(wParam)};
    Keys keys;

    if ((HIWORD(lParam) & KF_REPEAT) == KF_REPEAT) { return 0; }

    if (keys.set.contains(key))
    {
        switch (key)
        {
            case VK_PAUSE:
            {
                notify(m_app, msg::toggle_settings);

                break;
            }

            case 0x4C:
            {
                if (GetKeyState(VK_CONTROL) & 0x8000)
                {
                    if (m_browsers.url)
                    {
                        m_browsers.url->focus(COREWEBVIEW2_MOVE_FOCUS_REASON::
                                                  COREWEBVIEW2_MOVE_FOCUS_REASON_PROGRAMMATIC);
                    }
                }

                break;
            }

            case 0x4E:
            {
                if (GetKeyState(VK_CONTROL) & 0x8000) { notify(m_app, msg::new_window); }

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
                if (!m_layout.split && m_layout.swapped)
                {
                    m_browsers.first->focus(COREWEBVIEW2_MOVE_FOCUS_REASON::
                                                COREWEBVIEW2_MOVE_FOCUS_REASON_PROGRAMMATIC);
                }

                else if (!m_layout.split && !m_layout.swapped)
                {
                    m_browsers.second->focus(COREWEBVIEW2_MOVE_FOCUS_REASON::
                                                 COREWEBVIEW2_MOVE_FOCUS_REASON_PROGRAMMATIC);
                }

                m_layout.swapped = !m_layout.swapped;

                break;
            }

            case VK_F2:
            {
                m_layout.split = !m_layout.split;

                break;
            }

            case VK_F3:
            {
                if (m_layout.split) { m_layout.horizontal = !m_layout.horizontal; }

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

                break;
            }

            case VK_F11:
            {
                m_position.fullscreen = !m_position.fullscreen;
                fullscreen();

                break;
            }

            case VK_F12:
            {
                break;
            }
        }
    }

    if (m_browsers.url) { m_browsers.url->post_json(json(*this)); }

    PostMessageA(hwnd(), WM_SIZE, 0, 0);
    notify(hwnd(), msg::title_changed);
    notify(hwnd(), msg::favicon_changed);

    return 0;
}

auto Window::on_notify(WPARAM wParam, LPARAM lParam) -> int
{
    auto notification{reinterpret_cast<glow::Notification*>(lParam)};

    auto& id{notification->nmhdr.idFrom};
    auto& code{notification->nmhdr.code};
    auto& message{notification->message};

    switch (code)
    {
        case msg::web_message_received:
        {
            auto webMessage{json::parse(message)};

            if (webMessage.contains("initialized"))
            {
                if (m_browsers.url) { m_browsers.url->post_json(json(*this)); }
            }

            else if (webMessage.contains("height"))
            {
                m_layout.bar = webMessage["height"].get<int>();
                PostMessageA(hwnd(), WM_SIZE, 0, 0);
            }

            else if (webMessage.contains("first"))
            {
                if (m_browsers.first)
                {
                    m_browsers.first->navigate(webMessage["first"].get<std::string>());
                }
            }

            else if (webMessage.contains("second"))
            {
                if (m_browsers.second)
                {
                    m_browsers.second->navigate(webMessage["second"].get<std::string>());
                }
            }

            break;
        }

        case msg::source_changed:
        {
            if (!m_browsers.url) { break; }

            if (id == m_browsers.first->id()) { m_url.current.first = m_browsers.first->m_source; }

            else if (id == m_browsers.second->id())
            {
                m_url.current.second = m_browsers.second->m_source;
            }

            m_browsers.url->post_json(json(*this));

            notify(m_app, msg::save_settings);

            break;
        }

        case msg::favicon_changed:
        {
            if (!m_browsers.url) { break; }

            if (id == m_browsers.first->id())
            {
                m_favicon.first.reset(m_browsers.first->m_favicon.get());
                m_faviconUrl.first.assign(m_browsers.first->m_faviconUrl);
            }

            else if (id == m_browsers.second->id())
            {
                m_favicon.second.reset(m_browsers.second->m_favicon.get());
                m_faviconUrl.second.assign(m_browsers.second->m_faviconUrl);
            }

            if (!m_layout.swapped && !m_position.fullscreen)
            {
                PostMessageA(hwnd(), WM_SETICON, ICON_SMALL,
                             reinterpret_cast<LPARAM>(m_favicon.first.get()));
                PostMessageA(hwnd(), WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(m_hicon.get()));
            }

            else if (!m_position.fullscreen)
            {
                PostMessageA(hwnd(), WM_SETICON, ICON_SMALL,
                             reinterpret_cast<LPARAM>(m_favicon.second.get()));
                PostMessageA(hwnd(), WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(m_hicon.get()));
            }

            m_browsers.url->post_json(json(*this));

            break;
        }

        case msg::title_changed:
        {
            if (!m_browsers.url) { break; }

            if (id == m_browsers.first->id())
            {
                m_title.first.assign(m_browsers.first->m_documentTitle);
            }

            else if (id == m_browsers.second->id())
            {
                m_title.second.assign(m_browsers.second->m_documentTitle);
            }

            if (!m_layout.swapped)
            {
                if (!m_browsers.first->m_documentTitle.empty())
                {
                    title(m_browsers.first->m_documentTitle);
                }
            }

            else
            {
                if (!m_browsers.second->m_documentTitle.empty())
                {
                    title(m_browsers.second->m_documentTitle);
                }
            }

            break;
        }

        case msg::focus_changed:
        {
            if (id == m_browsers.first->id()) { m_focus = "first"; }

            else if (id == m_browsers.second->id()) { m_focus = "second"; }

            else if (id == m_browsers.url->id()) { m_focus = "url"; }

            m_browsers.url->post_json(json(*this));

            break;
        }
    }

    return 0;
}

auto Window::on_setting_change(WPARAM wParam, LPARAM lParam) -> int
{
    theme();

    return 0;
}

auto Window::on_size(WPARAM wParam, LPARAM lParam) -> int
{
    client_rect();
    EnumChildWindows(hwnd(), EnumChildProc, reinterpret_cast<intptr_t>(this));
    // Sleep(1);

    return 0;
}

auto Window::on_sys_key_down(WPARAM wParam, LPARAM lParam) -> int
{
    auto key{static_cast<unsigned int>(wParam)};
    Keys keys;

    if ((HIWORD(lParam) & KF_REPEAT) == KF_REPEAT) return 0;

    if (keys.set.contains(key))
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
