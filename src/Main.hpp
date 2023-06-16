#define PROGRAM_ICON 1

// MAIN
int __stdcall wWinMain(HINSTANCE, HINSTANCE, PWSTR, int);
__int64 __stdcall WndProc(HWND, UINT, WPARAM, LPARAM);

// WINDOW
unsigned short MakeWindowClass(HINSTANCE);
HWND MakeWindow(HINSTANCE);
HWND InitializeWindow(HINSTANCE, int);
void SetWindowTitle(HWND window);
void SetWindowIcon(HWND window);
RECT GetMenuBounds(HWND window);
RECT GetMainPanelBounds(HWND window);
RECT GetSidePanelBounds(HWND window);
bool PanelHideMenu(HWND);
bool PanelSplit(HWND);
bool PanelSwap(HWND window);
bool WindowMaximize(HWND);
bool WindowFullscreen(HWND);
bool WindowTop(HWND);
bool CheckSystemDarkMode();
bool SetDarkTitle();
bool SetDarkMode(HWND);
bool SetMica(HWND);
bool SetWindow(HWND, int);
enum PreferredAppMode
{
    Default,
    AllowDark,
    ForceDark,
    ForceLight,
    Max
};
std::wstring programIcon(L"PROGRAM_ICON");
std::wstring className(L"window");
std::wstring windowName(L"Airglow");
std::wstring menuName(L"menu");
RECT dimensions = {0, 0, 0, 0};
bool ontop = false;
bool maximized = false;
bool fullscreen = false;
bool split = false;
bool swapped = false;
bool menu = false;

// SETTINGS
std::wstring ToWide(std::string input);
std::string ToString(std::wstring input);
std::filesystem::path GetAppDataPath();
std::filesystem::path GetSettingsFilePath();
nlohmann::json DefaultSettings();
nlohmann::json LoadSettings(std::filesystem::path settingsFile);
void SaveSettings(nlohmann::json input, std::filesystem::path settingsFile);

// GDI+
unsigned long long gdiplusToken;
Gdiplus::GdiplusStartupInput gdiplusStartupInput;

// ENV
std::wstring wvBackgroundColor(L"WEBVIEW2_DEFAULT_BACKGROUND_COLOR");
std::wstring wvBackgroundColorValue(L"0");
std::wstring wvAdditionalBrowserArgs(L"WEBVIEW2_ADDITIONAL_BROWSER_ARGUMENTS");
std::wstring wvAdditionalBrowserArgsValue(L"--enable-features="
                                          L"msWebView2BrowserHitTransparent");

// std::wstring wvAdditionalBrowserArgsValue(L"");

// std::wstring wvAdditionalBrowserArgsValue(L"--enable-features="
//                                           L"OverlayScrollbar,"
//                                           L"msOverlayScrollbarWinStyle,"
//                                           L"msOverlayScrollbarWinStyleAnimation,"
//                                           L"msWebView2BrowserHitTransparent");

// std::wstring wvAdditionalBrowserArgsValue(L"--enable-features="
//                                           L"OverlayScrollbar,"
//                                           L"msOverlayScrollbarWinStyle:scrollbar_mode/full_mode,"
//                                           L"msOverlayScrollbarWinStyleAnimation");

// std::wstring wvAdditionalBrowserArgsValue(L"--enable-features="
//                                           L"OverlayScrollbar,"
//                                           L"msOverlayScrollbarWinStyle,"
//                                           L"msOverlayScrollbarWinStyleAnimation,"
//                                           L"msWebView2BrowserHitTransparent,"
//                                           L" --show-paint-rects"
//                                           L" --show-screenspace-rects"
//                                           L" --ui-show-fps-counter");

// WEBVIEW
static wil::com_ptr<ICoreWebView2Controller> wv_controller;
static wil::com_ptr<ICoreWebView2> wv_core;
static wil::com_ptr<ICoreWebView2_19> wv;
static wil::com_ptr<ICoreWebView2Settings> wv_settings;

static wil::com_ptr<ICoreWebView2Controller> wv_controller2;
static wil::com_ptr<ICoreWebView2> wv_core2;
static wil::com_ptr<ICoreWebView2_19> wv2;
static wil::com_ptr<ICoreWebView2Settings> wv_settings2;

static wil::com_ptr<ICoreWebView2Controller> wv_controller3;
static wil::com_ptr<ICoreWebView2> wv_core3;
static wil::com_ptr<ICoreWebView2_19> wv3;
static wil::com_ptr<ICoreWebView2Settings> wv_settings3;

bool CommandLineUrl();
void WebViewMessages(HWND window, PWSTR message);
void InitializeMenu(HWND window, std::filesystem::path appData);
void InitializeMainPanel(HWND window, std::filesystem::path appData);
void InitializeSidePanel(HWND window, std::filesystem::path appData);
std::wstring wvScript(
    L"document.onreadystatechange = () => {if (document.readyState === 'complete') {onkeydown = "
    L"(e) => {if (e.ctrlKey && e.key === 'w') {window.chrome.webview.postMessage('close')} "
    L"else "
    L"{window.chrome.webview.postMessage(e.key)}}}}");
std::wstring wvScriptBottom(
    L"document.onreadystatechange = () => {if (document.readyState === 'complete') {onkeydown = "
    L"(e) => {if (e.ctrlKey && e.key === 'w') {window.chrome.webview.postMessage('close')} "
    L"if (e.key === 'F3') {e.preventDefault();} "
    L"else "
    L"{console.log(e); window.chrome.webview.postMessage(e.key)}}}}");
std::wstring url1 = L"https://www.google.com/";
std::wstring url2 = L"https://www.bing.com/";
std::wstring url3 = L"about:blank";
// std::wstring url3 = L"https://localhost:8000";
