#include "res.hpp"

// MAIN
ATOM WindowClass(HINSTANCE);
HWND Window(HINSTANCE);
int APIENTRY wWinMain(HINSTANCE, HINSTANCE, PWSTR, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// ENV
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

// WINDOW
std::wstring programIcon(L"PROGRAM_ICON");
std::wstring className(L"window");
std::wstring windowName(L"Gooey");
std::wstring menuName(L"menu");

bool systemDarkMode;
bool darkTitle;
bool darkMode;
bool mica;
bool window;

// WEBVIEW1
static wil::com_ptr<ICoreWebView2Controller> wv_controller;
static wil::com_ptr<ICoreWebView2> wv;
static wil::com_ptr<ICoreWebView2Settings> wv_settings;

// WEBVIEW2
static wil::com_ptr<ICoreWebView2Controller> wv_controller2;
static wil::com_ptr<ICoreWebView2> wv2;
static wil::com_ptr<ICoreWebView2Settings> wv_settings2;

// WEBVIEW
std::wstring wvScript(
    L"document.onreadystatechange = () => {if (document.readyState === 'complete') {onkeydown = "
    L"(e) => {if (e.ctrlKey && e.key === 'w') {window.chrome.webview.postMessage('close')} if "
    L"(e.ctrlKey && e.key === 's') {window.chrome.webview.postMessage('split')} else "
    L"{window.chrome.webview.postMessage(e.key)}}}}");
void WebViewNavigate(wil::com_ptr<ICoreWebView2>, wil::com_ptr<ICoreWebView2>);
void CommandLineUrl();
std::wstring url1 = L"about:blank";
std::wstring url2 = L"about:blank";

// THEMING
enum PreferredAppMode
{
    Default,
    AllowDark,
    ForceDark,
    ForceLight,
    Max
};

bool CheckSystemDarkMode();
bool SetDarkTitle(HWND);
bool SetDarkMode(HWND);
bool SetMica(HWND);
bool SetWindow(HWND, int);

// KEYBOARD SHORTCUTS
int vkKeyTop = VK_F1;
int vkKeyMax = VK_F10;
int vkKeyFull = VK_F11;
int vkKeyControl = VK_CONTROL;
int vkKeyS = 0x53;
int vkKeyE = 0x45;
int vkKeyW = 0x57;

// int vkKeyAlt = VK_MENU;
// int vkKeyF4 = VK_F4;

std::wstring keyTop(L"F1");
std::wstring keySplit(L"split");
std::wstring keyMax(L"F10");
std::wstring keyFull(L"F11");
std::wstring keyClose(L"close");

void KeyTop(HWND);
void KeySplit(HWND);
void KeyMaximize(HWND);
void KeyFullscreen(HWND);
void KeyClose(HWND);

bool isTopmost;
bool isFullscreen;
bool isMaximized;
bool isSplit = false;

RECT windowSize;
