// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#include "window.hxx"

Window::Window(HWND app, std::pair<std::string, std::string> urls) : BaseWindow("Airglow")
{
    m_app = app;
    m_urls = urls;

    notify(m_app, msg::window_create);

    dwm_caption_color(false);
    dwm_dark_mode(true);
    dwm_system_backdrop(DWM_SYSTEMBACKDROP_TYPE::DWMSBT_MAINWINDOW);

    m_browsers.first = std::make_unique<MainBrowser>(hwnd());
    m_browsers.first->reveal();

    m_browsers.second = std::make_unique<SideBrowser>(hwnd());
    m_browsers.second->reveal();

    m_browsers.url = std::make_unique<URLBrowser>(hwnd());
    m_browsers.url->reveal();
}

auto Window::default_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    switch (uMsg)
    {
    case WM_CLOSE: return on_close(hWnd, wParam, lParam);
    case WM_KEYDOWN: return on_key_down(hWnd, wParam, lParam);
    case WM_NOTIFY: return on_notify(hWnd, wParam, lParam);
    case WM_SIZE: return on_size(hWnd, wParam, lParam);
    }

    return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

auto Window::on_close(HWND hWnd, WPARAM wParam, LPARAM lParam) -> int
{
    notify(m_app, msg::window_close);

    return close();
}

auto Window::on_key_down(HWND hWnd, WPARAM wParam, LPARAM lParam) -> int
{
    switch (wParam)
    {
    case key::pause:
    {
        OutputDebugStringA("PAUSE");
        break;
    }

    case key::l:
    {
        if (GetKeyState(VK_CONTROL) & 0x8000) OutputDebugStringA("L");
        break;
    }

    case key::w:
    {
        if (GetKeyState(VK_CONTROL) & 0x8000) SendMessageA(hwnd(), WM_CLOSE, 0, 0);
        break;
    }

    case key::f1:
    {
        OutputDebugStringA("F1");
        break;
    }

    case key::f2:
    {
        OutputDebugStringA("F2");
        break;
    }

    case key::f3:
    {
        OutputDebugStringA("F3");
        break;
    }

    case key::f4:
    {
        if (GetKeyState(VK_MENU) & 0x8000) SendMessageA(hwnd(), WM_CLOSE, 0, 0);
        OutputDebugStringA("F4");
        break;
    }

    case key::f6:
    {
        OutputDebugStringA("F6");
        break;
    }

    case key::f8:
    {
        OutputDebugStringA("F8");
        break;
    }

    case key::f11:
    {
        OutputDebugStringA("F11");
        break;
    }

    case key::f12:
    {
        OutputDebugStringA("F12");
        break;
    }
    }

    return 0;
}

auto Window::on_notify(HWND hWnd, WPARAM wParam, LPARAM lParam) -> int
{
    auto notification{reinterpret_cast<glow::gui::Notification*>(lParam)};

    switch (notification->nmhdr.code)
    {
    case msg::url_created:
    {
        if (m_browsers.first) m_browsers.first->navigate(m_urls.first);
        if (m_browsers.second) m_browsers.second->navigate(m_urls.second);
        break;
    }

    case msg::url_height:
    {
        m_bar = std::stoi(notification->message);
        PostMessageA(hwnd(), WM_SIZE, 0, 0);
        break;
    }

    case msg::post_mainurl:
    {
        if (m_browsers.first) m_browsers.first->navigate(notification->message);
        break;
    }

    case msg::post_sideurl:
    {
        if (m_browsers.second) m_browsers.second->navigate(notification->message);
        break;
    }

    case msg::receive_mainurl:
    {
        nlohmann::json message{{"mainUrl", notification->message}};
        if (m_browsers.url) m_browsers.url->post_json(message);
        break;
    }

    case msg::receive_sideurl:
    {
        nlohmann::json message{{"sideUrl", notification->message}};
        if (m_browsers.url) m_browsers.url->post_json(message);
        break;
    }
    }

    return 0;
}

auto Window::on_size(HWND hWnd, WPARAM wParam, LPARAM lParam) -> int
{
    client_rect();
    EnumChildWindows(hwnd(), EnumChildProc, reinterpret_cast<intptr_t>(this));
    Sleep(1);

    return 0;
}

auto CALLBACK Window::EnumChildProc(HWND hWnd, LPARAM lParam) -> BOOL
{
    auto self{reinterpret_cast<Window*>(lParam)};

    if (self)
    {
        auto gwlId{static_cast<intptr_t>(GetWindowLongPtrA(hWnd, GWL_ID))};

        auto r{&self->m_clientRect};
        auto width{r->right - r->left};
        auto height{r->bottom - r->top};
        auto border{static_cast<int>(self->s_border * self->m_scale)};
        auto bar{static_cast<int>(self->m_bar * self->m_scale)};

        auto hdwp{BeginDeferWindowPos(3)};

        if (gwlId == self->m_browsers.first->id())
            if (hdwp && self->m_browsers.first)
                hdwp = DeferWindowPos(hdwp, hWnd, nullptr, 0, 0, (width / 2) - border, height - bar,
                                      SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOOWNERZORDER |
                                          SWP_NOREDRAW | SWP_NOCOPYBITS);

        if (gwlId == self->m_browsers.second->id())
            if (hdwp && self->m_browsers.second)
                hdwp = DeferWindowPos(hdwp, hWnd, nullptr, (width / 2) + border, 0,
                                      (width / 2) - border, height - bar,
                                      SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOOWNERZORDER |
                                          SWP_NOREDRAW | SWP_NOCOPYBITS);

        if (gwlId == self->m_browsers.url->id())
            if (hdwp && self->m_browsers.url)
                hdwp = DeferWindowPos(hdwp, hWnd, nullptr, 0, r->bottom - bar, width, bar,
                                      SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOOWNERZORDER |
                                          SWP_NOREDRAW | SWP_NOCOPYBITS);

        if (hdwp) EndDeferWindowPos(hdwp);
    }

    return TRUE;
}
