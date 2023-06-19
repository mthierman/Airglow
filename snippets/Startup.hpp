SetWindowPos(window, nullptr, dimensions[0], dimensions[1], dimensions[2], dimensions[3], 0);
if (maximized)
    ShowWindow(window, SW_SHOWMAXIMIZED);
if (fullscreen)
    FullscreenWindow(window);
