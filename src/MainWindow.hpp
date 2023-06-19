class MainWindow
{
  public:
    static std::unique_ptr<MainWindow> Create(HINSTANCE, int);
    static __int64 __stdcall _WndProc(HWND, UINT, WPARAM, LPARAM);
    HWND m_hWnd;
    int _OnCommand();
    int _OnCreate();
    int _OnDestroy();
    int _OnDpiChanged();
    int _OnGetMinMaxInfo(LPARAM);
    int _OnPaint(HWND);
    int _OnSize();
    int _OnWindowPosChanged(HWND);
    int _OnSettingChange(HWND);
    int _OnKeyDown(HWND, WPARAM);

  private:
    MainWindow(HINSTANCE, int);
};
