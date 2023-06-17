#include "Main.hpp"
#include "WndProc.hpp"
#include "Window.hpp"
#include "WebView.hpp"
#include "Settings.hpp"

std::vector<int> GetBounds(HWND window)
{
    std::vector<int> bounds = {0, 0, 0, 0};
    RECT r;
    if (GetWindowRect(window, &r))
    {
        auto x = r.left;
        auto y = r.top;
        auto cx = r.right - r.left;
        auto cy = r.bottom - r.top;
        bounds = {x, y, cx, cy};
    }
    return bounds;
}

RECT BoundsToRect(HWND window, std::vector<int> bounds)
{
    RECT rect;
    rect.left = bounds[0];
    rect.top = bounds[1];
    rect.right = bounds[0] + bounds[2];
    rect.bottom = bounds[1] + bounds[3];
    return rect;
}

int __stdcall wWinMain(HINSTANCE instance, HINSTANCE hpinstance, PWSTR pcl, int ncs)
{
    auto window = InitializeWindow(instance, ncs);

    auto appData = GetAppDataPath();

    if (std::filesystem::exists(appData))
    {
        InitializeMenu(window, appData);
        InitializeMainPanel(window, appData);
        InitializeSidePanel(window, appData);
    }

    WindowSettings(window);

    MSG msg = {};
    while (GetMessageW(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return 0;
}
