// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#include <airglow/frame.hxx>

Frame::Frame()
{
    WNDCLASSEXA wcex{sizeof(WNDCLASSEXA)};
    auto classInfo{GetClassInfoExA(GetModuleHandleA(nullptr), MAKEINTATOM(Frame::m_atom), &wcex)};

    if (!classInfo)
    {
        OutputDebugStringA("Registering Frame class...");

        wcex.lpszClassName = "Frame";
        wcex.lpszMenuName = 0;
        wcex.lpfnWndProc = Frame::WndProc;
        wcex.style = 0;
        wcex.cbClsExtra = 0;
        wcex.cbWndExtra = sizeof(void*);
        wcex.hInstance = GetModuleHandleA(nullptr);
        wcex.hbrBackground = m_hbrBackground.get();
        wcex.hCursor = m_hCursor.get();
        wcex.hIcon = m_appIcon.get() ? m_appIcon.get() : m_hIcon.get();
        wcex.hIconSm = m_appIcon.get() ? m_appIcon.get() : m_hIcon.get();

        Frame::m_atom = RegisterClassExA(&wcex);
    }

    // https://stackoverflow.com/questions/31313624/click-through-transparent-window-no-dragging-allowed-c
    // https://stackoverflow.com/questions/64041296/how-to-programatically-hinder-windows-from-playing-default-beep-sound-when-use
    CreateWindowExA(WS_EX_TOOLWINDOW | WS_EX_TRANSPARENT | WS_EX_COMPOSITED | WS_EX_LAYERED |
                        WS_EX_TOPMOST,
                    MAKEINTATOM(Frame::m_atom), "Frame", WS_POPUP, 0, 0, 0, 0, nullptr, nullptr,
                    GetModuleHandleA(nullptr), this);

    show_normal();

    // SetLayeredWindowAttributes(m_hwnd.get(), RGB(0, 0, 0), 200, LWA_ALPHA);
    // glow::gui::set_rounded_corners(m_hwnd.get(), true);
    // glow::gui::enable_border_color(m_hwnd.get(), true);
    // glow::gui::use_immersive_dark_mode(m_hwnd.get());
    // glow::gui::set_border_color(m_hwnd.get(), RGB(0, 0, 0));
    // glow::gui::set_system_backdrop(m_hwnd.get(), DWM_SYSTEMBACKDROP_TYPE::DWMSBT_MAINWINDOW);
    EnableWindow(m_hwnd.get(), FALSE);
}

Frame::~Frame() {}

// auto Frame::show_normal() -> void { glow::gui::show_normal(m_hwnd.get()); }

// auto Frame::show() -> void { glow::gui::show(m_hwnd.get()); }

// auto Frame::hide() -> void { glow::gui::hide(m_hwnd.get()); }

auto CALLBACK Frame::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    auto self{window::<Frame>(hWnd, uMsg, lParam)};

    if (self)
    {
        switch (uMsg)
        {
        case WM_CLOSE: return self->on_close(hWnd, uMsg, wParam, lParam);
        case WM_DESTROY: return self->on_destroy(hWnd, uMsg, wParam, lParam);
        }

        return self->handle_wnd_proc(hWnd, uMsg, wParam, lParam);
    }

    else return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

auto Frame::handle_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

auto Frame::on_close(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> int
{
    m_hwnd.reset();

    return 0;
}

auto Frame::on_destroy(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> int { return 0; }
