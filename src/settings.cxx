// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#include "settings.hxx"

Settings::Settings(HWND app) : BaseWindow()
{
    m_app = app;
    notify(m_app, msg::window_create);

    dwm_caption_color(false);
    dwm_dark_mode(true);
    dwm_system_backdrop(DWM_SYSTEMBACKDROP_TYPE::DWMSBT_MAINWINDOW);

    m_main = std::make_unique<Browser>(hwnd());
    m_main->reveal();
}

auto Settings::default_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    switch (uMsg)
    {
    case WM_CLOSE: return on_close(wParam, lParam);
    case WM_KEYDOWN: return on_key_down(wParam, lParam);
    // case WM_NOTIFY: return on_notify(wParam, lParam);
    case WM_SIZE: return on_size(wParam, lParam);
    }

    return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

auto Settings::on_close(WPARAM wParam, LPARAM lParam) -> int
{
    notify(m_app, msg::window_close);

    return close();
}

auto Settings::on_size(WPARAM wParam, LPARAM lParam) -> int
{
    WindowDimensions dimensions;
    GetClientRect(hwnd(), &dimensions.rect);
    dimensions.scale = scale();
    dimensions.hwnd = hwnd();
    EnumChildWindows(hwnd(), EnumChildProc, std::bit_cast<LPARAM>(&dimensions));
    Sleep(1);

    return 0;
}

auto Settings::on_key_down(WPARAM wParam, LPARAM lParam) -> int
{
    switch (wParam)
    {
    case VK_PAUSE: OutputDebugStringA("PAUSE"); break;
    case 0x4C:
        if (GetKeyState(VK_CONTROL) & 0x8000) OutputDebugStringA("L");
        break;
    case 0x57:
        if (GetKeyState(VK_CONTROL) & 0x8000)
        {
            OutputDebugStringA("W");
            PostMessageA(m_hwnd.get(), WM_CLOSE, 0, 0);
        }
        break;
    case VK_F1: OutputDebugStringA("F1"); break;
    case VK_F2: OutputDebugStringA("F2"); break;
    case VK_F3: OutputDebugStringA("F3"); break;
    case VK_F4: OutputDebugStringA("F4"); break;
    case VK_F6: OutputDebugStringA("F6"); break;
    case VK_F8: OutputDebugStringA("F8"); break;
    case VK_F11: OutputDebugStringA("F11"); break;
    }

    return 0;
}

auto CALLBACK Settings::EnumChildProc(HWND hWnd, LPARAM lParam) -> BOOL
{
    auto dimensions{*std::bit_cast<WindowDimensions*>(lParam)};
    auto self{glow::gui::instance_from_window_long_ptr<Settings>(dimensions.hwnd)};

    if (self)
    {
        auto gwlId{static_cast<int64_t>(GetWindowLongPtrA(hWnd, GWL_ID))};

        auto r = &dimensions.rect;
        auto width{r->right - r->left};
        auto height{r->bottom - r->top};

        auto hdwp{BeginDeferWindowPos(4)};

        if (gwlId == self->m_main->id())
            if (hdwp)
                hdwp = DeferWindowPos(hdwp, hWnd, nullptr, 0, 0, width, height,
                                      SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOOWNERZORDER |
                                          SWP_NOREDRAW | SWP_NOCOPYBITS);

        if (hdwp) EndDeferWindowPos(hdwp);
    }

    return TRUE;
}

auto Settings::url_path() -> std::string
{
#if _DEBUG
    std::string path{"https://localhost:8000/settings/index.html"};
#else
    auto path{"file:///" + filesystem::known_folder().string() +
              "\\Airglow\\gui\\settings\\index.html"};
#endif

    return path;
}
