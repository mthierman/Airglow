#include "Browser.hxx"

Browser::Browser(Window& window, Settings& settings) {}

std::unique_ptr<Browser> Browser::Create(Window& window, Settings& settings)
{
    auto browser{std::unique_ptr<Browser>(new Browser(window, settings))};

    auto hwnd{window.hwnd};

    if (FAILED(CreateCoreWebView2EnvironmentWithOptions(
            nullptr, path_portable().wstring().c_str(), nullptr,
            Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
                [&](HRESULT hr, ICoreWebView2Environment* e) -> HRESULT
                {
                    e->CreateCoreWebView2Controller(
                        hwnd, Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                                  [&](HRESULT hr, ICoreWebView2Controller* c) -> HRESULT
                                  {
                                      using namespace wv2main;

                                      EventRegistrationToken tokenTitle;
                                      EventRegistrationToken tokenFavicon;
                                      EventRegistrationToken tokenReceivedMsg;
                                      EventRegistrationToken tokenNavigationCompleted;

                                      if (c)
                                      {
                                          wvController = c;
                                          wvController->get_CoreWebView2(&wvCore);
                                      }

                                      if (wvCore)
                                          wvBrowser = wvCore.try_query<ICoreWebView2_19>();

                                      if (wvBrowser)
                                      {
                                          wvBrowser->AddScriptToExecuteOnDocumentCreated(
                                              js_inject().c_str(), nullptr);

                                          wvBrowser->get_Settings(&wvSettings);

                                          wvSettings->put_AreDefaultContextMenusEnabled(true);
                                          wvSettings->put_AreDefaultScriptDialogsEnabled(true);
                                          wvSettings->put_AreDevToolsEnabled(true);
                                          wvSettings->put_AreHostObjectsAllowed(true);
                                          wvSettings->put_IsBuiltInErrorPageEnabled(true);
                                          wvSettings->put_IsScriptEnabled(true);
                                          wvSettings->put_IsStatusBarEnabled(false);
                                          wvSettings->put_IsWebMessageEnabled(true);
                                          wvSettings->put_IsZoomControlEnabled(false);
                                      }

                                      if (!command_line().first.empty())
                                          wvBrowser->Navigate(command_line().first.c_str());
                                      else
                                          wvBrowser->Navigate(to_wide(settings.mainUrl).c_str());

                                      browser->Bounds(window, settings);
                                      browser->Focus(window, settings);

                                      wvBrowser->add_DocumentTitleChanged(
                                          Callback<ICoreWebView2DocumentTitleChangedEventHandler>(
                                              [&](ICoreWebView2* sender, IUnknown* args) -> HRESULT
                                              {
                                                  browser->Title(window, settings);

                                                  return S_OK;
                                              })
                                              .Get(),
                                          &tokenTitle);

                                      wvBrowser->add_FaviconChanged(
                                          Callback<ICoreWebView2FaviconChangedEventHandler>(
                                              [&](ICoreWebView2* sender, IUnknown* args) -> HRESULT
                                              {
                                                  // Icon();

                                                  return S_OK;
                                              })
                                              .Get(),
                                          &tokenFavicon);

                                      wvBrowser->add_WebMessageReceived(
                                          Callback<ICoreWebView2WebMessageReceivedEventHandler>(
                                              [&](ICoreWebView2* webview,
                                                  ICoreWebView2WebMessageReceivedEventArgs* args)
                                                  -> HRESULT
                                              {
                                                  browser->Messages(window, settings, args);

                                                  return S_OK;
                                              })
                                              .Get(),
                                          &tokenReceivedMsg);

                                      return S_OK;
                                  })
                                  .Get());

                    e->CreateCoreWebView2Controller(
                        hwnd, Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                                  [&](HRESULT hr, ICoreWebView2Controller* c) -> HRESULT
                                  {
                                      using namespace wv2side;

                                      EventRegistrationToken tokenTitle;
                                      EventRegistrationToken tokenFavicon;
                                      EventRegistrationToken tokenReceivedMsg;
                                      EventRegistrationToken tokenNavigationCompleted;

                                      if (c)
                                      {
                                          wvController = c;
                                          wvController->get_CoreWebView2(&wvCore);
                                      }

                                      if (wvCore)
                                          wvBrowser = wvCore.try_query<ICoreWebView2_19>();

                                      if (wvBrowser)
                                      {
                                          wvBrowser->AddScriptToExecuteOnDocumentCreated(
                                              js_inject().c_str(), nullptr);

                                          wvBrowser->get_Settings(&wvSettings);

                                          wvSettings->put_AreDefaultContextMenusEnabled(true);
                                          wvSettings->put_AreDefaultScriptDialogsEnabled(true);
                                          wvSettings->put_AreDevToolsEnabled(true);
                                          wvSettings->put_AreHostObjectsAllowed(true);
                                          wvSettings->put_IsBuiltInErrorPageEnabled(true);
                                          wvSettings->put_IsScriptEnabled(true);
                                          wvSettings->put_IsStatusBarEnabled(false);
                                          wvSettings->put_IsWebMessageEnabled(true);
                                          wvSettings->put_IsZoomControlEnabled(false);
                                      }

                                      if (!command_line().second.empty())
                                          wvBrowser->Navigate(command_line().second.c_str());
                                      else
                                          wvBrowser->Navigate(to_wide(settings.sideUrl).c_str());

                                      browser->Bounds(window, settings);
                                      browser->Focus(window, settings);

                                      wvBrowser->add_DocumentTitleChanged(
                                          Callback<ICoreWebView2DocumentTitleChangedEventHandler>(
                                              [&](ICoreWebView2* sender, IUnknown* args) -> HRESULT
                                              {
                                                  browser->Title(window, settings);

                                                  return S_OK;
                                              })
                                              .Get(),
                                          &tokenTitle);

                                      wvBrowser->add_FaviconChanged(
                                          Callback<ICoreWebView2FaviconChangedEventHandler>(
                                              [&](ICoreWebView2* sender, IUnknown* args) -> HRESULT
                                              {
                                                  // Icon();

                                                  return S_OK;
                                              })
                                              .Get(),
                                          &tokenFavicon);

                                      wvBrowser->add_WebMessageReceived(
                                          Callback<ICoreWebView2WebMessageReceivedEventHandler>(
                                              [&](ICoreWebView2* webview,
                                                  ICoreWebView2WebMessageReceivedEventArgs* args)
                                                  -> HRESULT
                                              {
                                                  browser->Messages(window, settings, args);

                                                  return S_OK;
                                              })
                                              .Get(),
                                          &tokenReceivedMsg);

                                      return S_OK;
                                  })
                                  .Get());

                    e->CreateCoreWebView2Controller(
                        hwnd, Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                                  [&](HRESULT hr, ICoreWebView2Controller* c) -> HRESULT
                                  {
                                      using namespace wv2settings;

                                      EventRegistrationToken tokenTitle;
                                      EventRegistrationToken tokenFavicon;
                                      EventRegistrationToken tokenReceivedMsg;
                                      EventRegistrationToken tokenNavigationCompleted;

                                      if (c)
                                      {
                                          wvController = c;
                                          wvController->get_CoreWebView2(&wvCore);
                                      }

                                      if (wvCore)
                                          wvBrowser = wvCore.try_query<ICoreWebView2_19>();

                                      if (wvBrowser)
                                      {
                                          wvBrowser->AddScriptToExecuteOnDocumentCreated(
                                              js_inject().c_str(), nullptr);

                                          wvBrowser->get_Settings(&wvSettings);

                                          wvSettings->put_AreDefaultContextMenusEnabled(true);
                                          wvSettings->put_AreDefaultScriptDialogsEnabled(true);
                                          wvSettings->put_AreDevToolsEnabled(true);
                                          wvSettings->put_AreHostObjectsAllowed(true);
                                          wvSettings->put_IsBuiltInErrorPageEnabled(true);
                                          wvSettings->put_IsScriptEnabled(true);
                                          wvSettings->put_IsStatusBarEnabled(false);
                                          wvSettings->put_IsWebMessageEnabled(true);
                                          wvSettings->put_IsZoomControlEnabled(false);
                                      }

                                      wvBrowser->SetVirtualHostNameToFolderMapping(
                                          L"settings", path_portable().wstring().c_str(),
                                          COREWEBVIEW2_HOST_RESOURCE_ACCESS_KIND_ALLOW);

                                      wvBrowser->Navigate(L"https://settings/index.html");
#ifdef _DEBUG
                                      wvBrowser->Navigate(L"https://localhost:8000/");
#endif

                                      browser->Bounds(window, settings);
                                      browser->Focus(window, settings);

                                      SendMessageW(hwnd, WM_SETFOCUS, 0, 0);

                                      wvBrowser->add_DocumentTitleChanged(
                                          Callback<ICoreWebView2DocumentTitleChangedEventHandler>(
                                              [&](ICoreWebView2* sender, IUnknown* args) -> HRESULT
                                              {
                                                  browser->Title(window, settings);

                                                  return S_OK;
                                              })
                                              .Get(),
                                          &tokenTitle);

                                      wvBrowser->add_FaviconChanged(
                                          Callback<ICoreWebView2FaviconChangedEventHandler>(
                                              [&](ICoreWebView2* sender, IUnknown* args) -> HRESULT
                                              {
                                                  // Icon();

                                                  return S_OK;
                                              })
                                              .Get(),
                                          &tokenFavicon);

                                      wvBrowser->add_WebMessageReceived(
                                          Callback<ICoreWebView2WebMessageReceivedEventHandler>(
                                              [&](ICoreWebView2* webview,
                                                  ICoreWebView2WebMessageReceivedEventArgs* args)
                                                  -> HRESULT
                                              {
                                                  browser->Messages(window, settings, args);

                                                  return S_OK;
                                              })
                                              .Get(),
                                          &tokenReceivedMsg);

                                      return S_OK;
                                  })
                                  .Get());

                    return S_OK;
                })
                .Get())))
    {
        return nullptr;
    };

    return browser;
}

