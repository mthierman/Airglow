// clang-format off
// ╔─────────────────────────────╗
// │                             │
// │     ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦     │  Airglow - https://github.com/mthierman/Airglow
// │     ╠═╣║╠╦╝║ ╦║  ║ ║║║║     │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │     ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝     │  SPDX-License-Identifier: MIT
// │                             │
// ╚─────────────────────────────╝
// clang-format on

#pragma once

#include "Utility.hxx"
#include "Storage.hxx"

namespace winrt
{
using namespace winrt::Microsoft::Web::WebView2::Core;
using namespace winrt::Windows::Foundation;
}; // namespace winrt

class WebView
{
  public:
    WebView(Storage*, HWND, std::string);
    ~WebView();

    winrt::IAsyncAction create_webview();

    void post_settings(nlohmann::json j);
    void title();
    winrt::IAsyncAction icon();
    void focus();
    void navigation_completed();
    void initial_navigation();

    std::string parse_url(std::string);
    void navigate(std::string);

    winrt::CoreWebView2Environment environment{nullptr};
    winrt::CoreWebView2Profile profile{nullptr};
    winrt::CoreWebView2Controller controller{nullptr};
    winrt::CoreWebView2 core{nullptr};

  private:
    void gui_web_message_received(winrt::CoreWebView2WebMessageReceivedEventArgs const&);
    void bar_web_message_received(winrt::CoreWebView2WebMessageReceivedEventArgs const&);
    void accelerator_key_pressed(winrt::CoreWebView2AcceleratorKeyPressedEventArgs const&);

    Storage* storage{nullptr};
    HWND appHwnd{nullptr};
    std::string name{};
};
