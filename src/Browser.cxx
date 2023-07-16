#include "Browser.hxx"
#include "Utility.hxx"

using namespace Utility;
using namespace Gdiplus;
using namespace Microsoft::WRL;

wil::com_ptr<ICoreWebView2Environment> Browser::wv2_environment{nullptr};
wil::com_ptr<ICoreWebView2Controller> Browser::wv2_controller{nullptr};
wil::com_ptr<ICoreWebView2> Browser::wv2{nullptr};
wil::com_ptr<ICoreWebView2_19> Browser::wv2_19{nullptr};
wil::com_ptr<ICoreWebView2Settings> Browser::wv2_settings{nullptr};

Browser::Browser(HWND hwnd) {}

std::unique_ptr<Browser> Browser::Create(HWND hwnd)
{
    auto browser{std::unique_ptr<Browser>(new Browser(hwnd))};

    if (FAILED(CreateCoreWebView2EnvironmentWithOptions(
            nullptr, path_portable().wstring().c_str(), nullptr,
            Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
                [=](HRESULT hr, ICoreWebView2Environment* environment) -> HRESULT
                {
                    if (environment)
                        wv2_environment = environment;

                    return S_OK;
                })
                .Get())))
    {
        return nullptr;
    };

    if (FAILED(wv2_environment->CreateCoreWebView2Controller(
            hwnd, Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                      [=](HRESULT hr, ICoreWebView2Controller* controller) -> HRESULT
                      {
                          if (controller)
                              wv2_controller = controller;

                          if (SUCCEEDED(wv2_controller->get_CoreWebView2(&wv2)))
                              wv2_19 = wv2.try_query<ICoreWebView2_19>();

                          if (wv2_19)
                          {
                              wv2_controller->put_Bounds(get_rect(hwnd));
                              wv2_19->Navigate(L"https://wwww.google.com/");
                          }

                          return S_OK;
                      })
                      .Get())))
    {
        return nullptr;
    };

    return browser;
}

// std::unique_ptr<Browser> Browser::Create(HWND hwnd)
// {
//     auto browser{std::unique_ptr<Browser>(new Browser(hwnd))};

//     auto create = CreateCoreWebView2EnvironmentWithOptions(
//         nullptr, path_portable().wstring().c_str(), nullptr,
//         Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
//             [=](HRESULT hr, ICoreWebView2Environment* environment) -> HRESULT
//             {
//                 environment->CreateCoreWebView2Controller(
//                     hwnd,
//                     Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
//                               [=](HRESULT hr, ICoreWebView2Controller* controller) ->
//                               HRESULT
//                               {
//                                   if (!controller)
//                                       return E_POINTER;

//                                   wv2_controller = controller;

//                                   if (FAILED(wv2_controller->get_CoreWebView2(&wv2)))
//                                       return E_POINTER;

//                                   wv2_19 = wv2.try_query<ICoreWebView2_19>();

//                                   if (!wv2_19)
//                                       return E_POINTER;

//                                   wv2_controller->put_Bounds(get_rect(hwnd));

//                                   wv2_19->Navigate(L"https://wwww.google.com/");

//                                   return S_OK;
//                               })
//                               .Get());

//                 return S_OK;
//             })
//             .Get());

//     // if (!wv2_19)
//     //     return nullptr;

//     return browser;
// }

// std::unique_ptr<Browser> Browser::Create(HWND hwnd)
// {
//     auto browser{std::unique_ptr<Browser>(new Browser(hwnd))};

//     auto createEnvironment = CreateCoreWebView2EnvironmentWithOptions(
//         nullptr, path_portable().wstring().c_str(), nullptr,
//         Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
//             [=](HRESULT hr, ICoreWebView2Environment* environment) -> HRESULT
//             {
//                 auto createController = environment->CreateCoreWebView2Controller(
//                     hwnd,
//                     Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
//                         [=](HRESULT hr, ICoreWebView2Controller* controller) -> HRESULT
//                         {
//                             if (controller)
//                                 return E_ABORT;

//                             wv2_controller = controller;
//                             controller->get_CoreWebView2(&wv2);

//                             if (!wv2)
//                                 return E_ABORT;

