void Startup(HWND window)
{
    LoadSettings();

    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    SetWindowPos(window, nullptr, dimensions[0], dimensions[1], dimensions[2], dimensions[3], 0);
    if (maximized)
        ShowWindow(window, SW_MAXIMIZE);
}

void Shutdown(HWND window)
{
    ShowWindow(window, SW_SHOWNORMAL);
    RECT rect;
    GetWindowRect(window, &rect);
    dimensions = RectToBounds(rect);

    SaveSettings();

    Gdiplus::GdiplusShutdown(gdiplusToken);

    DestroyWindow(window);
}
