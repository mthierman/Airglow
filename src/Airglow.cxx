#include "Airglow.hxx"

using namespace Utility;

int __stdcall wWinMain(HINSTANCE hinstance, HINSTANCE hpinstance, PWSTR pcl, int ncs)
{
    HWND hwnd;

    auto pConfig = Config::Create();

    if (!std::filesystem::exists(pConfig->pathData))
    {
        std::string err = "Data folder not found";
        error(err);
        return 0;
    }

    auto pWindow = MainWindow::Create(hinstance, ncs, pConfig.get());
    hwnd = pWindow.get()->m_hWnd;

    if (!hwnd)
    {
        std::string err = "Window creation failed";
        error(err);
        return 0;
    }

    pWindow->Show(hwnd, ncs);

    auto pWebView = WebView::Create(hwnd, pConfig.get());

    WebView::Initialize(hwnd);

#ifdef _DEBUG
#endif

    MSG msg = {};
    while (GetMessageW(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    pConfig->Save();

    return 0;
}
