#pragma once

class Browser
{
  public:
    static std::unique_ptr<Browser> Create(HWND);
    // void UpdateConfig();
    // void UpdateBounds();
    // void UpdateFocus();
    // void SetWindowTitle();
    // HICON GetFavicon();
    // void SetWindowIcon();

  private:
    static wil::com_ptr<ICoreWebView2Environment> settings_environment;
    static wil::com_ptr<ICoreWebView2Controller> settings_controller;
    static wil::com_ptr<ICoreWebView2> settings_core;
    static wil::com_ptr<ICoreWebView2_19> settings_browser;
    static wil::com_ptr<ICoreWebView2Settings> settings_settings;

    Browser(HWND);
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
