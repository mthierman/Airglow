// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#include "window.hxx"

MainWindow::MainWindow(HWND app) : BaseWindow()
{
    m_app = app;
    notify(m_app, msg::window_create);

    m_browser = std::make_unique<Browser>(hwnd());
    m_browser->reveal();
}

auto MainWindow::default_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    switch (uMsg)
    {
    case WM_CLOSE: return on_close(wParam, lParam);
    case WM_SIZE: return on_size(wParam, lParam);
    }

    return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

auto MainWindow::on_close(WPARAM wParam, LPARAM lParam) -> int
{
    notify(m_app, msg::window_close);

    return close();
}

auto MainWindow::on_size(WPARAM wParam, LPARAM lParam) -> int
{
    WindowDimensions dimensions;
    GetClientRect(hwnd(), &dimensions.rect);
    dimensions.scale = scale();
    dimensions.hwnd = hwnd();
    EnumChildWindows(hwnd(), EnumChildProc, std::bit_cast<LPARAM>(&dimensions));
    Sleep(1);

    return 0;
}

auto CALLBACK MainWindow::EnumChildProc(HWND hWnd, LPARAM lParam) -> BOOL
{
    auto dimensions{*std::bit_cast<WindowDimensions*>(lParam)};
    auto self{glow::gui::instance_from_window_long_ptr<MainWindow>(dimensions.hwnd)};

    if (self)
    {
        auto gwlId{static_cast<int64_t>(GetWindowLongPtrA(hWnd, GWL_ID))};

        auto r = &dimensions.rect;
        auto width{r->right - r->left};
        auto height{r->bottom - r->top};

        auto hdwp{BeginDeferWindowPos(4)};

        if (gwlId == self->m_browser->id())
            if (hdwp)
                hdwp = DeferWindowPos(hdwp, hWnd, nullptr, 0, 0, width, height,
                                      SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOOWNERZORDER |
                                          SWP_NOREDRAW | SWP_NOCOPYBITS);

        if (hdwp) EndDeferWindowPos(hdwp);
    }

    return TRUE;
}
