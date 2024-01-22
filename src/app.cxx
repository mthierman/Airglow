// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#include "app.hxx"

App::App(int argc, char* argv[]) : m_settingsFile{json()}, m_argv{glow::console::argv(argc, argv)}
{
    SetEnvironmentVariableA("WEBVIEW2_DEFAULT_BACKGROUND_COLOR", "0");
    SetEnvironmentVariableA("WEBVIEW2_ADDITIONAL_BROWSER_ARGUMENTS",
                            "--allow-file-access-from-files");
}

auto App::operator()() -> int
{
    try
    {
        if (!std::filesystem::exists(m_settingsFile)) { save(); }
        else { load(); }

        if (m_argv.size() == 2)
        {
            m_url.current["first"] = m_argv.at(1);
            m_url.current["second"] = m_url.home["second"];
        }

        else if (m_argv.size() > 2)
        {
            m_url.current["first"] = m_argv.at(1);
            m_url.current["second"] = m_argv.at(2);
        }

        else
        {
            m_url.current["first"] = m_url.home["first"];
            m_url.current["second"] = m_url.home["second"];
        }

        m_windowMain = std::make_unique<Window>(hwnd(), m_url);
        m_windowMain->reveal();

        m_windowSettings = std::make_unique<Settings>(hwnd(), m_url);
        SetWindowPos(m_windowSettings->hwnd(), nullptr, 0, 0, 300, 400, SWP_NOMOVE);
        m_windowSettings->reveal();
        m_windowSettings->m_visible = true;
    }
    catch (const std::exception& e)
    {
        glow::console::message_box(e.what());
    }

    return glow::gui::message_loop();
}

auto App::data() -> std::filesystem::path
{
    auto path{glow::filesystem::known_folder() / "Airglow"};

    if (!std::filesystem::exists(path)) { std::filesystem::create_directory(path); }

    return path;
}

auto App::json() -> std::filesystem::path
{
    auto path{glow::filesystem::path_portable() / "Airglow.json"};

    return path;
}

auto App::save() -> void
{
    try
    {
        std::ofstream f(m_settingsFile);
        f << std::setw(4) << nlohmann::json(m_url) << std::endl;
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
        m_url = nlohmann::json::parse(f, nullptr, false, true);
        f.close();
    }
    catch (const std::exception& e)
    {
        return;
    }
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

    case msg::source_changed:
    {
        auto json{nlohmann::json::parse(notification->message)};

        if (json.contains("first")) { m_url.current["first"] = json["first"].get<std::string>(); }

        else if (json.contains("second"))
        {
            m_url.current["second"] = json["second"].get<std::string>();
        }

        save();

        break;
    }

    case msg::home_changed:
    {
        auto json{nlohmann::json::parse(notification->message)};

        if (json.contains("first")) { m_url.home["first"] = json["first"].get<std::string>(); }

        else if (json.contains("second"))
        {
            m_url.home["second"] = json["second"].get<std::string>();
        }

        save();

        break;
    }
    }

    if (m_windows.empty())
    {
        save();
        return close();
    }

    else { return 0; }
}