//                             wv2_19 = wv2.try_query<ICoreWebView2_19>();

//                             if (!wv2_19)
//                                 return E_ABORT;

//                             wv2_19->get_Settings(&wv2_settings);

//                             if (!wv2_settings)
//                                 return E_ABORT;

//                             wv2_settings->put_AreDefaultContextMenusEnabled(true);
//                             wv2_settings->put_AreDefaultScriptDialogsEnabled(true);
//                             wv2_settings->put_AreDevToolsEnabled(true);
//                             wv2_settings->put_AreHostObjectsAllowed(true);
//                             wv2_settings->put_IsBuiltInErrorPageEnabled(true);
//                             wv2_settings->put_IsScriptEnabled(true);
//                             wv2_settings->put_IsStatusBarEnabled(false);
//                             wv2_settings->put_IsWebMessageEnabled(true);
//                             wv2_settings->put_IsZoomControlEnabled(false);

//                             wv2_controller->put_Bounds(get_rect(hwnd));

//                             wv2_19->SetVirtualHostNameToFolderMapping(
//                                 L"settings", path_settings().wstring().c_str(),
//                                 COREWEBVIEW2_HOST_RESOURCE_ACCESS_KIND_ALLOW);

//                             wv2_19->Navigate(L"https://wwww.google.com/");

//                             EventRegistrationToken tokenNavigationCompleted;
//                             wv2_19->add_NavigationCompleted(
//                                 Callback<ICoreWebView2NavigationCompletedEventHandler>(
//                                     [&](ICoreWebView2* sender, IUnknown* args) -> HRESULT
//                                     { return S_OK; })
//                                     .Get(),
//                                 &tokenNavigationCompleted);

//                             EventRegistrationToken tokenDocumentTitleChanged;
//                             wv2_19->add_DocumentTitleChanged(
//                                 Callback<ICoreWebView2DocumentTitleChangedEventHandler>(
//                                     [&](ICoreWebView2* sender, IUnknown* args) -> HRESULT
//                                     { return S_OK; })
//                                     .Get(),
//                                 &tokenDocumentTitleChanged);

//                             EventRegistrationToken tokenFaviconChanged;
//                             wv2_19->add_FaviconChanged(
//                                 Callback<ICoreWebView2FaviconChangedEventHandler>(
//                                     [&](ICoreWebView2* sender, IUnknown* args) -> HRESULT
//                                     { return S_OK; })
//                                     .Get(),
//                                 &tokenFaviconChanged);

//                             EventRegistrationToken tokenWebMessageReceived;
//                             wv2_19->add_WebMessageReceived(
//                                 Callback<ICoreWebView2WebMessageReceivedEventHandler>(
//                                     [&](ICoreWebView2* sender,
//                                         ICoreWebView2WebMessageReceivedEventArgs* args)
//                                         -> HRESULT
//                                     { return S_OK; })
//                                     .Get(),
//                                 &tokenWebMessageReceived);

//                             return S_OK;
//                         })
//                         .Get());

//                 if (createController == E_ABORT)
//                     return E_ABORT;

//                 return S_OK;
//             })
//             .Get());

//     if (createEnvironment != S_OK)
//         return nullptr;

//     return browser;
// }

// MAIN WEBVIEW
//                 e->CreateCoreWebView2Controller(
//                     pConfig->window.hwnd,
//                     Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
//                         [&](HRESULT result, ICoreWebView2Controller* c) ->
//                         HRESULT
//                         {
//                             using namespace Browsers::Main;

//                             EventRegistrationToken tokenTitle;
//                             EventRegistrationToken tokenFavicon;
//                             EventRegistrationToken tokenReceivedMsg;
//                             EventRegistrationToken tokenNavigationCompleted;

//                             if (!c)
//                                 return E_POINTER;

//                             controller = c;
//                             controller->get_CoreWebView2(&core);

//                             if (!core)
//                                 return E_POINTER;

//                             browser = core.try_query<ICoreWebView2_19>();

//                             if (!browser)
//                                 return E_POINTER;

//                             browser->get_Settings(&settings);

//                             if (!settings)
//                                 return E_POINTER;

