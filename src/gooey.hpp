// MAIN
int APIENTRY wWinMain(HINSTANCE histance, HINSTANCE hprevinstance, PWSTR pcmdline, int ncmdshow);
LRESULT CALLBACK WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);

// ENV
std::wstring wvBackgroundColor(L"WEBVIEW2_DEFAULT_BACKGROUND_COLOR");
std::wstring wvBackgroundColorValue(L"0");
std::wstring wvAdditionalBrowserArgs(L"WEBVIEW2_ADDITIONAL_BROWSER_ARGUMENTS");
std::wstring wvAdditionalBrowserArgsValue(L"--enable-features=OverlayScrollbar,"
                                          L"msOverlayScrollbarWinStyle:scrollbar_mode/"
                                          L"full_mode,msOverlayScrollbarWinStyleAnimation,"
                                          L"msWebView2BrowserHitTransparent"
                                          L" --disable-features=msWebOOUI,msPdfOOUI");
std::wstring programIcon(L"PROGRAM_ICON");

// WINDOW
HICON icon;
HCURSOR cursor;
HBRUSH hollowBrush;
HBRUSH darkBrush;
HBRUSH lightBrush;
WNDCLASSEXW wcex;
std::wstring menuName(L"menu");
std::wstring className(L"window");
std::wstring windowName(L"Gooey");
MSG msg;

// WEBVIEW
static wil::com_ptr<ICoreWebView2Controller> wv_controller;
static wil::com_ptr<ICoreWebView2> wv;
static wil::com_ptr<ICoreWebView2Settings> wv_settings;
void WebViewNavigate(wil::com_ptr<ICoreWebView2> wv);
LPWSTR commandLine;
LPWSTR* commandLineList;
int nArgs;
int i;

// THEMING
int darkMode;
int ModeCheck();
void DarkTitle();
void DarkMode(HWND hwnd);
void SetMica(HWND hwnd);
void ExtendFrame(HWND hwnd);
winrt::Windows::UI::ViewManagement::UISettings settingsCheck;
winrt::Windows::UI::Color fgCheck;
enum PreferredAppMode
{
    Default,
    AllowDark,
    ForceDark,
    ForceLight,
    Max
};
HMODULE uxtheme;
FARPROC ord135;
int dwmtrue;
int dwmfalse;
HRESULT hrExtend;
DWM_SYSTEMBACKDROP_TYPE mica;
MARGINS m;
HRESULT hrMica;

// KEYBOARD SHORTCUTS
int vkKeyTop = VK_F1;
int vkKeyMax = VK_F10;
int vkKeyFull = VK_F11;
std::wstring keyTop(L"F1");
std::wstring keyMax(L"F10");
std::wstring keyFull(L"F11");
void KeyTop(HWND hwnd);
LRESULT topMost;
void KeyMaximize(HWND hwnd);
WINDOWPLACEMENT wp;
int placement;
void KeyFullscreen(HWND hwnd);
static RECT position;
LRESULT style;
MONITORINFO mi;
