#define PROGRAM_ICON 1

// MAIN
int __stdcall wWinMain(HINSTANCE, HINSTANCE, PWSTR, int);
void Startup();
void Shutdown();

// MESSAGES
__int64 __stdcall WndProc(HWND, UINT, WPARAM, LPARAM);
void Messages(std::wstring);

// WINDOW
HWND window;
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
void UpdateFocus();
void UpdateBounds(HWND);
void DebugMessages(HWND);
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
static unsigned long long gdiplusToken;
static Gdiplus::GdiplusStartupInput gdiplusStartupInput;

// WEBVIEW
void InitializeWebViews(HWND, std::filesystem::path);
std::wstring GetScriptFile(std::filesystem::path);
std::wstring GetScript(std::filesystem::path);
std::wstring GetMenuScript(std::filesystem::path);
void SetWindowTitle(HWND);
void SetWindowIcon(HWND);
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
