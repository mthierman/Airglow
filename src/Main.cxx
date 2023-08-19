#include "App.hxx"
#include "Utility.hxx"
#include "Storage.hxx"

int __stdcall wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    auto storage{std::make_unique<Storage>()};

    if (!storage)
        return util::error("Storage failed to initialize");

    auto app{std::make_unique<App>(storage.get(), hInstance, pCmdLine, nCmdShow)};

    if (!app)
        return util::error("Application failed to start");

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

    return 0;
}
