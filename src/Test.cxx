#include "Config.hxx"

void Test(HWND hwnd)
{
    auto toggleTest = bool_toggle(false);
    if (toggleTest != false)
        println("Toggle(): TEST FAILED");

    wstring dpi = L"DPI: " + std::to_wstring(GetDpiForWindow(hwnd));
    wprintln(dpi);

    RECT clientRect;
    GetClientRect(hwnd, &clientRect);
    wstring clientBounds = L"Client: " + std::to_wstring(clientRect.right - clientRect.left) +
                           L" x " + std::to_wstring(clientRect.bottom - clientRect.top);
    wprintln(clientBounds.c_str());

    RECT windowRect;
    GetWindowRect(hwnd, &windowRect);
    wstring windowBounds = L"Window: " + std::to_wstring(windowRect.right - windowRect.left) +
                           L" x " + std::to_wstring(windowRect.bottom - windowRect.top);
    wprintln(windowBounds.c_str());

    WINDOWPLACEMENT wp;
    wp.length = sizeof(WINDOWPLACEMENT);
    GetWindowPlacement(hwnd, &wp);
    if (wp.showCmd == 3)
        println("Window is maximized");

    auto acp = GetACP();
    wprint(std::to_wstring(acp));
}
