// clang-format off
// Airglow - https://github.com/mthierman/Airglow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#include "app.hxx"
// #include "global.hxx"

#include <filesystem>
#include <fstream>

App::App(std::vector<std::string>& args, glow::system::Event& singleInstance)
    : m_args { args },
      m_singleInstance { singleInstance } {
    if (!std::filesystem::exists(m_appData)) {
        std::filesystem::create_directory(m_appData);
    }

    if (!std::filesystem::exists(m_settingsFile)) {
        save_settings();
    }

    else {
        load_settings();
    }

    if (m_args.size() > 1) {
        m_state.withArgs = true;
    }

    if (m_args.size() == 2) {
        m_state.args.first = m_args.at(1);
    }

    else if (m_args.size() > 2) {
        m_state.args.first = m_args.at(1);
        m_state.args.second = m_args.at(2);
    }

    message(WM_NOTIFY, [this](glow::messages::wm_notify message) {
        auto& notification { message.notification() };

        switch (notification.notice) {
            using enum glow::messages::notice;
            case SETTINGS_TOGGLE: {
                // if (!m_settings->is_visible()) {
                //     m_settings->show();
                //     m_settings->m_browser->move_focus();
                // }

                // else if (m_settings->is_visible()) {
                //     if (!m_settings->is_foreground()) {
                //         m_settings->foreground();
                //         m_settings->m_browser->move_focus();
                //     }

                //     else {
                //         m_settings->hide();

                //         if (!m_windows.empty()) {
                //             m_windows.find(m_active)->second->m_first.browser->move_focus();
                //         }
                //     }
                // }
            } break;

            case SETTINGS_SAVE: {
                // save();
            } break;

            case CREATE_WINDOW: {
                // new_window();
            } break;

            case CLOSE_WINDOW: {
                // m_windows.erase(notification->id);

                // if (m_windows.empty()) {
                //     save();

                //     close();
                // }
            } break;

            case ACTIVATE_WINDOW: {
                // m_active = notification->id;
            } break;
        }

        return 0;
    });

    create_message_only();

    m_webViewEnvironment.create([this]() {
        m_singleInstance.m_callback
            = [this]() { notify(glow::messages::notice::CREATE_FOREGROUND_WINDOW); };
        m_settings = std::make_unique<Settings>();
        notify(glow::messages::notice::CREATE_WINDOW);
    });
}

App::~App() { glow::system::gdi_plus_shutdown(m_gdiPlusToken); }

auto App::operator()() -> int { return glow::messages::run_loop(); }

auto App::save_settings() -> void {
    try {
        std::ofstream f(m_settingsFile);
        f << std::setw(4) << nlohmann::json(m_state) << std::endl;
        f.close();
    } catch (const std::exception& e) {
        return;
    }
}

auto App::load_settings() -> void {
    try {
        std::ifstream f(m_settingsFile);
        m_state = nlohmann::json::parse(f, nullptr, false, true);
        f.close();
    } catch (const std::exception& e) {
        return;
    }
}