//                             settings->put_AreDefaultContextMenusEnabled(true);
//                             settings->put_AreDefaultScriptDialogsEnabled(true);
//                             settings->put_AreDevToolsEnabled(true);
//                             settings->put_AreHostObjectsAllowed(true);
//                             settings->put_IsBuiltInErrorPageEnabled(true);
//                             settings->put_IsScriptEnabled(true);
//                             settings->put_IsStatusBarEnabled(true);
//                             settings->put_IsWebMessageEnabled(true);
//                             settings->put_IsZoomControlEnabled(false);

//                             controller->put_Bounds(webView->MainBounds());

//                             browser->Navigate(webView->MainNavigation().c_str());

//                             auto script = webView->GetScriptFile();
//                             browser->ExecuteScript(script.c_str(), nullptr);
//                             browser->AddScriptToExecuteOnDocumentCreated(script.c_str(),
//                             nullptr);

//                             browser->add_DocumentTitleChanged(
//                                 Callback<ICoreWebView2DocumentTitleChangedEventHandler>(
//                                     [&](ICoreWebView2* sender, IUnknown* args) ->
//                                     HRESULT
//                                     {
//                                         webView->SetWindowTitle();

//                                         return S_OK;
//                                     })
//                                     .Get(),
//                                 &tokenTitle);

//                             browser->add_FaviconChanged(
//                                 Callback<ICoreWebView2FaviconChangedEventHandler>(
//                                     [&](ICoreWebView2* sender, IUnknown* args) ->
//                                     HRESULT
//                                     {
//                                         webView->SetWindowIcon();

//                                         return S_OK;
//                                     })
//                                     .Get(),
//                                 &tokenFavicon);

//                             browser->add_WebMessageReceived(
//                                 Callback<ICoreWebView2WebMessageReceivedEventHandler>(
//                                     [&](ICoreWebView2* webview,
//                                         ICoreWebView2WebMessageReceivedEventArgs*
//                                         args) -> HRESULT
//                                     {
//                                         webView->Messages(args);

//                                         return S_OK;
//                                     })
//                                     .Get(),
//                                 &tokenReceivedMsg);
// #ifdef DEV_TOOLS
//                             browser->OpenDevToolsWindow();
// #endif

//                             return S_OK;
//                         })
//                         .Get());

// SIDE WEBVIEW
//                 e->CreateCoreWebView2Controller(
//                     pConfig->window.hwnd,
//                     Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
//                         [&](HRESULT result, ICoreWebView2Controller* c) ->
//                         HRESULT
//                         {
//                             using namespace Browsers::Side;

//                             EventRegistrationToken tokenTitle;
//                             EventRegistrationToken tokenFavicon;
//                             EventRegistrationToken tokenReceivedMsg;
//                             EventRegistrationToken tokenNavigationCompleted;

//                             if (!c)
//                                 return E_POINTER;

//                             controller = c;
//                             controller->get_CoreWebView2(&core);

//                             if (!core)
//                                 return E_POINTER;

//                             browser = core.try_query<ICoreWebView2_19>();

//                             if (!browser)
//                                 return E_POINTER;

//                             browser->get_Settings(&settings);

//                             if (!settings)
//                                 return E_POINTER;

//                             settings->put_AreDefaultContextMenusEnabled(true);
//                             settings->put_AreDefaultScriptDialogsEnabled(true);
//                             settings->put_AreDevToolsEnabled(true);
//                             settings->put_AreHostObjectsAllowed(true);
//                             settings->put_IsBuiltInErrorPageEnabled(true);
//                             settings->put_IsScriptEnabled(true);
//                             settings->put_IsStatusBarEnabled(true);
//                             settings->put_IsWebMessageEnabled(true);
//                             settings->put_IsZoomControlEnabled(false);

//                             controller->put_Bounds(webView->SideBounds());

//                             browser->Navigate(webView->SideNavigation().c_str());

//                             auto script = webView->GetScriptFile();
//                             browser->ExecuteScript(script.c_str(), nullptr);
//                             browser->AddScriptToExecuteOnDocumentCreated(script.c_str(),
//                             nullptr);

