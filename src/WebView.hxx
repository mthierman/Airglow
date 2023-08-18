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
    WebView(Storage*, HWND, std::string, std::string);
    ~WebView();

    winrt::IAsyncAction create_webview();
    void resized();

    void post_settings(nlohmann::json j);
    void navigate_home();
    void navigate_main();
    void navigate_side();
    void title();
    winrt::IAsyncAction icon();
    void focus();
    void navigation_completed();

    winrt::CoreWebView2Environment environment{nullptr};
    winrt::CoreWebView2Controller controller{nullptr};
    winrt::CoreWebView2 core{nullptr};

  private:
    void gui_web_message_received(winrt::CoreWebView2WebMessageReceivedEventArgs const&);
    void accelerator_key_pressed(winrt::CoreWebView2AcceleratorKeyPressedEventArgs const&);

    Storage* storage;
    HWND appHwnd;
    std::string name{};
    winrt::hstring url{};
};
