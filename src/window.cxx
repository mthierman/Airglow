// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#include "window.hxx"

Window::Window(HWND app, std::map<std::string, std::string> current) : BaseWindow("Airglow")
{
    m_app = app;
    m_current = current;

    notify(m_app, msg::window_create);

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
        auto gwlId{static_cast<intptr_t>(GetWindowLongPtrA(hWnd, GWL_ID))};

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
    notify(m_app, msg::window_close);

    return close();
}

auto Window::on_key_down(WPARAM wParam, LPARAM lParam) -> int
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
            notify(m_app, msg::toggle_settings);

            break;
        }

        case 0x4C:
        {
            if (GetKeyState(VK_CONTROL) & 0x8000)
            {
                if (m_browsers.url) { m_browsers.url->focus(); }
            }

            break;
        }

        case 0x57:
        {
            if (GetKeyState(VK_CONTROL) & 0x8000) { SendMessageA(hwnd(), WM_CLOSE, 0, 0); }

            break;
        }

        case VK_F1:
        {
            m_layout.split = !m_layout.split;

            break;
        }

        case VK_F2:
        {
            m_layout.swapped = !m_layout.swapped;

            break;
        }

        case VK_F3:
        {
            if (m_layout.split) { m_layout.horizontal = !m_layout.horizontal; }

            break;
        }

        case VK_F4:
        {
            if (GetKeyState(VK_MENU) & 0x8000) { SendMessageA(hwnd(), WM_CLOSE, 0, 0); }

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

    if (m_browsers.url)
    {
        m_browsers.url->post_json(nlohmann::json{{"layout", nlohmann::json(m_layout)}});
    }

    PostMessageA(hwnd(), WM_SIZE, 0, 0);

    return 0;
}

auto Window::on_notify(WPARAM wParam, LPARAM lParam) -> int
{
    auto notification{reinterpret_cast<glow::gui::Notification*>(lParam)};

    switch (notification->nmhdr.code)
    {
    case msg::url_create:
    {
        if (!m_initialized)
        {
            m_initialized = true;
            if (m_browsers.first) { m_browsers.first->navigate(m_current["first"]); }
            if (m_browsers.second) { m_browsers.second->navigate(m_current["second"]); }
        }

        if (m_browsers.url)
        {
            m_browsers.url->post_json(nlohmann::json{{"layout", nlohmann::json(m_layout)}});
            m_browsers.url->post_json(nlohmann::json(m_systemColors));
        }

        break;
    }

    case msg::web_message_received:
    {
        auto json{nlohmann::json::parse(notification->message)};

        if (json.contains("height"))
        {
            m_layout.bar = json["height"].get<int>();
            PostMessageA(hwnd(), WM_SIZE, 0, 0);
        }

        else if (json.contains("first"))
        {
            if (m_browsers.first) { m_browsers.first->navigate(json["first"].get<std::string>()); }
        }

        else if (json.contains("second"))
        {
            if (m_browsers.second)
            {
                m_browsers.second->navigate(json["second"].get<std::string>());
            }
        }

        break;
    }

    case msg::source_changed:
    {
        auto json{nlohmann::json::parse(notification->message)};

        if (json.contains("first"))
        {
            if (m_browsers.url) { m_browsers.url->post_json(json); }
            notify(m_app, msg::source_changed, notification->message);
        }

        else if (json.contains("second"))
        {
            if (m_browsers.url) { m_browsers.url->post_json(json); }
            notify(m_app, msg::source_changed, notification->message);
        }

        break;
    }
    }

    return 0;
}

auto Window::on_setting_change(WPARAM wParam, LPARAM lParam) -> int
{
    theme();

    m_systemColors.update();
    if (m_browsers.url) { m_browsers.url->post_json(nlohmann::json(m_systemColors)); }

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
