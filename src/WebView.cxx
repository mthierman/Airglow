#include "WebView.hxx"

Settings* WebView::pSettings = nullptr;

WebView::WebView(HWND hwnd, Settings* settings) {}

std::unique_ptr<WebView> WebView::Create(HWND hwnd, Settings* settings)
{
    pSettings = settings;

    auto pWebView = std::unique_ptr<WebView>(new WebView(hwnd, settings));

    return pWebView;
}

void WebView::Initialize(HWND hwnd)
{
    CreateCoreWebView2EnvironmentWithOptions(
        nullptr, pSettings->pathData.c_str(), nullptr,
        Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [hwnd](HRESULT result, ICoreWebView2Environment* env) -> HRESULT
            {
                // SETTINGS WEBVIEW
                env->CreateCoreWebView2Controller(
                    hwnd, Microsoft::WRL::Callback<
                              ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                              [hwnd](HRESULT result, ICoreWebView2Controller* controller) -> HRESULT
                              {
                                  EventRegistrationToken msgToken;

                                  if (controller != nullptr)
                                  {
                                      settings_controller = controller;
                                      settings_controller->get_CoreWebView2(&settings_core);
                                  }

                                  settings_wv = settings_core.try_query<ICoreWebView2_19>();
                                  settings_wv->get_Settings(&settings_settings);
                                  settings_settings->put_AreDefaultContextMenusEnabled(false);
                                  settings_settings->put_AreDefaultScriptDialogsEnabled(true);
                                  settings_settings->put_AreDevToolsEnabled(true);
                                  settings_settings->put_AreHostObjectsAllowed(true);
                                  settings_settings->put_IsBuiltInErrorPageEnabled(true);
                                  settings_settings->put_IsScriptEnabled(true);
                                  settings_settings->put_IsStatusBarEnabled(false);
                                  settings_settings->put_IsWebMessageEnabled(true);
                                  settings_settings->put_IsZoomControlEnabled(false);
                                  settings_controller->put_Bounds(MenuBounds(hwnd));

                                  settings_wv->Navigate(L"https://www.example.com/");

                                  auto script = GetMenuScript();
                                  settings_wv->ExecuteScript(script.c_str(), nullptr);
                                  settings_wv->AddScriptToExecuteOnDocumentCreated(script.c_str(),
                                                                                   nullptr);

                                  settings_wv->add_WebMessageReceived(
                                      Microsoft::WRL::Callback<
                                          ICoreWebView2WebMessageReceivedEventHandler>(
                                          [hwnd](ICoreWebView2* webview,
                                                 ICoreWebView2WebMessageReceivedEventArgs* args)
                                              -> HRESULT
                                          {
                                              wil::unique_cotaskmem_string message;
                                              args->TryGetWebMessageAsString(&message);
                                              auto msg = std::wstring(message.get());
                                              Messages(msg);
                                              webview->PostWebMessageAsString(message.get());
                                              return S_OK;
                                          })
                                          .Get(),
                                      &msgToken);

                                  return S_OK;
                              })
                              .Get());

                // MAIN WEBVIEW
                env->CreateCoreWebView2Controller(
                    hwnd,
                    Microsoft::WRL::Callback<
                        ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                        [hwnd](HRESULT result, ICoreWebView2Controller* controller) -> HRESULT
                        {
                            EventRegistrationToken msgToken;
                            EventRegistrationToken faviconChangedToken;
                            EventRegistrationToken documentTitleChangedToken;

                            if (controller != nullptr)
                            {
                                main_controller = controller;
                                main_controller->get_CoreWebView2(&main_core);
                            }

                            main_wv = main_core.try_query<ICoreWebView2_19>();
                            main_wv->get_Settings(&main_settings);
                            main_settings->put_AreDefaultContextMenusEnabled(true);
                            main_settings->put_AreDefaultScriptDialogsEnabled(true);
                            main_settings->put_AreDevToolsEnabled(true);
                            main_settings->put_AreHostObjectsAllowed(true);
                            main_settings->put_IsBuiltInErrorPageEnabled(true);
                            main_settings->put_IsScriptEnabled(true);
                            main_settings->put_IsStatusBarEnabled(true);
                            main_settings->put_IsWebMessageEnabled(true);
                            main_settings->put_IsZoomControlEnabled(true);
                            main_controller->put_Bounds(MainBounds(hwnd));

                            auto args = Utility::CommandLine();

                            if (!args.first.empty())
                            {
                                main_wv->Navigate(args.first.c_str());
                            }

                            settings_wv->Navigate(L"https://www.google.com/");

                            auto script = GetMenuScript();
                            main_wv->ExecuteScript(script.c_str(), nullptr);
                            main_wv->AddScriptToExecuteOnDocumentCreated(script.c_str(), nullptr);

                            main_wv->add_DocumentTitleChanged(
                                Microsoft::WRL::Callback<
                                    ICoreWebView2DocumentTitleChangedEventHandler>(
                                    [hwnd](ICoreWebView2* sender, IUnknown* args) -> HRESULT
                                    {
                                        // SetWindowTitle(hwnd);
                                        return S_OK;
                                    })
                                    .Get(),
                                &documentTitleChangedToken);

                            main_wv->add_FaviconChanged(
                                Microsoft::WRL::Callback<ICoreWebView2FaviconChangedEventHandler>(
                                    [hwnd](ICoreWebView2* sender, IUnknown* args) -> HRESULT
                                    {
                                        // SetWindowIcon(hwnd);
                                        return S_OK;
                                    })
                                    .Get(),
                                &faviconChangedToken);

                            main_wv->add_WebMessageReceived(
                                Microsoft::WRL::Callback<
                                    ICoreWebView2WebMessageReceivedEventHandler>(
                                    [hwnd](
                                        ICoreWebView2* webview,
                                        ICoreWebView2WebMessageReceivedEventArgs* args) -> HRESULT
                                    {
                                        wil::unique_cotaskmem_string message;
                                        args->TryGetWebMessageAsString(&message);
                                        auto msg = message.get();
                                        Messages(msg);
                                        webview->PostWebMessageAsString(message.get());
                                        return S_OK;
                                    })
                                    .Get(),
                                &msgToken);

                            return S_OK;
                        })
                        .Get());

                // SIDE WEBVIEW
                env->CreateCoreWebView2Controller(
                    hwnd,
                    Microsoft::WRL::Callback<
                        ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                        [hwnd](HRESULT result, ICoreWebView2Controller* controller) -> HRESULT
                        {
                            EventRegistrationToken msgToken;
                            EventRegistrationToken faviconChangedToken;
                            EventRegistrationToken documentTitleChangedToken;

                            if (controller != nullptr)
                            {
                                side_controller = controller;
                                side_controller->get_CoreWebView2(&side_core);
                            }

                            side_wv = side_core.try_query<ICoreWebView2_19>();
                            side_wv->get_Settings(&side_settings);
                            side_settings->put_AreDefaultContextMenusEnabled(true);
                            side_settings->put_AreDefaultScriptDialogsEnabled(true);
                            side_settings->put_AreDevToolsEnabled(true);
                            side_settings->put_AreHostObjectsAllowed(true);
                            side_settings->put_IsBuiltInErrorPageEnabled(true);
                            side_settings->put_IsScriptEnabled(true);
                            side_settings->put_IsStatusBarEnabled(true);
                            side_settings->put_IsWebMessageEnabled(true);
                            side_settings->put_IsZoomControlEnabled(true);
                            side_controller->put_Bounds(SideBounds(hwnd));

                            auto args = Utility::CommandLine();

                            if (!args.second.empty())
                            {
                                side_wv->Navigate(args.second.c_str());
                            }

                            settings_wv->Navigate(L"https://www.bing.com/");

                            auto script = GetMenuScript();
                            side_wv->ExecuteScript(script.c_str(), nullptr);
                            side_wv->AddScriptToExecuteOnDocumentCreated(script.c_str(), nullptr);

                            side_wv->add_DocumentTitleChanged(
                                Microsoft::WRL::Callback<
                                    ICoreWebView2DocumentTitleChangedEventHandler>(
                                    [hwnd](ICoreWebView2* sender, IUnknown* args) -> HRESULT
                                    {
                                        // SetWindowTitle(hwnd);
                                        return S_OK;
                                    })
                                    .Get(),
                                &documentTitleChangedToken);

                            side_wv->add_FaviconChanged(
                                Microsoft::WRL::Callback<ICoreWebView2FaviconChangedEventHandler>(
                                    [hwnd](ICoreWebView2* sender, IUnknown* args) -> HRESULT
                                    {
                                        // SetWindowIcon(hwnd);
                                        return S_OK;
                                    })
                                    .Get(),
                                &faviconChangedToken);

                            side_wv->add_WebMessageReceived(
                                Microsoft::WRL::Callback<
                                    ICoreWebView2WebMessageReceivedEventHandler>(
                                    [hwnd](
                                        ICoreWebView2* webview,
                                        ICoreWebView2WebMessageReceivedEventArgs* args) -> HRESULT
                                    {
                                        wil::unique_cotaskmem_string message;
                                        args->TryGetWebMessageAsString(&message);
                                        auto msg = message.get();
                                        Messages(msg);
                                        webview->PostWebMessageAsString(message.get());
                                        return S_OK;
                                    })
                                    .Get(),
                                &msgToken);

                            return S_OK;
                        })
                        .Get());

                return S_OK;
            })
            .Get());
}

