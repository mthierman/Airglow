// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#include <airglow/browser.hxx>

namespace airglow
{

auto Browser::accelerator_key_pressed_handler(ICoreWebView2Controller* sender,
                                              ICoreWebView2AcceleratorKeyPressedEventArgs* args)
    -> HRESULT
{
    COREWEBVIEW2_KEY_EVENT_KIND kind{};
    args->get_KeyEventKind(&kind);

    if (kind == COREWEBVIEW2_KEY_EVENT_KIND_KEY_DOWN ||
        kind == COREWEBVIEW2_KEY_EVENT_KIND_SYSTEM_KEY_DOWN)
    {
        UINT key{0};
        args->get_VirtualKey(&key);

        switch (key)
        {
        case 19:
            args->put_Handled(TRUE);
            SendMessageA(m_parent, WM_KEYDOWN, VK_PAUSE, 0);
            break;

        case 76:
            args->put_Handled(TRUE);
            SendMessageA(m_parent, WM_KEYDOWN, 0x4C, 0);

            break;

        case 87:
            args->put_Handled(TRUE);
            SendMessageA(m_parent, WM_KEYDOWN, 0x57, 0);

            break;

        case 112:
            args->put_Handled(TRUE);
            SendMessageA(m_parent, WM_KEYDOWN, VK_F1, 0);

            break;

        case 113:
            args->put_Handled(TRUE);
            SendMessageA(m_parent, WM_KEYDOWN, VK_F2, 0);

            break;

        case 114:
            args->put_Handled(TRUE);
            SendMessageA(m_parent, WM_KEYDOWN, VK_F3, 0);

            break;

        case 115:
            args->put_Handled(TRUE);
            SendMessageA(m_parent, WM_KEYDOWN, VK_F4, 0);

            break;

        case 117:
            args->put_Handled(TRUE);
            SendMessageA(m_parent, WM_KEYDOWN, VK_F6, 0);

            break;

        case 118:
            args->put_Handled(TRUE);
            SendMessageA(m_parent, WM_KEYDOWN, VK_F7, 0);

            break;

        case 119:
            args->put_Handled(TRUE);
            SendMessageA(m_parent, WM_KEYDOWN, VK_F8, 0);

            break;

        case 120:
            args->put_Handled(TRUE);
            SendMessageA(m_parent, WM_KEYDOWN, VK_F9, 0);

            break;

        case 121:
            args->put_Handled(TRUE);
            SendMessageA(m_parent, WM_KEYDOWN, VK_F10, 0);

            break;

        case 122:
            args->put_Handled(TRUE);
            SendMessageA(m_parent, WM_KEYDOWN, VK_F11, 0);

            break;
        }
    }

    return S_OK;
}

auto MainBrowser::source_changed_handler(ICoreWebView2* sender,
                                         ICoreWebView2SourceChangedEventArgs* args) -> HRESULT
{
    glow::console::debug("navigation completed..");
    wil::unique_cotaskmem_string uriRaw;
    sender->get_Source(&uriRaw);

    auto uri{glow::text::narrow(uriRaw.get())};
    glow::console::debug(uri);
    NotificationMsg nMsg;
    nMsg.nmhdr.hwndFrom = m_hwnd.get();
    nMsg.nmhdr.idFrom = m_id;
    nMsg.nmhdr.code = CUSTOM_RECEIVE_MAINURL;
    nMsg.message = uri;

    SendMessage(m_parent, WM_NOTIFY, 0, std::bit_cast<LPARAM>(&nMsg));

    return S_OK;
}

auto MainBrowser::navigation_starting_handler(ICoreWebView2* sender,
                                              ICoreWebView2NavigationStartingEventArgs* args)
    -> HRESULT
{
    glow::console::debug("navigation completed..");
    wil::unique_cotaskmem_string uriRaw;
    sender->get_Source(&uriRaw);

    auto uri{glow::text::narrow(uriRaw.get())};
    glow::console::debug(uri);
    NotificationMsg nMsg;
    nMsg.nmhdr.hwndFrom = m_hwnd.get();
    nMsg.nmhdr.idFrom = m_id;
    nMsg.nmhdr.code = CUSTOM_RECEIVE_MAINURL;
    nMsg.message = uri;

    SendMessage(m_parent, WM_NOTIFY, 0, std::bit_cast<LPARAM>(&nMsg));

    return S_OK;
}

auto MainBrowser::navigation_completed_handler(ICoreWebView2* sender,
                                               ICoreWebView2NavigationCompletedEventArgs* args)
    -> HRESULT
{
    // glow::console::debug("navigation completed..");
    // wil::unique_cotaskmem_string uriRaw;
    // sender->get_Source(&uriRaw);

    // auto uri{glow::text::narrow(uriRaw.get())};
    // glow::console::debug(uri);
    // NotificationMsg nMsg;
    // nMsg.nmhdr.hwndFrom = m_hwnd.get();
    // nMsg.nmhdr.idFrom = m_id;
    // nMsg.nmhdr.code = CUSTOM_RECEIVE_MAINURL;
    // nMsg.message = uri;

    // SendMessage(m_parent, WM_NOTIFY, 0, std::bit_cast<LPARAM>(&nMsg));

    return S_OK;
}

auto SideBrowser::source_changed_handler(ICoreWebView2* sender,
                                         ICoreWebView2SourceChangedEventArgs* args) -> HRESULT
{
    // wil::unique_cotaskmem_string uriRaw;
    // sender->get_Source(&uriRaw);

    // auto uri{glow::text::narrow(uriRaw.get())};
    // NotificationMsg nMsg;
    // nMsg.nmhdr.hwndFrom = m_hwnd.get();
    // nMsg.nmhdr.idFrom = m_id;
    // nMsg.nmhdr.code = CUSTOM_RECEIVE_SIDEURL;
    // nMsg.message = uri;

    // SendMessage(m_parent, WM_NOTIFY, 0, std::bit_cast<LPARAM>(&nMsg));

    return S_OK;
}

} // namespace airglow
