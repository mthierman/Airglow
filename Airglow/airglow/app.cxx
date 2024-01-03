// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#include <airglow/app.hxx>

namespace airglow
{

auto App::create_window() -> void
{
    CreateWindowExA(0, m_wcex.lpszClassName, m_wcex.lpszClassName, 0, CW_USEDEFAULT, CW_USEDEFAULT,
                    CW_USEDEFAULT, CW_USEDEFAULT, HWND_MESSAGE, nullptr, GetModuleHandleA(nullptr),
                    this);
}

auto CALLBACK App::EnumChildProc(HWND hWnd, LPARAM lParam) -> BOOL
{
    auto gwlId{static_cast<int64_t>(GetWindowLongPtrA(hWnd, GWL_ID))};
    auto rect{*std::bit_cast<LPRECT>(lParam)};

    return TRUE;
}

auto App::handle_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    switch (uMsg)
    {
    case WM_NOTIFY: return on_notify(lParam);
    }

    return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

auto App::on_notify(LPARAM lParam) -> int
{
    auto nmhdr{*std::bit_cast<LPNMHDR>(lParam)};

    switch (nmhdr.code)
    {
    case CUSTOM_CREATE_WINDOW: m_windows.insert(nmhdr.hwndFrom); break;
    case CUSTOM_CLOSE_WINDOW: m_windows.erase(nmhdr.hwndFrom); break;
    }

    if (m_windows.empty())
    {
        m_hwnd.reset();
        PostQuitMessage(0);
    }

    return 0;
}

Window::Window(HWND app, std::string name) : m_app{app} {}

auto Window::operator()(bool show) -> void
{
    glow::window::Window::operator()(show);
    dwm_caption_color(false);
    dwm_dark_mode(true);
    dwm_system_backdrop(DWM_SYSTEMBACKDROP_TYPE::DWMSBT_MAINWINDOW);
}

auto Window::handle_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    switch (uMsg)
    {
    case WM_CREATE: return on_create();
    case WM_CLOSE: return on_close();
    }

    return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

auto Window::on_create() -> int
{
    NMHDR nmhdr;
    nmhdr.code = CUSTOM_CREATE_WINDOW;
    nmhdr.hwndFrom = m_hwnd.get();
    nmhdr.idFrom = m_id;
    SendMessageA(m_app, WM_NOTIFY, nmhdr.idFrom, std::bit_cast<LPARAM>(&nmhdr));

    return 0;
}

auto Window::on_close() -> int
{
    NMHDR nmhdr;
    nmhdr.code = CUSTOM_CLOSE_WINDOW;
    nmhdr.hwndFrom = m_hwnd.get();
    nmhdr.idFrom = m_id;
    SendMessageA(m_app, WM_NOTIFY, nmhdr.idFrom, std::bit_cast<LPARAM>(&nmhdr));

    m_hwnd.reset();

    return 0;
}

} // namespace airglow
