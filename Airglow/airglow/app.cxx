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

auto App::handle_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    switch (uMsg)
    {
    case WM_NOTIFY: return on_notify(hWnd, wParam, lParam);
    }

    return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

auto App::on_notify(HWND hWnd, WPARAM wParam, LPARAM lParam) -> int
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

} // namespace airglow
