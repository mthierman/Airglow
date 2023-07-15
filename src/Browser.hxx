#pragma once

class Browser
{
  public:
    static std::unique_ptr<Browser> Create(HWND hwnd);
    // void UpdateConfig();
    // void UpdateBounds();
    // void UpdateFocus();
    // void SetWindowTitle();
    // HICON GetFavicon();
    // void SetWindowIcon();

  private:
    static wil::com_ptr<ICoreWebView2Controller> wv2_controller;
    static wil::com_ptr<ICoreWebView2> wv2;
    static wil::com_ptr<ICoreWebView2_19> wv2_19;
    static wil::com_ptr<ICoreWebView2Settings> wv2_settings;

    Browser(HWND hwnd);
    // std::pair<wstring, wstring> CommandLine();
    // wstring MainNavigation();
    // wstring SideNavigation();
    // bool VerifySettingsUrl(ICoreWebView2WebMessageReceivedEventArgs* args);
    // wstring SettingsNavigation();
    // void Messages(ICoreWebView2WebMessageReceivedEventArgs*);
    // RECT SettingsBounds();
    // RECT MainBounds();
    // RECT SideBounds();
    // wstring GetScriptFile();
    // wstring GetScript();
};

// namespace Browsers
// {
// namespace Main
// {
// static wil::com_ptr<ICoreWebView2Controller> controller{};
// static wil::com_ptr<ICoreWebView2> core{};
// static wil::com_ptr<ICoreWebView2_19> browser{};
// static wil::com_ptr<ICoreWebView2Settings> settings{};
// }; // namespace Main

// namespace Side
// {
// static wil::com_ptr<ICoreWebView2Controller> controller{};
// static wil::com_ptr<ICoreWebView2> core{};
// static wil::com_ptr<ICoreWebView2_19> browser{};
// static wil::com_ptr<ICoreWebView2Settings> settings{};
// }; // namespace Side

// namespace Settings
// {
// static wil::com_ptr<ICoreWebView2Controller> controller{};
// static wil::com_ptr<ICoreWebView2> core{};
// static wil::com_ptr<ICoreWebView2_19> browser{};
// static wil::com_ptr<ICoreWebView2Settings> settings{};
// }; // namespace Settings
// }; // namespace Browsers
