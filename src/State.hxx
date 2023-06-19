class State
{
  public:
    static bool Toggle(bool);
    static void MaximizeWindow(HWND);
    static void FullscreenWindow(HWND);
    static void TopmostWindow(HWND);
    static void UpdateFocus();
    static void UpdateBounds(HWND);
}
