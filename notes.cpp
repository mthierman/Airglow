void TestAppData() {
  PWSTR path;
  auto appdata =
      SHGetKnownFolderPath(&FOLDERID_LocalAppData, 0, nullptr, &path);
  // if (SUCCEEDED(appdata)) {
  //   OutputDebugStringW(path);
  // }

  CoTaskMemFree(path);
}

using namespace winrt;
using namespace winrt::Windows;
using namespace winrt::Windows::Foundation;
using namespace winrt::Microsoft::Web::WebView2::Core;

// IAsyncOperation<CoreWebView2Environment> CreateWebView2Env(HWND hWnd)
//{
//     co_await winrt::resume_background();
//     co_return co_await CoreWebView2Environment::CreateAsync();
// }
//
// IAsyncOperation<CoreWebView2Controller> CreateWebView2Controller(HWND hWnd,
// CoreWebView2Environment env)
//{
//     CoreWebView2ControllerWindowReference wv2_handle =
//     CoreWebView2ControllerWindowReference::CreateFromWindowHandle(reinterpret_cast<std::uint64_t>(hWnd));
//     co_return co_await env.CreateCoreWebView2ControllerAsync(wv2_handle);
// }

IAsyncAction InitWebView2(HWND hWnd) {
  auto wv2_handle =
      CoreWebView2ControllerWindowReference::CreateFromWindowHandle(
          reinterpret_cast<std::uint64_t>(hWnd));
  CoreWebView2Environment env = co_await CoreWebView2Environment::CreateAsync();
  CoreWebView2Controller controller =
      co_await env.CreateCoreWebView2ControllerAsync(wv2_handle);
  CoreWebView2 webview = controller.CoreWebView2();
  CoreWebView2Settings settings = webview.Settings();
  settings.IsScriptEnabled(true);
  settings.AreDefaultScriptDialogsEnabled(true);
  settings.IsWebMessageEnabled(true);
  if (webview) {
    OutputDebugStringW(L"CoreWebView2 initialized!\n");
  };
  if (settings) {
    OutputDebugStringW(L"CoreWebView2 settings initialized!\n");
  }
  RECT bounds;
  GetClientRect(hWnd, &bounds);
  float x = static_cast<float>(bounds.left);
  float y = static_cast<float>(bounds.right);
  float width = static_cast<float>(bounds.right - bounds.left);
  float height = static_cast<float>(bounds.bottom - bounds.top);
  Rect newbounds = Rect(x, y, width, height);
  controller.Bounds(newbounds);
  webview.Navigate(L"https://www.google.com/");
}

CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
// init_apartment(apartment_type::multi_threaded);