std::wstring WebView::GetScriptFile(std::filesystem::path appData)
{
    std::stringstream buffer;
    std::wstring script;

    std::filesystem::path file =
        (appData.wstring() + std::filesystem::path::preferred_separator + L"Airglow.js");

    if (!std::filesystem::exists(file))

        if (std::filesystem::exists(file))
        {
            std::ifstream f(file);
            if (!std::filesystem::is_empty(file))
            {
                buffer << f.rdbuf();
                script = Utility::ToWide(buffer.str());
            }
            f.close();
        }

    return script;
}

std::wstring WebView::GetScript()
{
    std::wstring script = LR"(
        document.onreadystatechange = () => {
            if (document.readyState === "interactive") {
                let scheme = document.createElement("meta");
                scheme.setAttribute("name", "color-scheme");
                scheme.setAttribute("content", "light dark");
                document.getElementsByTagName("head")[0].appendChild(scheme);
                document.documentElement.style.setProperty(
                    "color-scheme",
                    "light dark"
                );
            }
            if (document.readyState === "complete") {
                onkeydown = (e) => {
                    if (e.ctrlKey && e.key === "w") {
                        window.chrome.webview.postMessage("close");
                    } else {
                        window.chrome.webview.postMessage(e.key);
                    }
                };
            }
        };
    )";

    return script;
}

