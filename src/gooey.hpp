// MAIN
int APIENTRY wWinMain(HINSTANCE, HINSTANCE, PWSTR, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
ATOM Application(HINSTANCE);

// ENV
std::wstring wvBackgroundColor(L"WEBVIEW2_DEFAULT_BACKGROUND_COLOR");
std::wstring wvBackgroundColorValue(L"0");
std::wstring wvAdditionalBrowserArgs(L"WEBVIEW2_ADDITIONAL_BROWSER_ARGUMENTS");
std::wstring wvAdditionalBrowserArgsValue(L"--enable-features=OverlayScrollbar,"
                                          L"msOverlayScrollbarWinStyle:scrollbar_mode/"
                                          L"full_mode,msOverlayScrollbarWinStyleAnimation,"
                                          L"msWebView2BrowserHitTransparent"
                                          L" --disable-features=msWebOOUI,msPdfOOUI");

// WINDOW
HICON icon;
HCURSOR cursor;
HBRUSH hollowBrush;
HBRUSH blackBrush;
HBRUSH whiteBrush;
HBRUSH darkBrush;
HBRUSH lightBrush;
WNDCLASSEXW wcex;
std::wstring programIcon(L"PROGRAM_ICON");
std::wstring menuName(L"menu");
std::wstring className(L"window");
std::wstring windowName(L"Gooey");
MSG msg;

// WEBVIEW1
RECT bounds;
RECT wvRect;
static wil::com_ptr<ICoreWebView2Controller> wv_controller;
static wil::com_ptr<ICoreWebView2> wv;
static wil::com_ptr<ICoreWebView2Settings> wv_settings;

// WEBVIEW2
RECT bounds2;
RECT wvRect2;
static wil::com_ptr<ICoreWebView2Controller> wv_controller2;
static wil::com_ptr<ICoreWebView2> wv2;
static wil::com_ptr<ICoreWebView2Settings> wv_settings2;

// WEBVIEW
void WebViewNavigate(wil::com_ptr<ICoreWebView2>);
LPWSTR commandLine;
LPWSTR* commandLineList;
int nArgs;
int i;

std::wstring wvScript(
    L"document.onreadystatechange = () => {if (document.readyState === 'complete') {onkeydown = "
    L"(e) => {if (e.ctrlKey && e.key === 'w') {window.chrome.webview.postMessage('close')} else "
    L"{window.chrome.webview.postMessage(e.key)}}}}");

// THEMING
int darkMode;
int ModeCheck();
void DarkTitle();
void DarkMode(HWND);
void SetMica(HWND);
bool ExtendFrame(HWND);
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
bool micaFrame;

// KEYBOARD SHORTCUTS
int vkKeyTop = VK_F1;
int vkKeyMax = VK_F10;
int vkKeyFull = VK_F11;
int vkKeyControl = VK_CONTROL;
int vkKeyW = 0x57;
std::wstring keyTop(L"F1");
std::wstring keyMax(L"F10");
std::wstring keyFull(L"F11");
std::wstring keyClose(L"close");
void KeyTop(HWND);
LRESULT topMost;
void KeyMaximize(HWND);
WINDOWPLACEMENT wPlacement;
BOOL placement;
void KeyFullscreen(HWND);
static RECT position;
LRESULT style;
MONITORINFO mi;
void KeyClose(HWND);

// MISC
std::wstring StringToWide(HINSTANCE, UINT);
