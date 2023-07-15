#include "App.hxx"

int __stdcall wWinMain(HINSTANCE hinstance, HINSTANCE hpinstance, PWSTR pcl, int ncs)
{
    auto app = App::Create(hinstance, ncs);

    app->Close();

    return 0;
}
