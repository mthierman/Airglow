// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#include "webview.hxx"

//==============================================================================
namespace airglow
{

//==============================================================================
auto WebView::accelerator_key_pressed_handler() -> void
{
    OutputDebugString("KEY PRESSED");
    EventRegistrationToken acceleratorKeyPressedToken;

    m_controller4->add_AcceleratorKeyPressed(
        Microsoft::WRL::Callback<ICoreWebView2AcceleratorKeyPressedEventHandler>(
            [=, this](ICoreWebView2Controller* sender,
                      ICoreWebView2AcceleratorKeyPressedEventArgs* args) -> HRESULT
            {
                COREWEBVIEW2_KEY_EVENT_KIND kind;
                args->get_KeyEventKind(&kind);
                if (kind == COREWEBVIEW2_KEY_EVENT_KIND_KEY_DOWN ||
                    kind == COREWEBVIEW2_KEY_EVENT_KIND_KEY_UP)
                {
                    UINT key;
                    args->get_VirtualKey(&key);
                    OutputDebugString(std::to_string(key).c_str());
                    switch (key)
                    {
                    case 19:
                        args->put_Handled(TRUE);
                        SendMessage(m_hwndParent.get(), WM_KEYDOWN, VK_PAUSE, 0);
                        break;
                    }
                }
                return S_OK;
            })
            .Get(),
        &acceleratorKeyPressedToken);
}

//==============================================================================
} // namespace airglow