std::wstring WebView::GetMenuScript()
{
    std::wstring script = LR"(
        document.onreadystatechange = () => {
            if (document.readyState === "interactive") {
                let scheme = document.createElement("meta");
                scheme.setAttribute("name", "color-scheme");
                scheme.setAttribute("content", "light dark");
                document.getElementsByTagName("head")[0].appendChild(scheme);
                document.documentElement.style.setProperty(
                    "color-scheme",
                    "light dark"
                );
            }
            if (document.readyState === "complete") {
                onkeydown = (e) => {
                    if (e.key === "F3") {
                        e.preventDefault();
                    }
                    if (e.ctrlKey && e.key === "w") {
                        window.chrome.webview.postMessage("close");
                    } else {
                        window.chrome.webview.postMessage(e.key);
                    }
                };
            }
        };
    )";

    return script;
}

void WebView::Messages(std::wstring message)
{
    //     std::wstring splitKey = std::wstring(L"F1");
    //     std::wstring swapKey = std::wstring(L"F2");
    //     std::wstring hideMenuKey = std::wstring(L"F4");
    //     std::wstring maximizeKey = std::wstring(L"F6");
    //     std::wstring fullscreenKey = std::wstring(L"F11");
    //     std::wstring onTopKey = std::wstring(L"F9");
    //     std::wstring closeKey = std::wstring(L"close");

    //     if (message == splitKey)
    //     {
    //         split = Toggle(split);
    //         UpdateBounds(window);
    //         UpdateFocus();
    //         SetWindowTitle(window);
    //         SetWindowIcon(window);
    //     }

    //     if (message == swapKey)
    //     {
    //         swapped = Toggle(swapped);
    //         UpdateBounds(window);
    //         UpdateFocus();
    //         SetWindowTitle(window);
    //         SetWindowIcon(window);
    //     }

    //     if (message == hideMenuKey)
    //     {
    //         menu = Toggle(menu);
    //         UpdateBounds(window);
    //         UpdateFocus();
    //         SetWindowTitle(window);
    //         SetWindowIcon(window);
    //     }

    //     if (message == maximizeKey)
    //     {
    //         if (!fullscreen)
    //             maximized = Toggle(maximized);
    //         MaximizeWindow(window);
    //         UpdateFocus();
    //     }

    //     if (message == fullscreenKey)
    //     {
    //         fullscreen = Toggle(fullscreen);
    //         FullscreenWindow(window);
    //         UpdateFocus();
    //     }

    //     if (message == onTopKey)
    //     {
    //         topmost = Toggle(topmost);
    //         TopmostWindow(window);
    //         UpdateFocus();
    //         SetWindowTitle(window);
    //         SetWindowIcon(window);
    //     }

    //     if (message == closeKey)
    //     {
    //         SendMessageW(window, WM_CLOSE, 0, 0);
    //     }
}

