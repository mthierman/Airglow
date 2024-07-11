// clang-format off
// Airglow - https://github.com/mthierman/Airglow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#include "app.hxx"
// #include "global.hxx"

#include <filesystem>
#include <fstream>

namespace airglow {
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

    create_message_only();

    m_webViewEnvironment.create([this]() {
        message(WM_NOTIFY, [this](glow::messages::wm_notify message) {
            auto& notification { message.notification() };

            switch (notification.notice) {
                using enum glow::messages::notice;

                case SINGLE_INSTANCE: {
                    notify(CREATE_BROWSER);
                    glow::window::set_foreground(m_windows.last_window());
                } break;

                case TOGGLE_SETTINGS: {
                    auto visible { glow::window::check_visibility(m_settings->m_hwnd.get()) };
                    auto foreground { glow::window::get_foreground(m_settings->m_hwnd.get())
                                      == m_settings->m_hwnd.get() };
                    if (!visible) {
                        glow::window::show(m_settings->m_hwnd.get());
                        // m_settings->m_browser->move_focus();
                    } else if (visible && !foreground) {
                        glow::window::set_foreground(m_settings->m_hwnd.get());
                        // m_settings->m_browser->move_focus();
                    } else {
                        glow::window::hide(m_settings->m_hwnd.get());
                        // if (!m_windows.empty()) {
                        //     m_windows.find(m_active)->second->m_first.browser->move_focus();
                        // }
                    }
                } break;

                case SAVE_SETTINGS: {
                    save_settings();
                } break;

                case CREATE_BROWSER: {
                    m_windows.add(std::make_unique<Browser>());
                } break;

                case CLOSE_BROWSER: {
                    m_windows.remove(notification.idFrom);
                } break;

                case ACTIVE_BROWSER: {
                    m_active = notification.idFrom;
                } break;
            }

            return 0;
        });

        m_singleInstance.m_callback = [this]() { notify(glow::messages::notice::SINGLE_INSTANCE); };
        m_settings = std::make_unique<Settings>();
        notify(glow::messages::notice::CREATE_BROWSER);
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
}; // namespace airglow
