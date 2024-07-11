// clang-format off
// Airglow - https://github.com/mthierman/Airglow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#include "app.hxx"
// #include "global.hxx"

#include <filesystem>
#include <fstream>

App::App(glow::system::Event& singleInstance)
    : m_singleInstance { singleInstance },
      m_gdiToken { glow::system::gdi_plus_startup() } {
    if (!std::filesystem::exists(file())) {
        save();
    }

    else {
        load();
    }

    // parse_args();

    // m_settings = std::make_unique<Settings>(m_hwnd.get(), m_state);
    // m_settings->create_window();

    // new_window();

    message(WM_NOTIFY, [this](glow::messages::wm_notify message) {
        //     auto notification { reinterpret_cast<glow::Notification*>(lParam) };

        //     switch (notification->code) {
        //         using enum CODE;

        //         case SETTINGS_TOGGLE: {
        //             if (!m_settings->is_visible()) {
        //                 m_settings->show();
        //                 m_settings->m_browser->move_focus();
        //             }

        //             else if (m_settings->is_visible()) {
        //                 if (!m_settings->is_foreground()) {
        //                     m_settings->foreground();
        //                     m_settings->m_browser->move_focus();
        //                 }

        //                 else {
        //                     m_settings->hide();

        //                     if (!m_windows.empty()) {
        //                         m_windows.find(m_active)->second->m_first.browser->move_focus();
        //                     }
        //                 }
        //             }

        //             break;
        //         }

        //         case SETTINGS_SAVE: {
        //             save();

        //             break;
        //         }

        //         case WINDOW_NEW: {
        //             new_window();

        //             break;
        //         }

        //         case WINDOW_CLOSE: {
        //             m_windows.erase(notification->id);

        //             if (m_windows.empty()) {
        //                 save();

        //                 close();
        //             }

        //             break;
        //         }

        //         case WINDOW_ACTIVATE: {
        //             m_active = notification->id;

        //             break;
        //         }

        //         default:
        //             break;
        //     }

        create_message_only();

        m_webViewEnvironment.create([this]() {
            // m_singleInstance.m_callback = [this]() { notify(CREATE_FOREGROUND_WINDOW); };

            // notify(CREATE_WINDOW);
        });

        return 0;
    });
}

App::~App() { glow::system::gdi_plus_shutdown(m_gdiToken); }

auto App::operator()() -> int { return glow::messages::run_loop(); }

auto App::data() -> std::filesystem::path {
    auto data { glow::filesystem::known_folder(FOLDERID_LocalAppData, { "Airglow" }) };

    if (!std::filesystem::exists(data)) {
        std::filesystem::create_directory(data);
    }

    return data;
}

auto App::file() -> std::filesystem::path {
    return std::filesystem::path { { data() / "Airglow.json" } };
}

auto App::save() -> void {
    try {
        std::ofstream f(file());
        f << std::setw(4) << nlohmann::json(m_state) << std::endl;
        f.close();
    } catch (const std::exception& e) {
        return;
    }
}

auto App::load() -> void {
    try {
        std::ifstream f(file());
        m_state = nlohmann::json::parse(f, nullptr, false, true);
        f.close();
    } catch (const std::exception& e) {
        return;
    }
}

auto App::parse_args(int argc, char* argv[]) -> void {
    auto args { std::vector<std::string>(argv, argv + argc) };

    if (args.size() > 1) {
        m_state.withArgs = true;
    }

    if (args.size() == 2) {
        m_state.args.first = args.at(1);
    }

    else if (args.size() > 2) {
        m_state.args.first = args.at(1);
        m_state.args.second = args.at(2);
    }
}

// auto App::new_window() -> void {
//     auto id { glow::random<size_t>() };
//     m_windows.try_emplace(id, std::make_unique<Window>(m_hwnd.get(), m_state, id));
//     m_windows.at(id)->create_window();
//     m_windows.at(id)->reveal();
// }
