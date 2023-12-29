// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#include "App.hxx"

namespace Airglow
{

auto App::run() -> void
{
    App app;
    set_title(app.m_hwnd.get(), "Airglow");
    set_caption_color(app.m_hwnd.get(), false);
    set_border_color(app.m_hwnd.get(), false);
    set_system_backdrop(app.m_hwnd.get(), DWM_SYSTEMBACKDROP_TYPE::DWMSBT_TABBEDWINDOW);
    set_theme(app.m_hwnd.get());
    message_loop();
}

auto App::handle_message(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    switch (uMsg)
    {
    case WM_KEYDOWN: return on_key_down(wParam);
    case WM_SIZE: return on_size();
    }

    return DefWindowProcA(hwnd, uMsg, wParam, lParam);
}

auto CALLBACK App::enum_child_proc(HWND hWnd, LPARAM lParam) -> BOOL
{
    // auto self{InstanceFromEnumChildProc<App>(hWnd, lParam)};

    auto childId{GetWindowLongPtrA(hWnd, GWL_ID)};
    auto rcParent{(LPRECT)lParam};

    if (childId == 1)
        SetWindowPos(hWnd, nullptr, 0, 0, (rcParent->right - rcParent->left) / 2,
                     (rcParent->bottom - rcParent->top), SWP_NOZORDER);

    if (childId == 2)
        SetWindowPos(hWnd, nullptr, (rcParent->right - rcParent->left) / 2, 0,
                     (rcParent->right - rcParent->left) / 2, (rcParent->bottom - rcParent->top),
                     SWP_NOZORDER);

    if (childId == 3) SetWindowPos(hWnd, nullptr, 0, 0, 0, 0, SWP_NOZORDER);

    return 1;
}

auto App::on_key_down(WPARAM wParam) -> int
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

auto App::on_size() -> int
{
    RECT clientRect{0};
    GetClientRect(m_hwnd.get(), &clientRect);
    EnumChildWindows(m_hwnd.get(), enum_child_proc, std::bit_cast<LPARAM>(&clientRect));
    Sleep(1);

    return 0;
}

auto App::save() -> void
{
    auto path{glow::filesystem::get_pgmptr()};
    if (!path.empty())
    {
        auto settingsFile{path / "settings.json"};

        try
        {
            if (!std::filesystem::exists(settingsFile))
            {
                try
                {
                    json j = m_settings;
                    std::ofstream f(settingsFile);
                    f << std::setw(4) << j << "\n";
                    f.close();
                }
                catch (const std::exception& e)
                {
                    std::println("{}", e.what());
                }
            }
            else
            {
                try
                {
                    json j = m_settings;
                    std::ofstream f(settingsFile);
                    f << std::setw(4) << j << "\n";
                    f.close();
                }
                catch (const std::exception& e)
                {
                    std::println("{}", e.what());
                }
            }
        }
        catch (const std::filesystem::filesystem_error& e)
        {
            std::println("{}", e.what());
        }
    }
}

auto App::load() -> void
{
    auto path{glow::filesystem::get_pgmptr()};
    if (!path.empty())
    {
        auto settingsFile{path / "settings.json"};

        try
        {
            if (!std::filesystem::exists(settingsFile))
            {
                try
                {
                    json j = m_settings;
                    std::ofstream f(settingsFile);
                    f << std::setw(4) << j << "\n";
                    f.close();
                }
                catch (const std::exception& e)
                {
                    std::println("{}", e.what());
                }
            }
            else
            {
                try
                {
                    //
                }
                catch (const std::exception& e)
                {
                    std::println("{}", e.what());
                }
            }
        }
        catch (const std::filesystem::filesystem_error& e)
        {
            std::println("{}", e.what());
        }
    }
}

} // namespace Airglow
