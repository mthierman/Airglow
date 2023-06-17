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

    auto settingsFile = GetSettingsFilePath();
    nlohmann::json settings = LoadSettings(settingsFile);

    dimensions = settings["dimensions"].get<std::vector<int>>();
    // RECT test = {dimensions[0], dimensions[1], dimensions[2], dimensions[3]};
    // auto windowSize = BoundsToRect(window, test);
    // auto windowPos = BoundsToRect(window, dimensions);

    fullscreen = settings["fullscreen"].get<bool>();
    maximized = settings["maximized"].get<bool>();
    menu = settings["menu"].get<bool>();
    ontop = settings["ontop"].get<bool>();
    split = settings["split"].get<bool>();
    swapped = settings["swapped"].get<bool>();
    if (fullscreen)
        WindowFullscreen(window);
    if (maximized)
        WindowMaximize(window);
    if (ontop)
        WindowTop(window);
    SetWindowPos(window, nullptr, dimensions[0], dimensions[1], dimensions[2], dimensions[3], 0);

    MSG msg = {};
    while (GetMessageW(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return 0;
}
