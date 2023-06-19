class MainWindow
{
  public:
    static std::unique_ptr<MainWindow> Create(HINSTANCE, int);
    static __int64 __stdcall _WndProc(HWND, UINT, WPARAM, LPARAM);
    static bool _ShowWindow(HWND, int);
    HWND m_hWnd;
    static unsigned long long gdiplusToken;
    static Gdiplus::GdiplusStartupInput gdiplusStartupInput;

    // MESSAGES
    int _OnCommand();
    int _OnCreate(HWND);
    int _OnDestroy();
    int _OnDpiChanged();
    int _OnGetMinMaxInfo(LPARAM);
    int _OnPaint(HWND);
    int _OnSize();
    int _OnWindowPosChanged(HWND);
    int _OnSettingChange(HWND);
    int _OnKeyDown(HWND, WPARAM);

    bool CheckSystemDarkMode();
    bool SetDarkTitle();
    bool SetDarkMode(HWND);
    bool SetMica(HWND);

    bool Toggle(bool);
    void MaximizeWindow(HWND);
    void FullscreenWindow(HWND);
    void TopmostWindow(HWND);
    void UpdateFocus();
    void UpdateBounds(HWND);

  private:
    MainWindow(HINSTANCE, int);
    enum PreferredAppMode
    {
        Default,
        AllowDark,
        ForceDark,
        ForceLight,
        Max
    };
};
