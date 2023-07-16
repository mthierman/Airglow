#include "App.hxx"
#include "Utility.hxx"
#include "Window.hxx"
#include "Browser.hxx"

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

    MSG msg{};
    while (GetMessageW(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return app;
}

App::~App() { GdiplusShutdown(gdiplusToken); }

App::App(HINSTANCE hinstance, int ncs) {}