void Browser::Bounds(Window& window, Settings& settings)
{
    if (!wv2main::wvController || !wv2side::wvController || !wv2settings::wvController)
        return;

    auto bounds{window_bounds(window.hwnd)};

    if (settings.menu)
    {
        wv2main::wvController->put_Bounds(RECT{0, 0, 0, 0});
        wv2side::wvController->put_Bounds(RECT{0, 0, 0, 0});
        wv2settings::wvController->put_Bounds(bounds);
    }

    else
    {
        wv2settings::wvController->put_Bounds(RECT{0, 0, 0, 0});

        if (!settings.split && !settings.swapped)
        {
            wv2main::wvController->put_Bounds(bounds);
            wv2side::wvController->put_Bounds(RECT{0, 0, 0, 0});
        }

        if (!settings.split && settings.swapped)
        {
            wv2main::wvController->put_Bounds(RECT{0, 0, 0, 0});
            wv2side::wvController->put_Bounds(bounds);
        }

        if (settings.split && !settings.swapped)
        {
            wv2main::wvController->put_Bounds(left_panel(bounds));
            wv2side::wvController->put_Bounds(right_panel(bounds));
        }

        if (settings.split && settings.swapped)
        {
            wv2main::wvController->put_Bounds(right_panel(bounds));
            wv2side::wvController->put_Bounds(left_panel(bounds));
        }
    }
}

