#include "App.hxx"

int __stdcall wWinMain(HINSTANCE hinstance, HINSTANCE hpinstance, PWSTR pcl, int ncs)
{

#ifdef _DEBUG
    FILE* dummyFile;
    AllocConsole();
    auto consoleHwnd{GetConsoleWindow()};
    SetConsoleTitleW(L"Debug");
    window_mica(consoleHwnd);
    // window_topmost(consoleHwnd);
    freopen_s(&dummyFile, "CONOUT$", "w", stdout);
    freopen_s(&dummyFile, "CONOUT$", "w", stderr);
    freopen_s(&dummyFile, "CONIN$", "r", stdin);
    std::cout.clear();
    std::clog.clear();
    std::cerr.clear();
    std::cin.clear();
#endif

    std::print(stdout, "TEST");

    auto app{App::Create(hinstance, ncs)};

    if (!app)
    {
        error("Application failed to start");
        return 0;
    }

    MSG msg;
    int r;
    while ((r = GetMessageW(&msg, nullptr, 0, 0)) != 0)
    {
        if (r == -1)
            error("Message loop crashed");

        else
        {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }

#ifdef _DEBUG
    fclose(dummyFile);
    FreeConsole();
#endif

    return 0;
}