//                             browser->add_DocumentTitleChanged(
//                                 Callback<ICoreWebView2DocumentTitleChangedEventHandler>(
//                                     [&](ICoreWebView2* sender, IUnknown* args) ->
//                                     HRESULT
//                                     {
//                                         webView->SetWindowTitle();

//                                         return S_OK;
//                                     })
//                                     .Get(),
//                                 &tokenTitle);

//                             browser->add_FaviconChanged(
//                                 Callback<ICoreWebView2FaviconChangedEventHandler>(
//                                     [&](ICoreWebView2* sender, IUnknown* args) ->
//                                     HRESULT
//                                     {
//                                         webView->SetWindowIcon();

//                                         return S_OK;
//                                     })
//                                     .Get(),
//                                 &tokenFavicon);

//                             browser->add_WebMessageReceived(
//                                 Callback<ICoreWebView2WebMessageReceivedEventHandler>(
//                                     [&](ICoreWebView2* webview,
//                                         ICoreWebView2WebMessageReceivedEventArgs*
//                                         args) -> HRESULT
//                                     {
//                                         webView->Messages(args);

//                                         return S_OK;
//                                     })
//                                     .Get(),
//                                 &tokenReceivedMsg);
// #ifdef DEV_TOOLS
//                             browser->OpenDevToolsWindow();
// #endif

//                             return S_OK;
//                         })
//                         .Get());

//                 return S_OK;
//             })
//             .Get());

//     return browser;
// }

// void Browser::UpdateBounds()
// {
//     if (!Browsers::Settings::controller || !Browsers::Main::controller ||
//         !Browsers::Side::controller)
//         return;

//     Browsers::Settings::controller->put_Bounds(SettingsBounds());
//     Browsers::Main::controller->put_Bounds(MainBounds());
//     Browsers::Side::controller->put_Bounds(SideBounds());
// }

// void Browser::UpdateConfig()
// {
//     if (!pConfig || !Browsers::Settings::browser)
//         return;

//     json j{{"settings",
//             {{"mainUrl", pConfig->settings.mainUrl},
//              {"sideUrl", pConfig->settings.sideUrl},
//              {"accentColor", pConfig->colors.accent}}}};

//     Browsers::Settings::browser->PostWebMessageAsJson(to_wide(j.dump()).c_str());
// }

// void Browser::UpdateFocus()
// {
//     if (!pConfig || !Browsers::Settings::controller || !Browsers::Main::controller ||
//         !Browsers::Side::controller)
//         return;

//     if (pConfig->settings.menu)
//         Browsers::Settings::controller->MoveFocus(
//             COREWEBVIEW2_MOVE_FOCUS_REASON::COREWEBVIEW2_MOVE_FOCUS_REASON_PROGRAMMATIC);

//     if (!pConfig->settings.swapped & !pConfig->settings.menu)
//         Browsers::Main::controller->MoveFocus(
//             COREWEBVIEW2_MOVE_FOCUS_REASON::COREWEBVIEW2_MOVE_FOCUS_REASON_PROGRAMMATIC);

//     if (pConfig->settings.swapped & !pConfig->settings.menu)
//         Browsers::Side::controller->MoveFocus(
//             COREWEBVIEW2_MOVE_FOCUS_REASON::COREWEBVIEW2_MOVE_FOCUS_REASON_PROGRAMMATIC);
// }

// void Browser::SetWindowTitle()
// {
//     if (!pConfig || !Browsers::Settings::controller || !Browsers::Main::controller ||
//         !Browsers::Side::controller)
//         return;

//     if (pConfig->settings.menu)
//     {
//         wil::unique_cotaskmem_string s{};
//         Browsers::Settings::browser->get_DocumentTitle(&s);
//         auto title = s.get();

//         if (!pConfig->settings.topmost)
//             SetWindowTextW(pConfig->window.hwnd, title);

//         if (pConfig->settings.topmost)
//         {
//             wstring add = title + wstring(L" [On Top]");
//             SetWindowTextW(pConfig->window.hwnd, add.c_str());
//         }
//     }

