// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#include "app.hxx"

auto App::operator()() -> int
{
    try
    {
        SetEnvironmentVariableA("WEBVIEW2_DEFAULT_BACKGROUND_COLOR", "0");
        SetEnvironmentVariableA("WEBVIEW2_ADDITIONAL_BROWSER_ARGUMENTS",
                                "--allow-file-access-from-files");

        m_settingsFile = json();
        if (!std::filesystem::exists(m_settingsFile)) { save(); }
        else { load(); }

        m_urls = args();

        m_windowMain = std::make_unique<Window>(hwnd(), m_urls);
        m_windowMain->reveal();

        m_windowSettings = std::make_unique<Settings>(hwnd());
    }
    catch (const std::exception& e)
    {
        glow::console::message_box(e.what());
    }

    return glow::gui::message_loop();
}

auto App::args() -> std::pair<std::string, std::string>
{
    std::pair<std::string, std::string> url;

    auto argv{glow::console::argv()};

    if (argv.size() == 2)
    {
        url.first = argv.at(1);
        url.second = argv.at(1);
    }

    else if (argv.size() > 2)
    {
        url.first = argv.at(1);
        url.second = argv.at(2);
    }

    else
    {
        url.first = m_settings.firstHome;
        url.second = m_settings.secondHome;
    }

    return url;
}

auto App::wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    switch (uMsg)
    {
    case WM_NOTIFY: return on_notify(wParam, lParam);
    }

    return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

auto App::on_notify(WPARAM wParam, LPARAM lParam) -> int
{
    auto notification{reinterpret_cast<glow::gui::Notification*>(lParam)};

    switch (notification->nmhdr.code)
    {
    case msg::window_create:
    {
        m_windows.insert(notification->nmhdr.idFrom);

        break;
    }

    case msg::window_close:
    {
        m_windows.erase(notification->nmhdr.idFrom);

        break;
    }

    case msg::toggle_settings:
    {
        m_windowSettings->m_visible ? m_windowSettings->hide() : m_windowSettings->show();

        break;
    }

    case msg::save_settings:
    {
        save();

        break;
    }
    }

    if (m_windows.empty()) { return close(); }

    else return 0;
}

auto App::data() -> std::filesystem::path
{
    auto path{glow::filesystem::known_folder() / "Airglow"};

    if (!std::filesystem::exists(path)) std::filesystem::create_directory(path);

    return path;
}

auto App::json() -> std::filesystem::path
{
    auto path{data() / "Airglow.json"};

    return path;
}

auto App::save() -> void
{
    try
    {
        std::ofstream f(m_settingsFile);
        f << std::setw(4) << nlohmann::json(m_settings) << std::endl;
        f.close();
    }
    catch (const std::exception& e)
    {
        return;
    }
}

auto App::load() -> void
{
    try
    {
        std::ifstream f(m_settingsFile);
        m_settings = nlohmann::json::parse(f, nullptr, false, true);
        f.close();
    }
    catch (const std::exception& e)
    {
        return;
    }
}
