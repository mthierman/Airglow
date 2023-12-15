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

auto run() -> void
{
    App app(PROJECT_NAME);

    glow::gui::message_loop();
}

auto App::handle_message(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    switch (uMsg)
    {
    case WM_KEYDOWN: return on_key_down(wParam);
    case WM_NOTIFY: return on_notify();
    case WM_SIZE: return on_size();
    }

    return DefWindowProcA(hwnd, uMsg, wParam, lParam);
}

auto CALLBACK App::enum_child_proc(HWND hwnd, LPARAM lParam) -> BOOL
{
    auto childId{GetWindowLongPtrA(hwnd, GWL_ID)};

    auto rcParent{(LPRECT)lParam};

    if (childId == 1)
        SetWindowPos(hwnd, nullptr, 0, 0, (rcParent->right - rcParent->left) / 2,
                     (rcParent->bottom - rcParent->top), SWP_NOZORDER);

    if (childId == 2)
        SetWindowPos(hwnd, nullptr, (rcParent->right - rcParent->left) / 2, 0,
                     (rcParent->right - rcParent->left) / 2, (rcParent->bottom - rcParent->top),
                     SWP_NOZORDER);

    // BLACK BORDERS:
    // if (childId == 1)
    //     SetWindowPos(hwnd, nullptr, 1, 1, ((rcParent->right - rcParent->left) / 2) - 2,
    //                  (rcParent->bottom - rcParent->top) - 2, SWP_NOZORDER);

    // if (childId == 2)
    //     SetWindowPos(hwnd, nullptr, ((rcParent->right - rcParent->left) / 2) + 1, 1,
    //                  ((rcParent->right - rcParent->left) / 2) - 2,
    //                  (rcParent->bottom - rcParent->top) - 2, SWP_NOZORDER);

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

auto App::on_notify() -> int
{
    on_size();

    return 0;
}

auto App::on_size() -> int
{
    RECT clientRect{0};
    GetClientRect(m_hwnd.get(), &clientRect);
    EnumChildWindows(m_hwnd.get(), enum_child_proc, reinterpret_cast<LPARAM>(&clientRect));
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

} // namespace airglow
