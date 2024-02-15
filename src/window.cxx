// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#include "window.hxx"

Window::Window(::HWND app, State& state, size_t id)
    : glow::Window("Airglow", id), m_app{app}, m_state{state}
{
    dwm_caption_color(false);
    dwm_system_backdrop(DWMSBT_MAINWINDOW);
    theme();

    std::function<::HRESULT()> firstCallback{[this]()
                                             {
                                                 m_first.browser->focus();
                                                 m_first.browser->move_focus();

                                                 return S_OK;
                                             }};

    std::function<::HRESULT()> urlCallback{[=, this]()
                                           {
                                               m_url.browser->navigate(m_url.browser->url("url"));

                                               return S_OK;
                                           }};

    m_first.browser = std::make_unique<Browser>(m_hwnd.get(), firstCallback);
    m_first.browser->reveal();

    m_second.browser = std::make_unique<Browser>(m_hwnd.get());
    m_second.browser->reveal();

    m_url.browser = std::make_unique<Browser>(m_hwnd.get(), urlCallback);
    m_url.browser->reveal();
}

auto CALLBACK Window::EnumChildProc(::HWND hWnd, ::LPARAM lParam) -> ::BOOL
{
    auto self{reinterpret_cast<Window*>(lParam)};

    if (!self) { return true; }

    auto gwlId{static_cast<size_t>(::GetWindowLongPtrA(hWnd, GWL_ID))};
    auto& rect{self->m_client.rect};
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

    auto& first = self->m_first.browser->m_client;
    auto& second = self->m_second.browser->m_client;
    auto& url = self->m_url.browser->m_client;

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

    auto hdwp{::BeginDeferWindowPos(3)};

    if (gwlId == self->m_first.browser->m_id)
    {
        if (hdwp && self->m_first.browser)
        {
            hdwp = ::DeferWindowPos(
                hdwp, hWnd, nullptr, first.x, first.y, first.width, first.height,
                SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOREDRAW | SWP_NOCOPYBITS);
        }
    }

    if (gwlId == self->m_second.browser->m_id)
    {
        if (hdwp && self->m_second.browser)
        {
            hdwp = ::DeferWindowPos(
                hdwp, hWnd, nullptr, second.x, second.y, second.width, second.height,
                SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOREDRAW | SWP_NOCOPYBITS);
        }
    }

    if (gwlId == self->m_url.browser->m_id)
    {
        if (hdwp && self->m_url.browser)
        {
            hdwp = ::DeferWindowPos(hdwp, hWnd, nullptr, url.x, url.y, url.width, url.height,
                                    SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOOWNERZORDER |
                                        SWP_NOREDRAW | SWP_NOCOPYBITS);
        }
    }

    if (hdwp) { ::EndDeferWindowPos(hdwp); }

    return true;
}

