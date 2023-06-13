#include "Main.hpp"
#include "Key.hpp"
#include "Utility.hpp"
#include "Theme.hpp"
#include "WndProc.hpp"

int __stdcall wWinMain(HINSTANCE hinstance, HINSTANCE hpinstance, PWSTR pcl, int ncs)
{
    SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

    SetEnvironmentVariableW(wvBackgroundColor.c_str(), wvBackgroundColorValue.c_str());

    SetEnvironmentVariableW(wvAdditionalBrowserArgs.c_str(), wvAdditionalBrowserArgsValue.c_str());

    CommandLineUrl();

    auto atom = WindowClass(hinstance);

    if (!atom)
    {
        return 0;
    }

    auto hwnd = Window(hinstance);

    if (!hwnd)
    {
        return 0;
    }

    systemDarkMode = CheckSystemDarkMode();
    darkTitle = SetDarkTitle(hwnd);
    darkMode = SetDarkMode(hwnd);
    mica = SetMica(hwnd);
    window = SetWindow(hwnd, ncs);

    CreateCoreWebView2EnvironmentWithOptions(
        nullptr, nullptr, nullptr,
        Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [hwnd](HRESULT result, ICoreWebView2Environment* env) -> HRESULT
            {
                env->CreateCoreWebView2Controller(
                    hwnd,
                    Microsoft::WRL::Callback<
                        ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                        [hwnd](HRESULT result, ICoreWebView2Controller* controller) -> HRESULT
                        {
                            if (controller != nullptr)
                            {
                                wv_controller = controller;
                                wv_controller->get_CoreWebView2(&wv);
                            }

                            wv->get_Settings(&wv_settings);

                            wv_settings->put_AreDefaultContextMenusEnabled(true);
                            wv_settings->put_AreDefaultScriptDialogsEnabled(true);
                            wv_settings->put_AreDevToolsEnabled(true);
                            wv_settings->put_AreHostObjectsAllowed(true);
                            wv_settings->put_IsBuiltInErrorPageEnabled(true);
                            wv_settings->put_IsScriptEnabled(true);
                            wv_settings->put_IsStatusBarEnabled(true);
                            wv_settings->put_IsWebMessageEnabled(true);
                            wv_settings->put_IsZoomControlEnabled(true);

                            RECT bounds;
                            RECT wvRect;

                            GetClientRect(hwnd, &bounds);

                            wvRect = {
                                bounds.left,
                                bounds.top,
                                bounds.right / 2,
                                bounds.bottom,
                            };

                            if (!isSplit)
                                wv_controller->put_Bounds(bounds);
                            if (isSplit)
                                wv_controller->put_Bounds(wvRect);

                            wv->Navigate(url1.c_str());

                            EventRegistrationToken token;

                            wv->ExecuteScript(wvScript.c_str(), nullptr);

                            wv->AddScriptToExecuteOnDocumentCreated(wvScript.c_str(), nullptr);

                            wv->add_WebMessageReceived(
                                Microsoft::WRL::Callback<
                                    ICoreWebView2WebMessageReceivedEventHandler>(
                                    [hwnd](
                                        ICoreWebView2* webview,
                                        ICoreWebView2WebMessageReceivedEventArgs* args) -> HRESULT
                                    {
                                        wil::unique_cotaskmem_string message;

                                        args->TryGetWebMessageAsString(&message);

                                        if ((std::wstring)message.get() == keyTop.c_str())
                                        {
                                            KeyTop(hwnd);
                                        }

                                        if ((std::wstring)message.get() == keySplit.c_str())
                                        {
                                            KeySplit(hwnd);
                                        }

                                        if ((std::wstring)message.get() == keyMax.c_str())
                                        {
                                            KeyMaximize(hwnd);
                                        }

                                        if ((std::wstring)message.get() == keyFull.c_str())
                                        {
                                            KeyFullscreen(hwnd);
                                        }

                                        if ((std::wstring)message.get() == keyClose.c_str())
                                        {
                                            KeyClose(hwnd);
                                        }

                                        webview->PostWebMessageAsString(message.get());

                                        return S_OK;
                                    })
                                    .Get(),
                                &token);
                            return S_OK;
                        })
                        .Get());
                return S_OK;
            })
            .Get());

    CreateCoreWebView2EnvironmentWithOptions(
        nullptr, nullptr, nullptr,
        Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [hwnd](HRESULT result, ICoreWebView2Environment* env) -> HRESULT
            {
                env->CreateCoreWebView2Controller(
                    hwnd,
                    Microsoft::WRL::Callback<
                        ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                        [hwnd](HRESULT result, ICoreWebView2Controller* controller) -> HRESULT
                        {
                            if (controller != nullptr)
                            {
                                wv_controller2 = controller;
                                wv_controller2->get_CoreWebView2(&wv2);
                            }

                            wv2->get_Settings(&wv_settings2);

                            wv_settings2->put_AreDefaultContextMenusEnabled(true);
                            wv_settings2->put_AreDefaultScriptDialogsEnabled(true);
                            wv_settings2->put_AreDevToolsEnabled(true);
                            wv_settings2->put_AreHostObjectsAllowed(true);
                            wv_settings2->put_IsBuiltInErrorPageEnabled(true);
                            wv_settings2->put_IsScriptEnabled(true);
                            wv_settings2->put_IsStatusBarEnabled(true);
                            wv_settings2->put_IsWebMessageEnabled(true);
                            wv_settings2->put_IsZoomControlEnabled(true);

                            RECT bounds2;
                            RECT wvRect2;
                            RECT boundsDisabled;

                            GetClientRect(hwnd, &bounds2);

                            wvRect2 = {
                                bounds2.right / 2,
                                bounds2.top,
                                bounds2.right,
                                bounds2.bottom,
                            };

                            if (isSplit)
                                wv_controller2->put_Bounds(wvRect2);

                            wv2->Navigate(url2.c_str());

                            EventRegistrationToken token;

                            wv2->ExecuteScript(wvScript.c_str(), nullptr);

                            wv2->AddScriptToExecuteOnDocumentCreated(wvScript.c_str(), nullptr);

                            wv2->add_WebMessageReceived(
                                Microsoft::WRL::Callback<
                                    ICoreWebView2WebMessageReceivedEventHandler>(
                                    [hwnd](
                                        ICoreWebView2* webview,
                                        ICoreWebView2WebMessageReceivedEventArgs* args) -> HRESULT
                                    {
                                        wil::unique_cotaskmem_string message;

                                        args->TryGetWebMessageAsString(&message);

                                        if ((std::wstring)message.get() == keyTop.c_str())
                                        {
                                            KeyTop(hwnd);
                                        }

                                        if ((std::wstring)message.get() == keySplit.c_str())
                                        {
                                            KeySplit(hwnd);
                                        }

                                        if ((std::wstring)message.get() == keyMax.c_str())
                                        {
                                            KeyMaximize(hwnd);
                                        }

                                        if ((std::wstring)message.get() == keyFull.c_str())
                                        {
                                            KeyFullscreen(hwnd);
                                        }

                                        if ((std::wstring)message.get() == keyClose.c_str())
                                        {
                                            KeyClose(hwnd);
                                        }

                                        webview->PostWebMessageAsString(message.get());

                                        return S_OK;
                                    })
                                    .Get(),
                                &token);
                            return S_OK;
                        })
                        .Get());
                return S_OK;
            })
            .Get());

    // auto menuHandle = GetSystemMenu(hwnd, false);
    // MENUITEMINFOW seperator;
    // seperator.cbSize = sizeof(MENUITEMINFOW);
    // seperator.fMask = MIIM_FTYPE;
    // seperator.fType = MFT_SEPARATOR;
    // MENUITEMINFOW split;
    // split.cbSize = sizeof(MENUITEMINFOW);
    // split.fMask = MIIM_STRING | MIIM_ID;
    // split.wID = 1005;
    // split.dwTypeData = (LPWSTR)L"Toggle Split";

    // InsertMenuItemW(menuHandle, 1, FALSE, &seperator);
    // InsertMenuItemW(menuHandle, 1, FALSE, &split);

    MSG msg = {};
    while (GetMessageW(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return 0;
}

unsigned short WindowClass(HINSTANCE instance)
{
    WNDCLASSEXW wcex = {};
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = instance;
    wcex.hCursor = (HCURSOR)LoadImageW(nullptr, (LPCWSTR)IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = menuName.c_str();
    wcex.lpszClassName = className.c_str();
    wcex.hIcon = (HICON)LoadImageW(instance, programIcon.c_str(), IMAGE_ICON, 0, 0,
                                   LR_DEFAULTCOLOR | LR_DEFAULTSIZE | LR_SHARED);
    wcex.hIconSm = (HICON)LoadImageW(instance, programIcon.c_str(), IMAGE_ICON, 0, 0,
                                     LR_DEFAULTCOLOR | LR_DEFAULTSIZE | LR_SHARED);

    return RegisterClassExW(&wcex);
}

HWND Window(HINSTANCE hinstance)
{
    return CreateWindowExW(0, className.c_str(), windowName.c_str(), WS_OVERLAPPEDWINDOW,
                           CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr,
                           nullptr, hinstance, nullptr);
}
