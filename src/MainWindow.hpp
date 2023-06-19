class MainWindow
{
  public:
    static std::unique_ptr<MainWindow> Create(HINSTANCE, int);
    static __int64 __stdcall _WndProc(HWND, UINT, WPARAM, LPARAM);
    static bool _ShowWindow(HWND, int);
    HWND m_hWnd;

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
