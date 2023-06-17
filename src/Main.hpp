#define PROGRAM_ICON 1

// ENV
std::wstring programIcon(L"PROGRAM_ICON");
std::wstring className(L"window");
std::wstring windowName(L"Airglow");
std::wstring menuName(L"menu");
std::wstring wvBackgroundColor(L"WEBVIEW2_DEFAULT_BACKGROUND_COLOR");
std::wstring wvBackgroundColorValue(L"0");
std::wstring script;
std::wstring menuScript;

// MAIN
int __stdcall wWinMain(HINSTANCE, HINSTANCE, PWSTR, int);
__int64 __stdcall WndProc(HWND, UINT, WPARAM, LPARAM);
void WebViewMessages(HWND, std::wstring);

// APP
void Startup(HWND);
void Shutdown(HWND);

// WINDOW
unsigned short MakeWindowClass(HINSTANCE);
HWND MakeWindow(HINSTANCE);
HWND InitializeWindow(HINSTANCE, int);
std::vector<int> RectToBounds(RECT);
RECT BoundsToRect(std::vector<int>);
RECT GetMenuBounds(HWND);
RECT GetMainPanelBounds(HWND);
RECT GetSidePanelBounds(HWND);
bool HideMenu();
bool SplitPanel();
bool SwapPanel();
bool WindowMaximize(HWND);
bool WindowFullscreen(HWND);
bool WindowTop(HWND);
bool CheckSystemDarkMode();
bool SetDarkTitle();
bool SetDarkMode(HWND);
bool SetMica(HWND);
bool SetWindow(HWND, int);
void SetWindowFocus(HWND);
void TestingResize(HWND);
void WindowResizing(HWND);
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

// WEBVIEW
void InitializeMenu(HWND, std::filesystem::path);
void InitializeMainPanel(HWND, std::filesystem::path);
void InitializeSidePanel(HWND, std::filesystem::path);
std::wstring GetScriptFile(std::filesystem::path);
std::wstring GetScript(std::filesystem::path);
std::wstring GetMenuScript(std::filesystem::path);
void SetWindowTitle(HWND);
void SetWindowIcon(HWND);
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

// SETTINGS
std::filesystem::path GetAppDataPath();
std::filesystem::path GetSettingsFilePath(std::filesystem::path);
nlohmann::json DefaultSettings();
nlohmann::json CurrentSettings();
nlohmann::json LoadSettings();
void SaveSettings();
std::filesystem::path appData;
std::filesystem::path settingsFile;

// UTILITY
std::wstring ToWide(std::string);
std::string ToString(std::wstring);
std::wstring BoolToWide(bool);
std::pair<std::wstring, std::wstring> CommandLine();