auto Window::wnd_proc(::HWND hWnd, ::UINT uMsg, ::WPARAM wParam, ::LPARAM lParam) -> ::LRESULT
{
    switch (uMsg)
    {
        case WM_ACTIVATE: return on_activate(wParam, lParam);
        case WM_DESTROY: return on_destroy(wParam, lParam);
        case WM_GETMINMAXINFO: return on_get_min_max_info(wParam, lParam);
        case WM_KEYDOWN: return on_key_down(wParam, lParam);
        case WM_NOTIFY: return on_notify(wParam, lParam);
        case WM_SETTINGCHANGE: return on_setting_change(wParam, lParam);
        case WM_SIZE: return on_size(wParam, lParam);
        case WM_SYSKEYDOWN: return on_sys_key_down(wParam, lParam);
    }

    return ::DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

auto Window::on_activate(::WPARAM wParam, ::LPARAM lParam) -> int
{
    notify(m_app, CODE::WINDOW_ACTIVATE);

    return 0;
}

auto Window::on_destroy(::WPARAM wParam, ::LPARAM lParam) -> int
{
    notify(m_app, CODE::WINDOW_CLOSE);

    return 0;
}

auto Window::on_get_min_max_info(::WPARAM wParam, ::LPARAM lParam) -> int
{
    RECT caption{};
    DwmGetWindowAttribute(m_hwnd.get(), DWMWA_CAPTION_BUTTON_BOUNDS, &caption, sizeof(caption));
    auto captionWidth = caption.right - caption.left;

    UINT border{};
    DwmGetWindowAttribute(m_hwnd.get(), DWMWA_VISIBLE_FRAME_BORDER_THICKNESS, &border,
                          sizeof(border));
    auto borderWidth = border * 2;

    RECT extended{};
    DwmGetWindowAttribute(m_hwnd.get(), DWMWA_EXTENDED_FRAME_BOUNDS, &extended, sizeof(extended));
    auto extendedWidth = extended.right - extended.left;

    auto iconWidth = GetSystemMetricsForDpi(SM_CXSMICON, m_dpi);

    RECT window{};
    GetWindowRect(m_hwnd.get(), &window);
    auto windowWidth = window.right - window.left;

    auto xMax = captionWidth + (border * 2) + iconWidth + (windowWidth - extendedWidth);

    auto minmax{reinterpret_cast<::MINMAXINFO*>(lParam)};
    minmax->ptMinTrackSize.x = xMax;
    // minmax->ptMinTrackSize.y = 500;

    return 0;
}

auto Window::on_key_down(::WPARAM wParam, ::LPARAM lParam) -> int
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

            case 0x4C:
            {
                if (::GetKeyState(VK_CONTROL) & 0x8000)
                {
                    m_url.browser->move_focus();
                    m_url.browser->post_json(json{{"focus", m_layout.focus}});
                }

                break;
            }

            case 0x4E:
            {
                if (::GetKeyState(VK_CONTROL) & 0x8000) { notify(m_app, CODE::WINDOW_NEW); }

                break;
            }

            case 0x57:
            {
                if (::GetKeyState(VK_CONTROL) & 0x8000) { close(); }

                break;
            }

            case VK_F1:
            {
                m_layout.swap = !m_layout.swap;

                if (!m_layout.split)
                {
                    if (!m_layout.swap) { m_first.browser->move_focus(); }

                    else if (m_layout.swap) { m_second.browser->move_focus(); }
                }

                notify(m_hwnd.get(), CODE::LAYOUT_CHANGE);

                break;
            }

            case VK_F2:
            {
                m_layout.split = !m_layout.split;

                notify(m_hwnd.get(), CODE::LAYOUT_CHANGE);

                break;
            }

            case VK_F3:
            {
                if (m_layout.split) { m_layout.vertical = !m_layout.vertical; }

                notify(m_hwnd.get(), CODE::LAYOUT_CHANGE);

                break;
            }

            case VK_F4:
            {
                if (GetKeyState(VK_MENU) & 0x8000) { close(); }

                else
                {
                    m_maximize = !m_maximize;
                    toggle_maximize();

                    notify(m_hwnd.get(), CODE::LAYOUT_CHANGE);
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
                m_topmost = !m_topmost;
                toggle_topmost();

                notify(m_hwnd.get(), CODE::LAYOUT_CHANGE);

                break;
            }

            case VK_F11:
            {
                m_fullscreen = !m_fullscreen;
                toggle_fullscreen();

                notify(m_hwnd.get(), CODE::LAYOUT_CHANGE);

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

auto Window::on_notify(::WPARAM wParam, ::LPARAM lParam) -> int
{
    auto notification{reinterpret_cast<glow::Notification*>(lParam)};

    switch (notification->code)
    {
        using enum CODE;

        case WEB_MESSAGE_RECEIVED:
        {
            auto webMessage{json::parse(notification->message)};

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
                resize();
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
            if (notification->id == m_first.browser->m_id)
            {
                m_first.source.assign(m_first.browser->m_source);
            }

            else if (notification->id == m_second.browser->m_id)
            {
                m_second.source.assign(m_second.browser->m_source);
            }

            m_url.browser->post_json(json(*this));

            notify(m_app, SETTINGS_SAVE);

            break;
        }

        case FAVICON_CHANGED:
        {
            std::function<::HRESULT()> firstCallback{
                [this]()
                {
                    m_first.hicon.reset(m_first.browser->m_favicon.second.get());
                    update_caption();

                    return S_OK;
                }};

            std::function<::HRESULT()> secondCallback{
                [this]()
                {
                    m_second.hicon.reset(m_second.browser->m_favicon.second.get());
                    update_caption();

                    return S_OK;
                }};

            m_first.browser->get_favicon(firstCallback);
            m_second.browser->get_favicon(secondCallback);

            if (notification->id == m_first.browser->m_id)
            {
                m_first.favicon.assign(m_first.browser->m_favicon.first);
            }

            else if (notification->id == m_second.browser->m_id)
            {
                m_second.favicon.assign(m_second.browser->m_favicon.first);
            }

            m_url.browser->post_json(json(*this));

            break;
        }

        case TITLE_CHANGED:
        {
            if (notification->id == m_first.browser->m_id)
            {
                m_first.title = m_first.browser->m_title.value_or("Airglow");
            }

            else if (notification->id == m_second.browser->m_id)
            {
                m_second.title = m_second.browser->m_title.value_or("Airglow");
            }

            m_url.browser->post_json(json(*this));

            update_caption();

            break;
        }

        case FOCUS_CHANGED:
        {
            if (notification->id == m_first.browser->m_id) { m_layout.focus = "first"; }

            else if (notification->id == m_second.browser->m_id) { m_layout.focus = "second"; }

            m_url.browser->post_json(json(*this));

            break;
        }

        case LAYOUT_CHANGE:
        {
            resize();
            update_caption();

            m_url.browser->post_json(json(*this));

            break;
        }
    }

    return 0;
}

auto Window::on_setting_change(::WPARAM wParam, ::LPARAM lParam) -> int
{
    theme();
    m_url.browser->post_json(json(*this));

    return 0;
}

auto Window::on_size(::WPARAM wParam, ::LPARAM lParam) -> int
{
    position();
    ::EnumChildWindows(m_hwnd.get(), EnumChildProc, reinterpret_cast<size_t>(this));

    return 0;
}

auto Window::on_sys_key_down(::WPARAM wParam, ::LPARAM lParam) -> int
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
    if (!m_fullscreen)
    {
        if (!m_layout.swap) { set_icon(m_first.hicon.get(), true, false); }

        else { set_icon(m_second.hicon.get(), true, false); }

        set_icon(m_hicon.get(), false, true);
    }

    if (!m_layout.swap) { set_title(m_first.title); }

    else { set_title(m_second.title); }
}