//     if (!pConfig->settings.menu && !pConfig->settings.swapped)
//     {
//         wil::unique_cotaskmem_string s{};
//         Browsers::Main::browser->get_DocumentTitle(&s);
//         auto title = s.get();

//         if (!pConfig->settings.topmost)
//             SetWindowTextW(pConfig->window.hwnd, title);

//         if (pConfig->settings.topmost)
//         {
//             wstring add = title + wstring(L" [On Top]");
//             SetWindowTextW(pConfig->window.hwnd, add.c_str());
//         }
//     }

//     if (!pConfig->settings.menu && pConfig->settings.swapped)
//     {
//         wil::unique_cotaskmem_string s{};
//         Browsers::Side::browser->get_DocumentTitle(&s);
//         auto title = s.get();

//         if (!pConfig->settings.topmost)
//             SetWindowTextW(pConfig->window.hwnd, title);

//         if (pConfig->settings.topmost)
//         {
//             wstring add = title + wstring(L" [On Top]");
//             SetWindowTextW(pConfig->window.hwnd, add.c_str());
//         }
//     }
// }

// void Browser::SetWindowIcon()
// {

//     if (!pConfig || !Browsers::Settings::controller || !Browsers::Main::controller ||
//         !Browsers::Side::controller)
//         return;

//     if (pConfig->settings.menu)
//     {
//         // #ifdef DEBUG_MSG
//         //         LPWSTR faviconUri;
//         //         Browsers::Settings::browser->get_FaviconUri(&faviconUri);
//         //         wprintln(wstring(faviconUri));
//         // #endif
//         Browsers::Settings::browser->GetFavicon(
//             COREWEBVIEW2_FAVICON_IMAGE_FORMAT_PNG,
//             Callback<ICoreWebView2GetFaviconCompletedHandler>(
//                 [&](HRESULT result, IStream* iconStream) -> HRESULT
//                 {
//                     if (iconStream)
//                     {
//                         Gdiplus::Bitmap iconBitmap(iconStream);
//                         wil::unique_hicon icon;
//                         if (iconBitmap.GetHICON(&icon) == Gdiplus::Status::Ok)
//                         {
//                             auto favicon = std::move(icon);
//                             SetClassLongPtrW(pConfig->window.hwnd, GCLP_HICONSM,
//                                              (LONG_PTR)favicon.get());
//                             SetClassLongPtrW(pConfig->window.hwnd, GCLP_HICON,
//                                              (LONG_PTR)pConfig->window.hIcon);
//                         }
//                     }
//                     return S_OK;
//                 })
//                 .Get());
//     }

//     if (!pConfig->settings.swapped && !pConfig->settings.menu)
//     {
//         // #ifdef DEBUG_MSG
//         //         LPWSTR faviconUri;
//         //         Browsers::Main::browser->get_FaviconUri(&faviconUri);
//         //         wprintln(wstring(faviconUri));
//         // #endif
//         Browsers::Main::browser->GetFavicon(
//             COREWEBVIEW2_FAVICON_IMAGE_FORMAT_PNG,
//             Callback<ICoreWebView2GetFaviconCompletedHandler>(
//                 [&](HRESULT result, IStream* iconStream) -> HRESULT
//                 {
//                     if (iconStream)
//                     {
//                         Gdiplus::Bitmap iconBitmap(iconStream);
//                         wil::unique_hicon icon;
//                         if (iconBitmap.GetHICON(&icon) == Gdiplus::Status::Ok)
//                         {
//                             auto favicon = std::move(icon);
//                             SetClassLongPtrW(pConfig->window.hwnd, GCLP_HICONSM,
//                                              (LONG_PTR)favicon.get());
//                             SetClassLongPtrW(pConfig->window.hwnd, GCLP_HICON,
//                                              (LONG_PTR)pConfig->window.hIcon);
//                         }
//                     }
//                     return S_OK;
//                 })
//                 .Get());
//     }

