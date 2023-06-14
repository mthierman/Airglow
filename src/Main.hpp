#include "res.hpp"

// WINDOW
unsigned short MakeWindowClass(HINSTANCE);
HWND MakeWindow(HINSTANCE);
HWND InitializeWindow(HINSTANCE, int);
std::wstring programIcon(L"PROGRAM_ICON");
std::wstring className(L"window");
std::wstring windowName(L"Gooey");
std::wstring menuName(L"menu");
RECT bounds;
bool isTopmost;
bool isFullscreen;
bool isMaximized;
bool isSplit;

// MAIN & WNDPROC
int __stdcall wWinMain(HINSTANCE, HINSTANCE, PWSTR, int);
__int64 __stdcall WndProc(HWND, UINT, WPARAM, LPARAM);

// ENV
bool CommandLineUrl();
std::wstring wvBackgroundColor(L"WEBVIEW2_DEFAULT_BACKGROUND_COLOR");
std::wstring wvBackgroundColorValue(L"0");
std::wstring wvAdditionalBrowserArgs(L"WEBVIEW2_ADDITIONAL_BROWSER_ARGUMENTS");
std::wstring wvAdditionalBrowserArgsValue(L"--enable-features="
                                          L"OverlayScrollbar,"
                                          L"msOverlayScrollbarWinStyle,"
                                          L"msOverlayScrollbarWinStyleAnimation,"
                                          L"msWebView2BrowserHitTransparent");
// std::wstring wvAdditionalBrowserArgsValue(L"--enable-features="
//                                           L"OverlayScrollbar,"
//                                           L"msOverlayScrollbarWinStyle,"
//                                           L"msOverlayScrollbarWinStyleAnimation,"
//                                           L"msWebView2BrowserHitTransparent,"
//                                           L" --show-paint-rects"
//                                           L" --show-screenspace-rects"
//                                           L" --ui-show-fps-counter");

// GDI+
unsigned long long gdiplusToken;
Gdiplus::GdiplusStartupInput gdiplusStartupInput;

// WEBVIEW
void WebViewNavigate(wil::com_ptr<ICoreWebView2>, wil::com_ptr<ICoreWebView2>);
std::wstring wvScript(
    L"document.onreadystatechange = () => {if (document.readyState === 'complete') {onkeydown = "
    L"(e) => {if (e.ctrlKey && e.key === 'w') {window.chrome.webview.postMessage('close')} if "
    L"(e.altKey && e.key === 's') {window.chrome.webview.postMessage('split')} else "
    L"{window.chrome.webview.postMessage(e.key)}}}}");
std::wstring url1 = L"about:blank";
std::wstring url2 = L"about:blank";
wil::unique_cotaskmem_string title;

// WEBVIEW
static wil::com_ptr<ICoreWebView2Controller> wv_controller;
static wil::com_ptr<ICoreWebView2> wv_core;
static wil::com_ptr<ICoreWebView2_19> wv;
static wil::com_ptr<ICoreWebView2Settings> wv_settings;

static wil::com_ptr<ICoreWebView2Controller> wv_controller2;
static wil::com_ptr<ICoreWebView2> wv_core2;
static wil::com_ptr<ICoreWebView2_19> wv2;
static wil::com_ptr<ICoreWebView2Settings> wv_settings2;

// THEMING
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

// KEYBOARD SHORTCUTS
bool KeyMaximize(HWND);
bool KeyTop(HWND);
bool KeyFullscreen(HWND);
bool KeySplit(HWND);
bool KeyClose(HWND);
