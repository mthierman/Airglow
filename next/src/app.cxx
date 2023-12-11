// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#include "app.hxx"

namespace airglow
{

//==============================================================================
auto App::run() -> void
{
    glow::gui::msg_loop();
}

//==============================================================================
auto App::handle_message(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    switch (uMsg)
    {
    case WM_NOTIFY: return on_notify();
    case WM_WINDOWPOSCHANGED: return on_window_pos_changed();
    }

    return ::DefWindowProc(hwnd, uMsg, wParam, lParam);
}

//==============================================================================
auto CALLBACK App::enum_child_proc(HWND hwnd, LPARAM lParam) -> BOOL
{
    auto childId{::GetWindowLongPtr(hwnd, GWL_ID)};

    auto rcParent{(LPRECT)lParam};

    if (childId == 1)
        ::SetWindowPos(hwnd, nullptr, 0, 0, (rcParent->right - rcParent->left) / 2,
                       (rcParent->bottom - rcParent->top), SWP_NOZORDER);

    if (childId == 2)
        ::SetWindowPos(hwnd, nullptr, (rcParent->right - rcParent->left) / 2, 0,
                       (rcParent->right - rcParent->left) / 2, (rcParent->bottom - rcParent->top),
                       SWP_NOZORDER);

    return 1;
}

//==============================================================================
auto App::on_notify() -> int
{
    ::RECT clientRect{0};
    ::GetClientRect(m_hwnd.get(), &clientRect);
    ::EnumChildWindows(m_hwnd.get(), enum_child_proc, reinterpret_cast<LPARAM>(&clientRect));

    return 0;
}

//==============================================================================
auto App::on_window_pos_changed() -> int
{
    ::RECT clientRect{0};
    ::GetClientRect(m_hwnd.get(), &clientRect);
    ::EnumChildWindows(m_hwnd.get(), enum_child_proc, reinterpret_cast<LPARAM>(&clientRect));

    return 0;
}
} // namespace airglow