//     if (pConfig->settings.swapped && !pConfig->settings.menu)
//     {
//         // #ifdef DEBUG_MSG
//         //         LPWSTR faviconUri;
//         //         Browsers::Side::browser->get_FaviconUri(&faviconUri);
//         //         wprintln(wstring(faviconUri));
//         // #endif
//         Browsers::Side::browser->GetFavicon(
//             COREWEBVIEW2_FAVICON_IMAGE_FORMAT_PNG,
//             Callback<ICoreWebView2GetFaviconCompletedHandler>(
//                 [&](HRESULT result, IStream* iconStream) -> HRESULT
//                 {
//                     if (iconStream)
//                     {
//                         Gdiplus::Bitmap iconBitmap(iconStream);
//                         wil::unique_hicon icon;
//                         if (iconBitmap.GetHICON(&icon) == Gdiplus::Status::Ok)
//                         {
//                             auto favicon = std::move(icon);
//                             SetClassLongPtrW(pConfig->window.hwnd, GCLP_HICONSM,
//                                              (LONG_PTR)favicon.get());
//                             SetClassLongPtrW(pConfig->window.hwnd, GCLP_HICON,
//                                              (LONG_PTR)pConfig->window.hIcon);
//                         }
//                     }
//                     return S_OK;
//                 })
//                 .Get());
//     }
// }

// std::pair<wstring, wstring> Browser::CommandLine()
// {
//     std::pair<wstring, wstring> commands;

//     auto cmd = GetCommandLineW();
//     int count;

//     auto args = CommandLineToArgvW(cmd, &count);

//     if (count == 2)
//     {
//         commands.first = args[1];
//         commands.second = wstring{};
//     }

//     if (count == 3)
//     {
//         commands.first = args[1];
//         commands.second = args[2];
//     }

//     LocalFree(args);

//     if (!commands.first.empty())
//     {
//         if (!commands.first.starts_with(L"http") ||
//         !commands.first.starts_with(L"https"))
//             commands.first = L"https://" + commands.first;
//     }

//     if (!commands.second.empty())
//     {
//         if (!commands.second.starts_with(L"http") ||
//         !commands.second.starts_with(L"https"))
//             commands.second = L"https://" + commands.second;
//     }

//     return commands;
// }

// wstring Browser::MainNavigation()
// {
//     auto args = CommandLine();

//     if (!args.first.empty())
//     {
//         return args.first;
//     }

//     return to_wide("https://" + pConfig->settings.mainUrl);
// }

// wstring Browser::SideNavigation()
// {
//     auto args = CommandLine();

//     if (!args.second.empty())
//     {
//         return args.second;
//     }

//     return to_wide("https://" + pConfig->settings.sideUrl);
// }

// bool Browser::VerifySettingsUrl(ICoreWebView2WebMessageReceivedEventArgs* args)
// {
//     wstring verify{};
//     verify = L"https://settings/index.html";
// #ifdef _DEBUG
//     verify = L"https://localhost:8000/";
// #endif
//     wil::unique_cotaskmem_string s{};
//     args->get_Source(&s);
//     wstring source = s.get();
//     if (source != verify)
//         return false;

//     return true;
// }

// wstring Browser::SettingsNavigation()
// {
// #ifdef _DEBUG
//     return L"https://localhost:8000/";
// #endif
//     return L"https://settings/index.html";
// }

// void Browser::Messages(ICoreWebView2WebMessageReceivedEventArgs* args)
// {
//     // string splitKey{"F1"};
//     // string swapKey{"F2"};
//     // string hideMenuKey{"F4"};
//     // string maximizeKey{"F6"};
//     // string fullscreenKey{"F11"};
//     // string onTopKey{"F9"};
//     // string closeKey{"close"};

//     wil::unique_cotaskmem_string messageRaw;
//     if (SUCCEEDED(args->TryGetWebMessageAsString(&messageRaw)))
//     {
//         if (!pConfig)
//             return;

//         auto message = to_string(wstring(messageRaw.get()));

//         if (message.compare(0, 8, "mainUrl ") == 0)
//         {
//             pConfig->settings.mainUrl = message.substr(8);
//         }

//         if (message.compare(0, 8, "sideUrl ") == 0)
//         {
//             pConfig->settings.sideUrl = message.substr(8);
//         }

//         if (message == "split")
//         {
//             SendMessageW(pConfig->window.hwnd, WM_KEYDOWN, VK_F1, 0);
//         }

