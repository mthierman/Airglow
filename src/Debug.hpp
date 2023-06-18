void DebugMessages(HWND window)
{
    std::wstring dpi = L"DPI: " + std::to_wstring(GetDpiForWindow(window)) + L"\n";
    OutputDebugStringW(dpi.c_str());

    RECT client;
    GetClientRect(window, &client);
    std::wstring clientRect = L"ClientRect: " + std::to_wstring(client.right - client.left) +
                              L" x " + std::to_wstring(client.bottom - client.top) + L"\n";
    OutputDebugStringW(clientRect.c_str());

    RECT bounds;
    GetWindowRect(window, &bounds);
    std::wstring windowRect = L"WindowRect: " + std::to_wstring(bounds.right - bounds.left) +
                              L" x " + std::to_wstring(bounds.bottom - bounds.top) + L"\n";
    OutputDebugStringW(windowRect.c_str());

    OutputDebugStringW(BoolToWide(maximized).c_str());

    WINDOWPLACEMENT wp = {};
    wp.length = sizeof(WINDOWPLACEMENT);
    GetWindowPlacement(window, &wp);
    if (wp.showCmd == 3)
        OutputDebugStringW(L"MAXIMIZED!");
}
