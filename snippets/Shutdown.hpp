auto style = GetWindowLongPtrW(window, GWL_STYLE);
if (!(style & WS_OVERLAPPEDWINDOW))
    FullscreenWindow(window);

WINDOWPLACEMENT wp = {sizeof(WINDOWPLACEMENT)};
GetWindowPlacement(window, &wp);
auto cmd = wp.showCmd;
if (cmd == 3)
    maximized = true;
else
    maximized = false;

ShowWindow(window, SW_SHOWNORMAL);
RECT rect;
GetWindowRect(window, &rect);
dimensions = RectToBounds(rect);