void WebView::BoundsUpdate(HWND hwnd)
{
    if (settings_controller != nullptr)
        settings_controller->put_Bounds(MenuBounds(hwnd));

    // if (main_controller != nullptr)
    //     main_controller->put_Bounds(MainBounds(hwnd));

    // if (side_controller != nullptr)
    //     side_controller->put_Bounds(SideBounds(hwnd));
}

RECT WebView::FullBounds(HWND hwnd)
{
    RECT bounds = {0, 0, 0, 0};
    GetClientRect(hwnd, &bounds);
    auto panel = bounds;

    return panel;
}

RECT WebView::MenuBounds(HWND hwnd)
{
    RECT bounds = {0, 0, 0, 0};
    RECT panel = {0, 0, 0, 0};
    GetClientRect(hwnd, &bounds);

    // if (pSettings->boolMenu)
    // {
    //     OutputDebugStringW(L"TRUE");
    //     panel = {
    //         bounds.left,
    //         bounds.top,
    //         bounds.right,
    //         bounds.bottom,
    //     };
    // }

    panel = bounds;

    return panel;
}

RECT WebView::MainBounds(HWND window)
{
    RECT bounds = {0, 0, 0, 0};
    RECT panel = {0, 0, 0, 0};
    GetClientRect(window, &bounds);

    // if (pSettings->boolMenu)
    //     return panel;

    // if (!pSettings->boolSplit & !pSettings->boolSwapped)
    //     panel = bounds;

    // if (!pSettings->boolSplit & pSettings->boolSwapped)
    //     return panel;

    // if (pSettings->boolSplit & !pSettings->boolSwapped)
    // {
    //     panel = {
    //         bounds.left,
    //         bounds.top,
    //         bounds.right / 2,
    //         bounds.bottom,
    //     };
    // }

    // if (pSettings->boolSplit & pSettings->boolSwapped)
    // {
    //     panel = {
    //         bounds.right / 2,
    //         bounds.top,
    //         bounds.right,
    //         bounds.bottom,
    //     };
    // }

    return panel;
}

