#include "Main.hpp"
#include "WndProc.hpp"
#include "Window.hpp"
#include "Utility.hpp"
#include "Theme.hpp"
#include "Key.hpp"
#include "WebView.hpp"

int __stdcall wWinMain(HINSTANCE instance, HINSTANCE hpinstance, PWSTR pcl, int ncs)
{
    auto window = InitializeWindow(instance, ncs);

    InitializeWebView1(window);
    InitializeWebView2(window);

    MSG msg = {};
    while (GetMessageW(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return 0;
}
