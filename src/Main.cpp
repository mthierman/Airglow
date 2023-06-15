#include "Main.hpp"
#include "WndProc.hpp"
#include "Window.hpp"
#include "Theme.hpp"
#include "Key.hpp"
#include "WebView.hpp"

int __stdcall wWinMain(HINSTANCE instance, HINSTANCE hpinstance, PWSTR pcl, int ncs)
{
    auto window = InitializeWindow(instance, ncs);

    auto appData = GetAppDataPath();

    auto dpi = GetDpiForWindow(window);

    auto test = std::to_wstring(dpi);

    OutputDebugStringW(L"\nDPI: ");
    OutputDebugStringW(test.c_str());

    if (appData)
    {
        InitializeWebView3(window, userData);
        InitializeWebView1(window, userData);
        InitializeWebView2(window, userData);
    }

    MSG msg = {};

    while (GetMessageW(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return 0;
}