void Browser::Focus(Window& window, Settings& settings)
{
    if (!wv2main::wvController || !wv2side::wvController || !wv2settings::wvController)
        return;

    if (!settings.menu && !settings.swapped)
        wv2main::wvController->MoveFocus(
            COREWEBVIEW2_MOVE_FOCUS_REASON::COREWEBVIEW2_MOVE_FOCUS_REASON_PROGRAMMATIC);

    if (!settings.menu && settings.swapped)
        wv2side::wvController->MoveFocus(
            COREWEBVIEW2_MOVE_FOCUS_REASON::COREWEBVIEW2_MOVE_FOCUS_REASON_PROGRAMMATIC);

    if (settings.menu)
        wv2settings::wvController->MoveFocus(
            COREWEBVIEW2_MOVE_FOCUS_REASON::COREWEBVIEW2_MOVE_FOCUS_REASON_PROGRAMMATIC);
}

void Browser::Title(Window& window, Settings& settings)
{
    if (!wv2main::wvController || !wv2side::wvController || !wv2settings::wvController)
        return;

    if (settings.menu)
    {
        wil::unique_cotaskmem_string s{};
        wv2settings::wvBrowser->get_DocumentTitle(&s);
        auto title = s.get();

        if (!settings.topmost)
            SetWindowTextW(window.hwnd, title);

        if (settings.topmost)
        {
            wstring add = title + wstring(L" [On Top]");
            SetWindowTextW(window.hwnd, add.c_str());
        }
    }

    if (!settings.menu && !settings.swapped)
    {
        wil::unique_cotaskmem_string s{};
        wv2main::wvBrowser->get_DocumentTitle(&s);
        auto title = s.get();

        if (!settings.topmost)
            SetWindowTextW(window.hwnd, title);

        if (settings.topmost)
        {
            wstring add = title + wstring(L" [On Top]");
            SetWindowTextW(window.hwnd, add.c_str());
        }
    }

    if (!settings.menu && settings.swapped)
    {
        wil::unique_cotaskmem_string s{};
        wv2side::wvBrowser->get_DocumentTitle(&s);
        auto title = s.get();

        if (!settings.topmost)
            SetWindowTextW(window.hwnd, title);

        if (settings.topmost)
        {
            wstring add = title + wstring(L" [On Top]");
            SetWindowTextW(window.hwnd, add.c_str());
        }
    }
}

