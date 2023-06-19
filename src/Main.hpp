#define PROGRAM_ICON 1

// MAIN
int __stdcall wWinMain(HINSTANCE, HINSTANCE, PWSTR, int);
void Startup();
void Shutdown();

// MESSAGES
__int64 __stdcall WndProc(HWND, UINT, WPARAM, LPARAM);
void Messages(std::wstring);

// WINDOW
class MainWindow
{
  public:
    static std::unique_ptr<MainWindow> Create(HINSTANCE, int);
    static __int64 __stdcall _WndProc(HWND, UINT, WPARAM, LPARAM);
    void InitializeWindow(HINSTANCE, int);
    HWND m_hWnd;
    int _OnPaint(HWND);

  private:
    MainWindow(HINSTANCE, int);
};

// STATE
bool Toggle(bool);
void MaximizeWindow(HWND);
void FullscreenWindow(HWND);
void TopmostWindow(HWND);
void UpdateFocus();
void UpdateBounds(HWND);
std::vector<int> dimensions;
bool menu;
bool split;
bool swapped;
bool maximized;
bool fullscreen;
bool topmost;
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
std::vector<int> RectToBounds(RECT);
RECT BoundsToRect(std::vector<int>);
RECT GetMenuBounds(HWND);
RECT GetMainPanelBounds(HWND);
RECT GetSidePanelBounds(HWND);
void prints(std::string);
void printw(std::wstring);
std::wstring ToWide(std::string);
std::string ToString(std::wstring);
std::wstring BoolToWide(bool);
std::pair<std::wstring, std::wstring> CommandLine();

// DEBUG
void Tests();
