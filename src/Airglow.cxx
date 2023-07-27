#include "App.hxx"

int __stdcall wWinMain(HINSTANCE hinstance, HINSTANCE hpinstance, PWSTR pcl, int ncs)
{


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

    return 0;
}
