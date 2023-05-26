// WEBVIEW
static wil::com_ptr<ICoreWebView2Controller> wv_controller;
static wil::com_ptr<ICoreWebView2> wv;
static wil::com_ptr<ICoreWebView2Settings> wv_settings;
void WebViewNavigate(wil::com_ptr<ICoreWebView2> wv);

// WINDOWS
int APIENTRY wWinMain(HINSTANCE histance, HINSTANCE hprevinstance, PWSTR pcmdline, int ncmdshow);
LRESULT CALLBACK WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);

// THEMING
HBRUSH hollowBrush;
HBRUSH darkBrush;
HBRUSH lightBrush;
int lightMode;
int ModeCheck();
void DarkTitle();
void DarkMode(HWND hwnd);
void SetMica(HWND hwnd);
void ExtendFrame(HWND hwnd);

// KEYBOARD SHORTCUTS
void KeyTop(HWND hwnd);
void KeyMaximize(HWND hwnd);
void KeyFullscreen(HWND hwnd);
