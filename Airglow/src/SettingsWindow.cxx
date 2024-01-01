// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#include "SettingsWindow.hxx"

auto SettingsWebView::initialized() -> void
{
    // m_settings8->put_AreBrowserAcceleratorKeysEnabled(true);
    m_settings8->put_AreDefaultContextMenusEnabled(false);
    // m_settings8->put_AreDefaultScriptDialogsEnabled(true);
    // m_settings8->put_AreDevToolsEnabled(true);
    // m_settings8->put_AreHostObjectsAllowed(true);
    // m_settings8->put_HiddenPdfToolbarItems(
    //     COREWEBVIEW2_PDF_TOOLBAR_ITEMS::COREWEBVIEW2_PDF_TOOLBAR_ITEMS_NONE);
    // m_settings8->put_IsBuiltInErrorPageEnabled(true);
    // m_settings8->put_IsGeneralAutofillEnabled(true);
    // m_settings8->put_IsPasswordAutosaveEnabled(true);
    // m_settings8->put_IsPinchZoomEnabled(true);
    // m_settings8->put_IsReputationCheckingRequired(true);
    // m_settings8->put_IsScriptEnabled(true);
    // m_settings8->put_IsStatusBarEnabled(true);
    // m_settings8->put_IsSwipeNavigationEnabled(true);
    // m_settings8->put_IsWebMessageEnabled(true);
    m_settings8->put_IsZoomControlEnabled(false);
}

SettingsWindow::SettingsWindow()
{
    set_title(m_hwnd.get(), "Airglow - Settings");
    enable_caption_color(m_hwnd.get(), false);
    set_system_backdrop(m_hwnd.get(), DWM_SYSTEMBACKDROP_TYPE::DWMSBT_TRANSIENTWINDOW);
    use_immersive_dark_mode(m_hwnd.get());

    m_browser = std::make_unique<SettingsWebView>(m_hwnd.get(), +WebViews::settings,
                                                  "https://localhost:8000/settings/index.html");

    m_browser->show_normal();
}

auto CALLBACK SettingsWindow::EnumChildProc(HWND hWnd, LPARAM lParam) -> BOOL
{
    auto gwlId{GetWindowLongPtrA(hWnd, GWL_ID)};

    auto rect{*std::bit_cast<LPRECT>(lParam)};
    auto position{glow::gui::rect_to_position(rect)};

    if (gwlId == +WebViews::settings)
        SetWindowPos(hWnd, nullptr, 0, 0, position.width, position.height, SWP_NOZORDER);

    return TRUE;
}

auto SettingsWindow::handle_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    switch (uMsg)
    {
    case WM_KEYDOWN: return on_key_down(wParam);
    case WM_SIZE: return on_size();
    }

    return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

auto SettingsWindow::on_key_down(WPARAM wParam) -> int
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

auto SettingsWindow::on_size() -> int
{
    RECT clientRect{0};
    GetClientRect(m_hwnd.get(), &clientRect);
    EnumChildWindows(m_hwnd.get(), EnumChildProc, std::bit_cast<LPARAM>(&clientRect));
    Sleep(1);

    return 0;
}