RECT WebView::SideBounds(HWND window)
{
    RECT bounds = {0, 0, 0, 0};
    RECT panel = {0, 0, 0, 0};
    GetClientRect(window, &bounds);

    // if (pSettings->boolMenu)
    //     return panel;

    // if (!pSettings->boolSplit & !pSettings->boolSwapped)
    //     return panel;

    // if (!pSettings->boolSplit & pSettings->boolSwapped)
    //     panel = bounds;

    // if (pSettings->boolSplit & !pSettings->boolSwapped)
    // {
    //     panel = {
    //         bounds.right / 2,
    //         bounds.top,
    //         bounds.right,
    //         bounds.bottom,
    //     };
    // }

    // if (pSettings->boolSplit & pSettings->boolSwapped)
    // {
    //     panel = {
    //         bounds.left,
    //         bounds.top,
    //         bounds.right / 2,
    //         bounds.bottom,
    //     };
    // }

    return panel;
}

// void WebView::SetWindowTitle(HWND window)
// {
//     std::wstring titleTop = L" [On Top]";

//     if (!swapped)
//     {
//         if (main_wv != nullptr)
//         {
//             wil::unique_cotaskmem_string s;
//             main_wv->get_DocumentTitle(&s);
//             auto title = s.get();

//             if (!topmost)
//                 SetWindowTextW(window, title);

//             if (topmost)
//             {
//                 std::wstring add = title + titleTop;
//                 SetWindowTextW(window, add.c_str());
//             }
//         }
//     }

//     else
//     {
//         if (side_wv != nullptr)
//         {
//             wil::unique_cotaskmem_string s;
//             side_wv->get_DocumentTitle(&s);
//             auto title = s.get();

//             if (!topmost)
//                 SetWindowTextW(window, title);

//             if (topmost)
//             {
//                 std::wstring add = title + titleTop;
//                 SetWindowTextW(window, add.c_str());
//             }
//         }
//     }
// }

// void WebView::SetWindowIcon(HWND window)
// {
//     if (!swapped)
//     {
//         if (main_wv != nullptr)
//         {
//             main_wv->GetFavicon(COREWEBVIEW2_FAVICON_IMAGE_FORMAT_PNG,
//                                 Microsoft::WRL::Callback<ICoreWebView2GetFaviconCompletedHandler>(
//                                     [window](HRESULT result, IStream* iconStream) -> HRESULT
//                                     {
//                                         if (iconStream != nullptr)
//                                         {
//                                             Gdiplus::Bitmap iconBitmap(iconStream);
//                                             wil::unique_hicon icon;
//                                             if (iconBitmap.GetHICON(&icon) ==
//                                             Gdiplus::Status::Ok)
//                                             {
//                                                 auto favicon = std::move(icon);
//                                                 SendMessageW(window, WM_SETICON, ICON_BIG,
//                                                              (LPARAM)favicon.get());
//                                             }
//                                         }
//                                         return S_OK;
//                                     })
//                                     .Get());
//         }
//     }

//     else
//     {
//         if (side_wv != nullptr)
//         {
//             side_wv->GetFavicon(COREWEBVIEW2_FAVICON_IMAGE_FORMAT_PNG,
//                                 Microsoft::WRL::Callback<ICoreWebView2GetFaviconCompletedHandler>(
//                                     [window](HRESULT result, IStream* iconStream) -> HRESULT
//                                     {
//                                         if (iconStream != nullptr)
//                                         {
//                                             Gdiplus::Bitmap iconBitmap(iconStream);
//                                             wil::unique_hicon icon;
//                                             if (iconBitmap.GetHICON(&icon) ==
//                                             Gdiplus::Status::Ok)
//                                             {
//                                                 auto favicon = std::move(icon);
//                                                 SendMessageW(window, WM_SETICON, ICON_BIG,
//                                                              (LPARAM)favicon.get());
//                                             }
//                                         }
//                                         return S_OK;
//                                     })
//                                     .Get());
//         }
//     }
// }
