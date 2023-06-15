#include "Main.hpp"
#include "WndProc.hpp"
#include "Window.hpp"
#include "Theme.hpp"
#include "WebView.hpp"

// void TestingMain(HWND window)
// {
//
// }

int __stdcall wWinMain(HINSTANCE instance, HINSTANCE hpinstance, PWSTR pcl, int ncs)
{
    auto window = InitializeWindow(instance, ncs);
    auto appData = GetAppDataPath();
    if (appData)
    {
        InitializeWebView3(window, userData);
        InitializeWebView1(window, userData);
        InitializeWebView2(window, userData);
    }
    // TestingMain(window);
    MSG msg = {};
    while (GetMessageW(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return 0;
}
