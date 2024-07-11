// clang-format off
// Airglow - https://github.com/mthierman/Airglow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#include "settings.hxx"

namespace airglow {
Settings::Settings(glow::webview::WebViewEnvironment& webViewEnvironment)
    : m_webViewEnvironment { webViewEnvironment } {
    message(WM_CREATE, [this](glow::messages::wm_create message) {
        glow::window::set_backdrop(m_hwnd.get(), DWM_SYSTEMBACKDROP_TYPE::DWMSBT_TRANSIENTWINDOW);
        glow::window::set_position(m_hwnd.get(), 0, 0, 500, 500);
        return 0;
    });

    message(WM_CLOSE, [this](glow::messages::wm message) {
        glow::window::hide(m_hwnd.get());

        return 0;
    });

    message(WM_GETMINMAXINFO, [this](glow::messages::wm_getminmaxinfo message) {
        message.minMaxInfo().ptMinTrackSize.x = 500;
        message.minMaxInfo().ptMinTrackSize.y = 500;

        return 0;
    });

    message(WM_KEYDOWN, [this](glow::messages::wm_keydown_keyup message) {
        using enum glow::messages::notice;
        auto key { message.key() };

        if (message.wasKeyDown()) {
            return 0;
        }

        switch (key) {
            case VK_PAUSE: {
                notify(TOGGLE_SETTINGS);
            } break;

            case 0x57: {
                if (glow::input::was_key_down(VK_CONTROL)) {
                    notify(TOGGLE_SETTINGS);
                }
            } break;

            case VK_F4: {
                if (glow::input::was_key_down(VK_CONTROL)) {
                    notify(TOGGLE_SETTINGS);
                }
            } break;
        }

        return 0;
    });

    create("Settings", false);

    m_webView.create(m_webViewEnvironment, m_hwnd.get(), [this]() {
        m_webView.navigate("about:blank");
        m_webView.put_bounds(m_hwnd.get());

        message(WM_SIZE, [this](glow::messages::wm_size message) {
            m_webView.put_bounds(m_hwnd.get());
            return 0;
        });

        message(WM_SHOWWINDOW, [this](glow::messages::wm_showwindow message) {
            switch (message.show()) {
                case true: {
                    m_webView.show();

                } break;
                case false: {
                    m_webView.hide();
                } break;
            }
            return 0;
        });

        message(WM_NOTIFY, [this](glow::messages::wm_notify message) {
            auto& notification { message.notification() };

            switch (notification.notice) {
                using enum glow::messages::notice;

                case WEB_MESSAGE_RECEIVED: {
                    // auto webMessage { json::parse(notification->message) };

                    // if (webMessage.contains("initialized")) {
                    //     m_browser->post_json(json(*this));
                    // }

                    // else if (webMessage.contains("m_state")) {
                    //     m_state = webMessage["m_state"].get<State>();
                    //     notify(m_app, CODE::SETTINGS_SAVE);
                    //     m_browser->post_json(json(*this));
                    // }
                } break;
            }

            return 0;
        });
    });
}
}; // namespace airglow
