class WebView
{
  public:
    WebView();
    static void Create(HWND, std::filesystem::path);
    // std::wstring GetScriptFile(std::filesystem::path);
    // std::wstring GetScript(std::filesystem::path);
    // std::wstring GetMenuScript(std::filesystem::path);
    // void SetWindowTitle(HWND);
    // void SetWindowIcon(HWND);
    static wil::com_ptr<ICoreWebView2_19> settings_wv;
    static wil::com_ptr<ICoreWebView2Controller> settings_controller;
    static wil::com_ptr<ICoreWebView2> settings_core;
    static wil::com_ptr<ICoreWebView2Settings> settings_settings;
    static wil::com_ptr<ICoreWebView2_19> main_wv;
    static wil::com_ptr<ICoreWebView2Controller> main_controller;
    static wil::com_ptr<ICoreWebView2> main_core;
    static wil::com_ptr<ICoreWebView2Settings> main_settings;
    static wil::com_ptr<ICoreWebView2_19> side_wv;
    static wil::com_ptr<ICoreWebView2Controller> side_controller;
    static wil::com_ptr<ICoreWebView2> side_core;
    static wil::com_ptr<ICoreWebView2Settings> side_settings;
};
