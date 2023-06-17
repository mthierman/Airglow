#define PROGRAM_ICON 1

// ENV
std::wstring programIcon(L"PROGRAM_ICON");
std::wstring className(L"window");
std::wstring windowName(L"Airglow");
std::wstring menuName(L"menu");
std::wstring wvBackgroundColor(L"WEBVIEW2_DEFAULT_BACKGROUND_COLOR");
std::wstring wvBackgroundColorValue(L"0");
std::wstring wvAdditionalBrowserArgs(L"WEBVIEW2_ADDITIONAL_BROWSER_ARGUMENTS");
// std::wstring wvAdditionalBrowserArgsValue(L"--enable-features="
//                                           L"msWebView2BrowserHitTransparent");
std::wstring wvAdditionalBrowserArgsValue(L"");
// std::wstring wvScript(
//     L"document.onreadystatechange = () => {if (document.readyState === 'complete') {onkeydown = "
//     L"(e) => {if (e.ctrlKey && e.key === 'w') {window.chrome.webview.postMessage('close')} "
//     L"else "
//     L"{window.chrome.webview.postMessage(e.key)}}}}");

std::wstring script;

// std::wstring
//     wvScript(L"document.onreadystatechange = () => {if (document.readyState === 'interactive') "
//              L"{let scheme = document.createElement(\"meta\");"
//              L"scheme.setAttribute(\"name\", \"color-scheme\");"
//              L"scheme.setAttribute(\"content\", \"light dark\");"
//              L"document.getElementsByTagName(\"head\")[0].appendChild(scheme);"
//              L"document.documentElement.style.setProperty(\"color-scheme\", \"light dark\");}}");

// std::wstring wvScriptBottom(
//     L"document.onreadystatechange = () => {if (document.readyState === 'complete') {onkeydown "
//     L"= "
//     L"(e) => {if (e.ctrlKey && e.key === 'w') {window.chrome.webview.postMessage('close')} "
//     L"if (e.key === 'F3') {e.preventDefault();} "
//     L"else "
//     L"{console.log(e); window.chrome.webview.postMessage(e.key)}}}}");

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
void SetFocus();
std::vector<int> GetBounds(HWND window);
RECT BoundsToRect(HWND window, std::vector<int> bounds);
void Startup(HWND window);
void Shutdown(HWND window);
enum PreferredAppMode
{
    Default,
    AllowDark,
    ForceDark,
    ForceLight,
    Max
};
std::vector<int> dimensions;
bool ontop;
bool maximized;
bool fullscreen;
bool split;
bool swapped;
bool menu;
std::wstring mainpage;
std::wstring sidepage;

// GDI+
unsigned long long gdiplusToken;
Gdiplus::GdiplusStartupInput gdiplusStartupInput;

// SETTINGS
std::filesystem::path GetAppDataPath();
std::filesystem::path GetSettingsFilePath(std::filesystem::path appData);
std::wstring GetScriptFile(std::filesystem::path appData);
nlohmann::json DefaultSettings();
nlohmann::json CurrentSettings(HWND window);
nlohmann::json LoadSettings(std::filesystem::path settingsFile);
void SaveSettings(nlohmann::json input, std::filesystem::path settingsFile);
std::filesystem::path appData;

// WEBVIEW
void InitializeMenu(HWND window, std::filesystem::path appData);
void InitializeMainPanel(HWND window, std::filesystem::path appData);
void InitializeSidePanel(HWND window, std::filesystem::path appData);
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

// UTILITY
std::wstring ToWide(std::string input);
std::string ToString(std::wstring input);
std::wstring BoolToWide(bool input);
std::pair<std::wstring, std::wstring> CommandLine();
void WebViewMessages(HWND window, PWSTR message);
