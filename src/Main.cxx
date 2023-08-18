#include "App.hxx"
#include "Utility.hxx"
#include "Storage.hxx"

int __stdcall wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    SetEnvironmentVariableW(L"WEBVIEW2_DEFAULT_BACKGROUND_COLOR", L"0");

    unsigned long long gdiplusToken{};
    Gdiplus::GdiplusStartupInput gdiplusStartupInput{};

    if (Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr) !=
        Gdiplus::Status::Ok)
        return util::error("GDI+ failed to initialize");

    auto storage{std::make_unique<Storage>()};

    if (!storage)
        return util::error("Storage failed to initialize");

    auto app{std::make_unique<App>(storage.get(), hInstance, pCmdLine, nCmdShow)};

    if (!app)
        return util::error("Application failed to start");

#ifdef _DEBUG
        // std::pair<HWND, FILE*> debugConsole{};
        // debugConsole = util::create_console(app->get_hwnd());
#endif

    MSG msg;
    int r;
    while ((r = GetMessageW(&msg, nullptr, 0, 0)) != 0)
    {
        if (r == -1)
            return 0;

        else
        {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }

#ifdef _DEBUG
    // util::remove_console(debugConsole);
#endif

    Gdiplus::GdiplusShutdown(gdiplusToken);

    return 0;
}