//         if (message == "swapped")
//         {
//             SendMessageW(pConfig->window.hwnd, WM_KEYDOWN, VK_F2, 0);
//         }

//         if (message == "menu")
//         {
//             SendMessageW(pConfig->window.hwnd, WM_KEYDOWN, VK_F4, 0);
//         }

//         if (message == "maximize")
//         {
//             SendMessageW(pConfig->window.hwnd, WM_KEYDOWN, VK_F6, 0);
//         }

//         if (message == "topmost")
//         {
//             SendMessageW(pConfig->window.hwnd, WM_KEYDOWN, VK_F9, 0);
//         }

//         if (message == "fullscreen")
//         {
//             SendMessageW(pConfig->window.hwnd, WM_KEYDOWN, VK_F11, 0);
//         }

//         if (message == "close")
//         {
//             SendMessageW(pConfig->window.hwnd, WM_KEYDOWN, 0x57, 0);
//         }
//     }
// }

// RECT Browser::MainBounds()
// {
//     if (pConfig->settings.menu || !pConfig->settings.split && pConfig->settings.swapped)
//         return RECT{0, 0, 0, 0};

//     auto bounds{get_rect(pConfig->window.hwnd)};

//     if (!pConfig->settings.split && !pConfig->settings.swapped)
//         return bounds;

//     if (pConfig->settings.split & !pConfig->settings.swapped)
//     {
//         return RECT{
//             bounds.left,
//             bounds.top,
//             bounds.right / 2,
//             bounds.bottom,
//         };
//     }

//     if (pConfig->settings.split & pConfig->settings.swapped)
//     {
//         return RECT{
//             bounds.right / 2,
//             bounds.top,
//             bounds.right,
//             bounds.bottom,
//         };
//     }

//     return bounds;
// }

// RECT Browser::SideBounds()
// {
//     if (pConfig->settings.menu || !pConfig->settings.split && !pConfig->settings.swapped)
//         return RECT{0, 0, 0, 0};

//     auto bounds{get_rect(pConfig->window.hwnd)};

//     if (!pConfig->settings.split & pConfig->settings.swapped)
//         return bounds;

//     if (pConfig->settings.split & !pConfig->settings.swapped)
//     {
//         return RECT{
//             bounds.right / 2,
//             bounds.top,
//             bounds.right,
//             bounds.bottom,
//         };
//     }

//     if (pConfig->settings.split & pConfig->settings.swapped)
//     {
//         return RECT{
//             bounds.left,
//             bounds.top,
//             bounds.right / 2,
//             bounds.bottom,
//         };
//     }

//     return bounds;
// }

// wstring Browser::GetScriptFile()
// {
//     stringstream buffer{};
//     wstring script{};

//     if (std::filesystem::exists(pConfig->paths.js))
//     {
//         ifstream f(pConfig->paths.js);
//         if (!std::filesystem::is_empty(pConfig->paths.js))
//         {
//             buffer << f.rdbuf();
//             script = to_wide(buffer.str());
//         }
//         f.close();
//     }

//     return script;
// }

// wstring Browser::GetScript()
// {
//     return wstring{LR"(
//         document.onreadystatechange = () => {
//             if (document.readyState === "interactive") {
//                 let scheme = document.createElement("meta");
//                 scheme.setAttribute("name", "color-scheme");
//                 scheme.setAttribute("content", "light dark");
//                 document.getElementsByTagName("head")[0].appendChild(scheme);
//                 document.documentElement.style.setProperty(
//                     "color-scheme",
//                     "light dark"
//                 );
//             }
//             if (document.readyState === "complete") {
//                 onkeydown = (e) => {
//                     if (e.ctrlKey && e.key === "w") {
//                         window.chrome.webview.postMessage("close");
//                     } else {
//                         window.chrome.webview.postMessage(e.key);
//                     }
//                 };
//             }
//         };
//     )"};
// }

// RECT Browser::SettingsBounds()
// {
//     if (!pConfig->settings.menu)
//         return RECT{0, 0, 0, 0};

//     return get_rect(hwnd);
// }
