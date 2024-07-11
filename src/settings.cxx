// clang-format off
// Airglow - https://github.com/mthierman/Airglow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#include "settings.hxx"

namespace airglow {
Settings::Settings(glow::webview::WebViewEnvironment& webViewEnvironment)
    : m_webViewEnvironment { webViewEnvironment } {
    message(WM_GETMINMAXINFO, [this](glow::messages::wm_getminmaxinfo message) {
        message.minMaxInfo().ptMinTrackSize.x = 500;
        message.minMaxInfo().ptMinTrackSize.y = 500;

        return 0;
    });

    message(WM_CREATE, [this](glow::messages::wm_create message) {
        glow::window::set_backdrop(m_hwnd.get(), DWM_SYSTEMBACKDROP_TYPE::DWMSBT_TRANSIENTWINDOW);
        glow::window::set_position(m_hwnd.get(), 0, 0, 500, 500);
        return 0;
    });

    message(WM_CLOSE, [this](glow::messages::wm message) {
        glow::window::hide(m_hwnd.get());

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
    });
}

// auto CALLBACK Settings::EnumChildProc(::HWND hWnd, ::LPARAM lParam) -> ::BOOL {
//     auto self { reinterpret_cast<Settings*>(lParam) };

//     if (!self) {
//         return true;
//     }

//     auto gwlId { static_cast<size_t>(::GetWindowLongPtrA(hWnd, GWL_ID)) };
//     auto& rect { self->m_client.rect };
//     auto& width { rect.right };
//     auto& height { rect.bottom };

//     auto hdwp { ::BeginDeferWindowPos(1) };

//     if (gwlId == self->m_browser->m_id) {
//         if (hdwp && self->m_browser) {
//             hdwp = ::DeferWindowPos(hdwp,
//                                     hWnd,
//                                     nullptr,
//                                     0,
//                                     0,
//                                     width,
//                                     height,
//                                     SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOOWNERZORDER |
//                                     SWP_NOREDRAW
//                                         | SWP_NOCOPYBITS);
//         }
//     }

//     if (hdwp) {
//         ::EndDeferWindowPos(hdwp);
//     }

//     return true;
// }

// auto Settings::on_key_down(::WPARAM wParam, ::LPARAM lParam) -> int {
//     auto key { static_cast<unsigned int>(wParam) };

//     if ((HIWORD(lParam) & KF_REPEAT) == KF_REPEAT) {
//         return 0;
//     }

//     if (m_keys.set.contains(key)) {
//         switch (key) {
//             case VK_PAUSE: {
//                 notify(m_app, CODE::SETTINGS_TOGGLE);

//                 break;
//             }

//             case 0x57: {
//                 if (::GetKeyState(VK_CONTROL) & 0x8000) {
//                     notify(m_app, CODE::SETTINGS_TOGGLE);
//                 }

//                 break;
//             }

//             case VK_F4: {
//                 if (::GetKeyState(VK_MENU) & 0x8000) {
//                     notify(m_app, CODE::SETTINGS_TOGGLE);
//                 }

//                 break;
//             }
//         }
//     }

//     return 0;
// }

// auto Settings::on_notify(::WPARAM wParam, ::LPARAM lParam) -> int {
//     auto notification { reinterpret_cast<glow::Notification*>(lParam) };

//     switch (notification->code) {
//         using enum CODE;

//         case WEB_MESSAGE_RECEIVED: {
//             auto webMessage { json::parse(notification->message) };

//             if (webMessage.contains("initialized")) {
//                 m_browser->post_json(json(*this));
//             }

//             else if (webMessage.contains("m_state")) {
//                 m_state = webMessage["m_state"].get<State>();
//                 notify(m_app, CODE::SETTINGS_SAVE);
//                 m_browser->post_json(json(*this));
//             }

//             break;
//         }

//         default:
//             break;
//     }

//     return 0;
// }
}; // namespace airglow
