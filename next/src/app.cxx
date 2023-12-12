// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#include "app.hxx"

//==============================================================================
namespace airglow
{

//==============================================================================
auto App::run() -> void { glow::gui::msg_loop(); }

//==============================================================================
auto App::handle_message(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    switch (uMsg)
    {
    case WM_KEYDOWN: return on_key_down(wParam);
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
auto App::on_key_down(WPARAM wParam) -> int
{
    switch (wParam)
    {
    case VK_PAUSE: OutputDebugString("PAUSE"); break;
    case 0x4C:
        if (::GetKeyState(VK_CONTROL) & 0x8000) OutputDebugString("L");
        break;
    case 0x57:
        if (::GetKeyState(VK_CONTROL) & 0x8000)
        {
            OutputDebugString("W");
            PostMessage(m_hwnd.get(), WM_CLOSE, 0, 0);
        }
        break;
    case VK_F1: OutputDebugString("F1"); break;
    case VK_F2: OutputDebugString("F2"); break;
    case VK_F3: OutputDebugString("F3"); break;
    case VK_F4: OutputDebugString("F4"); break;
    case VK_F6: OutputDebugString("F6"); break;
    case VK_F8: OutputDebugString("F8"); break;
    case VK_F11: OutputDebugString("F11"); break;
    }

    return 0;
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

    // std::this_thread::sleep_for(std::chrono::milliseconds(1));
    // std::this_thread::sleep_for(std::chrono::microseconds(1));
    // std::this_thread::sleep_for(std::chrono::nanoseconds(1));
    ::Sleep(1);

    return 0;
}

//==============================================================================
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

//==============================================================================
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

//==============================================================================
} // namespace airglow
