// clang-format off
// Airglow - https://github.com/mthierman/Airglow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#include "settings.hxx"

namespace airglow {
Settings::Settings() {
    // message(WM_GETMINMAXINFO, [this](glow::messages::wm_getminmaxinfo message) {
    //     message.minMaxInfo().ptMinTrackSize.x = 500;
    //     message.minMaxInfo().ptMinTrackSize.y = 500;

    //     return 0;
    // });

    create("Settings", false);
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

// auto Settings::on_create(::WPARAM wParam, ::LPARAM lParam) -> int {
//     position();

//     dwm_caption_color(false);
//     dwm_system_backdrop(DWMSBT_TRANSIENTWINDOW);
//     theme();

//     ::SetWindowPos(m_hwnd.get(), nullptr, 0, 0, 500, 500, SWP_NOMOVE);

//     std::function<::HRESULT()> settingsCallback { [=, this]() {
//         m_browser->navigate(m_browser->url("settings"));

//         return S_OK;
//     } };

//     m_browser = std::make_unique<Browser>(m_hwnd.get(), settingsCallback);
//     m_browser->create_window();
//     m_browser->reveal();

//     return 0;
// }

// auto Settings::on_close(::WPARAM wParam, ::LPARAM lParam) -> int {
//     hide();

//     return 0;
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

// auto Settings::on_setting_change(::WPARAM wParam, ::LPARAM lParam) -> int {
//     theme();

//     return 0;
// }

// auto Settings::on_show_window(::WPARAM wParam, ::LPARAM lParam) -> int {
//     switch (wParam) {
//         case true: {
//             m_browser->put_is_visible(true);

//             break;
//         }

//         case false: {
//             m_browser->put_is_visible(false);

//             break;
//         }
//     }

//     return 0;
// }

// auto Settings::on_size(::WPARAM wParam, ::LPARAM lParam) -> int {
//     position();
//     ::EnumChildWindows(m_hwnd.get(), EnumChildProc, reinterpret_cast<size_t>(this));

//     return 0;
// }
}; // namespace airglow
