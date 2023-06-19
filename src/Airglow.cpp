#include "Airglow.hpp"
#include "MainWindow.hpp"

int __stdcall wWinMain(HINSTANCE hinstance, HINSTANCE hpinstance, PWSTR pcl, int ncs)
{
    auto window = MainWindow::Create(hinstance, ncs);

    if (!window)
        return 0;

    if (window)
        hwnd = window.get()->m_hWnd;

    if (hwnd)
        ShowWindow(hwnd, SW_SHOWNORMAL);

    else
        return 0;

    MSG msg = {};
    while (GetMessageW(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return 0;
}
