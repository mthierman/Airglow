#include "App.hxx"
#include "Browser.hxx"
#include "Utility.hxx"
#include "Window.hxx"

using namespace Utility;
using namespace Gdiplus;

std::unique_ptr<App> App::Create(HINSTANCE hinstance, int ncs)
{
    auto app{std::unique_ptr<App>(new App(hinstance, ncs))};

    if (GdiplusStartup(&app->gdiplusToken, &app->gdiplusStartupInput, nullptr) != Status::Ok)
    {
        error("GDI+ initialization failed");
        return nullptr;
    }

    auto window{Window::Create(hinstance, ncs)};

    if (!window)
        return nullptr;

    window->Show();

    auto browser{Browser::Create(window->hwnd)};
    if (!browser)
    {
        error("WebView2 creation failed");
        return nullptr;
    }

    while (GetMessageW(&app->msg, nullptr, 0, 0))
    {
        TranslateMessage(&app->msg);
        DispatchMessageW(&app->msg);
    }

    return app;
}

App::~App() { GdiplusShutdown(gdiplusToken); }

App::App(HINSTANCE hinstance, int ncs) {}
