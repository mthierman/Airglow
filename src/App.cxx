#include "App.hxx"
#include "Utility.hxx"
#include "Window.hxx"
#include "Browser.hxx"

App::App(HINSTANCE hinstance, int ncs) {}

App::~App() { Gdiplus::GdiplusShutdown(gdiplusToken); }

std::unique_ptr<App> App::Create(HINSTANCE hinstance, int ncs)
{
    auto app{std::unique_ptr<App>(new App(hinstance, ncs))};

    if (GdiplusStartup(&app->gdiplusToken, &app->gdiplusStartupInput, nullptr) !=
        Gdiplus::Status::Ok)
        return nullptr;

    auto browser{Browser::Create()};

    if (!browser)
        return nullptr;

    auto window{Window::Create(hinstance, ncs)};

    if (!window)
        return nullptr;

    auto hwnd = window->hwnd;

    browser->CreateBrowsers(hwnd);

    MSG msg{};
    while (GetMessageW(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return app;
}