// void WebView::SetWindowIcon()
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

void Browser::Messages(Window& window, Settings& settings,
                       ICoreWebView2WebMessageReceivedEventArgs* args)
{
    wil::unique_cotaskmem_string messageRaw;

    if (SUCCEEDED(args->TryGetWebMessageAsString(&messageRaw)))
    {
        auto message = wstring(messageRaw.get());

        if (message.compare(0, 8, L"mainUrl ") == 0)
        {
            to_wide(settings.mainUrl) = message.substr(8);
        }

        if (message.compare(0, 8, L"sideUrl ") == 0)
        {
            to_wide(settings.sideUrl) = message.substr(8);
        }

        if (message == L"split")
        {
            SendMessageW(window.hwnd, WM_KEYDOWN, VK_F1, 0);
        }

        if (message == L"swapped")
        {
            SendMessageW(window.hwnd, WM_KEYDOWN, VK_F2, 0);
        }

        if (message == L"menu")
        {
            SendMessageW(window.hwnd, WM_KEYDOWN, VK_F4, 0);
        }

        if (message == L"maximize")
        {
            SendMessageW(window.hwnd, WM_KEYDOWN, VK_F6, 0);
        }

        if (message == L"fullscreen")
        {
            SendMessageW(window.hwnd, WM_KEYDOWN, VK_F11, 0);
        }

        if (message == L"topmost")
        {
            SendMessageW(window.hwnd, WM_KEYDOWN, VK_F9, 0);
        }

        if (message == L"close")
        {
            SendMessageW(window.hwnd, WM_KEYDOWN, 0x57, 0);
        }
    }
}
