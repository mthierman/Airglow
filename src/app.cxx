// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#include "app.hxx"
#include "global.hxx"

#include <filesystem>
#include <fstream>

auto App::WndProc(::UINT uMsg, ::WPARAM wParam, ::LPARAM lParam) -> ::LRESULT
{
    switch (uMsg)
    {
        case WM_CREATE: return on_create(wParam, lParam);
        case WM_NOTIFY: return on_notify(wParam, lParam);
    }

    return ::DefWindowProcA(m_hwnd.get(), uMsg, wParam, lParam);
}

auto App::on_create(::WPARAM wParam, ::LPARAM lParam) -> int
{
    ::SetEnvironmentVariableA("WEBVIEW2_DEFAULT_BACKGROUND_COLOR", "0");
    ::SetEnvironmentVariableA("WEBVIEW2_ADDITIONAL_BROWSER_ARGUMENTS",
                              "--allow-file-access-from-files");

    if (!std::filesystem::exists(file())) { save(); }

    else { load(); }

    parse_args();

    m_settings = std::make_unique<Settings>(m_hwnd.get(), m_state);
    m_settings->create_window();

    new_window();

    return 0;
}

auto App::on_notify(::WPARAM wParam, ::LPARAM lParam) -> int
{
    auto notification{reinterpret_cast<glow::Notification*>(lParam)};

    switch (notification->code)
    {
        using enum CODE;

        case SETTINGS_TOGGLE:
        {
            if (!m_settings->is_visible())
            {
                m_settings->show();
                m_settings->m_browser->move_focus();
            }

            else if (m_settings->is_visible())
            {
                if (!m_settings->is_foreground())
                {
                    m_settings->foreground();
                    m_settings->m_browser->move_focus();
                }

                else
                {
                    m_settings->hide();

                    if (!m_windows.empty())
                    {
                        m_windows.find(m_active)->second->m_first.browser->move_focus();
                    }
                }
            }

            break;
        }

        case SETTINGS_SAVE:
        {
            save();

            break;
        }

        case WINDOW_NEW:
        {
            new_window();

            break;
        }

        case WINDOW_CLOSE:
        {
            m_windows.erase(notification->id);

            if (m_windows.empty())
            {
                save();

                close();
            }

            break;
        }

        case WINDOW_ACTIVATE:
        {
            m_active = notification->id;

            break;
        }
    }

    return 0;
}

auto App::parse_args() -> void
{
    auto argv{glow::cmd_to_argv()};

    if (argv.size() > 1) { m_state.withArgs = true; }

    if (argv.size() == 2) { m_state.args.first = argv.at(1); }

    else if (argv.size() > 2)
    {
        m_state.args.first = argv.at(1);
        m_state.args.second = argv.at(2);
    }
}

auto App::new_window() -> void
{
    auto id{glow::random<size_t>()};
    m_windows.try_emplace(id, std::make_unique<Window>(m_hwnd.get(), m_state, id));
    m_windows.at(id)->create_window();
    m_windows.at(id)->reveal();
}

auto App::data() -> std::filesystem::path
{
    auto path{glow::known_folder() / "Airglow"};

    if (!std::filesystem::exists(path)) { std::filesystem::create_directory(path); }

    return path;
}

auto App::file() -> std::filesystem::path
{
    return std::filesystem::path{{glow::app_path() / "Airglow.json"}};
}

auto App::save() -> void
{
    try
    {
        std::ofstream f(file());
        f << std::setw(4) << nlohmann::json(m_state) << std::endl;
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
        std::ifstream f(file());
        m_state = nlohmann::json::parse(f, nullptr, false, true);
        f.close();
    }
    catch (const std::exception& e)
    {
        return;
    }
}
