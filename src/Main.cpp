#include "Main.hpp"
#include "WndProc.hpp"
#include "Window.hpp"
#include "Utility.hpp"
#include "Theme.hpp"
#include "Key.hpp"

int __stdcall wWinMain(HINSTANCE instance, HINSTANCE hpinstance, PWSTR pcl, int ncs)
{
    SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
    SetEnvironmentVariableW(wvBackgroundColor.c_str(), wvBackgroundColorValue.c_str());
    SetEnvironmentVariableW(wvAdditionalBrowserArgs.c_str(), wvAdditionalBrowserArgsValue.c_str());
    auto args = CommandLineUrl();
    auto title = url1 + L" | " + url2;
    auto atom = MakeWindowClass(instance);
    if (!atom)
    {
        return 0;
    }
    auto window = MakeWindow(instance);
    if (!window)
    {
        return 0;
    }
    SetWindowTextW(window, title.c_str());
    SetDarkTitle();
    SetDarkMode(window);
    SetMica(window);
    SetWindow(window, ncs);
    GetClientRect(window, &bounds);
    CreateCoreWebView2EnvironmentWithOptions(
        nullptr, nullptr, nullptr,
        Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [window](HRESULT result, ICoreWebView2Environment* env) -> HRESULT
            {
                env->CreateCoreWebView2Controller(
                    window,
                    Microsoft::WRL::Callback<
                        ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                        [window](HRESULT result, ICoreWebView2Controller* controller) -> HRESULT
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
                            RECT wv_bounds = {
                                bounds.left,
                                bounds.top,
                                bounds.right / 2,
                                bounds.bottom,
                            };
                            if (!isSplit)
                                wv_controller->put_Bounds(bounds);
                            if (isSplit)
                                wv_controller->put_Bounds(wv_bounds);
                            wv->Navigate(url1.c_str());
                            EventRegistrationToken token;
                            wv->ExecuteScript(wvScript.c_str(), nullptr);
                            wv->AddScriptToExecuteOnDocumentCreated(wvScript.c_str(), nullptr);
                            wv->add_WebMessageReceived(
                                Microsoft::WRL::Callback<
                                    ICoreWebView2WebMessageReceivedEventHandler>(
                                    [window](
                                        ICoreWebView2* webview,
                                        ICoreWebView2WebMessageReceivedEventArgs* args) -> HRESULT
                                    {
                                        wil::unique_cotaskmem_string message;
                                        args->TryGetWebMessageAsString(&message);
                                        if ((std::wstring)message.get() ==
                                            std::wstring(L"F1").c_str())
                                        {
                                            isMaximized = KeyMaximize(window);
                                        }
                                        if ((std::wstring)message.get() ==
                                            std::wstring(L"F2").c_str())
                                        {
                                            isTopmost = KeyTop(window);
                                        }
                                        if ((std::wstring)message.get() ==
                                            std::wstring(L"F11").c_str())
                                        {
                                            isFullscreen = KeyFullscreen(window);
                                        }
                                        if ((std::wstring)message.get() ==
                                            std::wstring(L"split").c_str())
                                        {
                                            isSplit = KeySplit(window);
                                        }
                                        if ((std::wstring)message.get() ==
                                            std::wstring(L"close").c_str())
                                        {
                                            KeyClose(window);
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
            [window](HRESULT result, ICoreWebView2Environment* env) -> HRESULT
            {
                env->CreateCoreWebView2Controller(
                    window,
                    Microsoft::WRL::Callback<
                        ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                        [window](HRESULT result, ICoreWebView2Controller* controller) -> HRESULT
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
                            RECT wv_bounds2 = {
                                bounds.right / 2,
                                bounds.top,
                                bounds.right,
                                bounds.bottom,
                            };
                            if (isSplit)
                                wv_controller2->put_Bounds(wv_bounds2);
                            wv2->Navigate(url2.c_str());
                            EventRegistrationToken token;
                            wv2->ExecuteScript(wvScript.c_str(), nullptr);
                            wv2->AddScriptToExecuteOnDocumentCreated(wvScript.c_str(), nullptr);
                            wv2->add_WebMessageReceived(
                                Microsoft::WRL::Callback<
                                    ICoreWebView2WebMessageReceivedEventHandler>(
                                    [window](
                                        ICoreWebView2* webview,
                                        ICoreWebView2WebMessageReceivedEventArgs* args) -> HRESULT
                                    {
                                        wil::unique_cotaskmem_string message;
                                        args->TryGetWebMessageAsString(&message);
                                        if ((std::wstring)message.get() ==
                                            std::wstring(L"F1").c_str())
                                        {
                                            isMaximized = KeyMaximize(window);
                                        }
                                        if ((std::wstring)message.get() ==
                                            std::wstring(L"F2").c_str())
                                        {
                                            isTopmost = KeyTop(window);
                                        }
                                        if ((std::wstring)message.get() ==
                                            std::wstring(L"F11").c_str())
                                        {
                                            isFullscreen = KeyFullscreen(window);
                                        }
                                        if ((std::wstring)message.get() ==
                                            std::wstring(L"split").c_str())
                                        {
                                            isSplit = KeySplit(window);
                                        }
                                        if ((std::wstring)message.get() ==
                                            std::wstring(L"close").c_str())
                                        {
                                            KeyClose(window);
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
    MSG msg = {};
    while (GetMessageW(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return 0;
}
