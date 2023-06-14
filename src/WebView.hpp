void CreateWebView2Env(HWND window)
{
    using namespace Microsoft::WRL;

    CreateCoreWebView2EnvironmentWithOptions(
        nullptr, nullptr, nullptr,
        Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [window](HRESULT result, ICoreWebView2Environment* env) -> HRESULT
            {
                if (env != nullptr)
                {
                    wv_env3 = env;
                }
                return S_OK;
            })
            .Get());

    wv_env3->CreateCoreWebView2Controller(
        window, Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                    [window](HRESULT result, ICoreWebView2Controller* controller) -> HRESULT
                    {
                        if (controller != nullptr)
                        {
                            wv_controller3 = controller;

                            wv_controller3->get_CoreWebView2(&wv_core3);
                            wv3 = wv_core3.try_query<ICoreWebView2_19>();

                            wv3->get_Settings(&wv_settings3);

                            wv3->Navigate(L"https://www.google.com/");

                            GetClientRect(window, &bounds);
                            wv_controller3->put_Bounds(bounds);
                        }
                        return S_OK;
                    })
                    .Get());

    // wv3->Navigate(L"https://www.bing.com/");
    // wv3 = wv_core3.try_query<ICoreWebView2_19>();
    // GetClientRect(window, &bounds);
    // wv_controller3->put_Bounds(bounds);
    // wv3->get_Settings(&wv_settings3);
    // wv_core3->Navigate(L"https://www.google.com/");
}
