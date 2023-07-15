#include "App.hxx"
#include "Config.hxx"
#include "MainWindow.hxx"

std::unique_ptr<App> App::Create(HINSTANCE hinstance, int ncs)
{
    auto app{std::unique_ptr<App>(new App(hinstance, ncs))};

    if (GdiplusStartup(&app->gdiplusToken, &app->gdiplusStartupInput, nullptr) != Status::Ok)
    {
        error("GDI+ initialization failed");
        return nullptr;
    }

    auto config{Config::Create()};
    if (!config)
    {
        error("Configuration failed");
        return nullptr;
    }

    auto window{MainWindow::Create(hinstance, ncs, config.get())};
    if (!window)
    {
        error("Window creation failed");
        return 0;
    }

    return app;
}

App::~App() { GdiplusShutdown(gdiplusToken); }

App::App(HINSTANCE hinstance, int ncs) {}
