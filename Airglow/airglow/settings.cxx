// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#include <airglow/settings.hxx>

namespace airglow::settings
{

void to_json(nlohmann::json& j, const Settings& settings)
{
    j = nlohmann::json{{"name", settings.m_name},
                       {"version", settings.m_version},
                       {"width", settings.m_width},
                       {"height", settings.m_height}};
}

void from_json(const nlohmann::json& j, Settings& settings)
{
    j.at("name").get_to(settings.m_name);
    j.at("version").get_to(settings.m_version);
    j.at("width").get_to(settings.m_width);
    j.at("height").get_to(settings.m_height);
}

auto save_settings(Settings settings) -> void
{
    auto path{glow::filesystem::portable()};
    if (!path.empty())
    {
        auto settingsFile{path / "settings.json"};

        try
        {
            if (!std::filesystem::exists(settingsFile))
            {
                try
                {
                    nlohmann::json j = settings;
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
                    nlohmann::json j = settings;
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

auto load_settings(Settings settings) -> void
{
    auto path{glow::filesystem::portable()};
    if (!path.empty())
    {
        auto settingsFile{path / "settings.json"};

        try
        {
            if (!std::filesystem::exists(settingsFile))
            {
                try
                {
                    nlohmann::json j = settings;
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

Window::Window(HWND app, std::string className) : glow::window::Window(className) { m_app = app; }

auto Window::operator()(bool show) -> void
{
    glow::window::Window::operator()(show);
    dwm_caption_color(false);
    dwm_dark_mode(true);
    dwm_system_backdrop(DWM_SYSTEMBACKDROP_TYPE::DWMSBT_MAINWINDOW);

    m_browser = std::make_unique<airglow::Browser>(+Browsers::browser1, m_hwnd.get());
    (*m_browser)();
}

auto Window::handle_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    switch (uMsg)
    {
    case WM_CREATE: return on_create(hWnd, wParam, lParam);
    case WM_CLOSE: return on_close(hWnd, wParam, lParam);
    case WM_SIZE: return on_size(hWnd, wParam, lParam);
    }

    return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

auto Window::on_create(HWND hWnd, WPARAM wParam, LPARAM lParam) -> int
{
    NMHDR nmhdr;
    nmhdr.code = CUSTOM_CREATE_WINDOW;
    nmhdr.hwndFrom = m_hwnd.get();
    nmhdr.idFrom = m_id;
    SendMessageA(m_app, WM_NOTIFY, nmhdr.idFrom, std::bit_cast<LPARAM>(&nmhdr));

    return 0;
}

auto Window::on_close(HWND hWnd, WPARAM wParam, LPARAM lParam) -> int
{
    NMHDR nmhdr;
    nmhdr.code = CUSTOM_CLOSE_WINDOW;
    nmhdr.hwndFrom = m_hwnd.get();
    nmhdr.idFrom = m_id;
    SendMessageA(m_app, WM_NOTIFY, nmhdr.idFrom, std::bit_cast<LPARAM>(&nmhdr));

    m_hwnd.reset();

    return 0;
}

auto Window::on_size(HWND hWnd, WPARAM wParam, LPARAM lParam) -> int
{
    RECT rect{0};
    GetClientRect(hWnd, &rect);
    EnumChildWindows(hWnd, EnumChildProc, std::bit_cast<LPARAM>(&rect));
    Sleep(1);

    return 0;
}

auto CALLBACK Window::EnumChildProc(HWND hWnd, LPARAM lParam) -> BOOL
{
    auto gwlId{static_cast<int64_t>(GetWindowLongPtrA(hWnd, GWL_ID))};
    auto rect{*std::bit_cast<RECT*>(lParam)};

    auto defer{true};

    if (defer)
    {
        auto hdwp{BeginDeferWindowPos(4)};

        if (gwlId == +Browsers::settings)
            if (hdwp)
                hdwp = DeferWindowPos(hdwp, hWnd, nullptr, 0, 0, rect.right - rect.left,
                                      rect.bottom - rect.top,
                                      SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOOWNERZORDER |
                                          SWP_NOREDRAW | SWP_NOCOPYBITS);

        if (hdwp) EndDeferWindowPos(hdwp);
    }

    else
    {
        if (gwlId == +Browsers::settings)
            SetWindowPos(hWnd, nullptr, 0, 0, rect.right - rect.left, rect.bottom - rect.top,
                         SWP_NOZORDER);
    }

    return TRUE;
}

} // namespace airglow::settings
