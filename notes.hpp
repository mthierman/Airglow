void SetMica(HWND hwnd) {
  HRESULT hr = S_OK;
  auto mica = DWM_SYSTEMBACKDROP_TYPE::DWMSBT_MAINWINDOW;
  hr = DwmSetWindowAttribute(hwnd, DWMWA_SYSTEMBACKDROP_TYPE, &mica,
                             sizeof(&mica));
}

void SetTitlebar(HWND hwnd) {
  auto color = 0x00505050;
  HRESULT hr = S_OK;
  hr = DwmSetWindowAttribute(hwnd, DWMWINDOWATTRIBUTE::DWMWA_CAPTION_COLOR,
                             &color, sizeof(color));
}

void ExtendFrame(HWND hwnd) {
  // MARGINS m = {-1};
  MARGINS m = {0, 0, -500, -1};
  HRESULT hr = S_OK;
  hr = DwmExtendFrameIntoClientArea(hwnd, &m);
  if (SUCCEEDED(hr)) {
    SetMica(hwnd);
    // SetTitleBar(hwnd);
    //   COLORREF DARK_COLOR = 0x00505050;
    //   BOOL SET_CAPTION_COLOR = SUCCEEDED(
    //       DwmSetWindowAttribute(hwnd,
    //       DWMWINDOWATTRIBUTE::DWMWA_CAPTION_COLOR,
    //                             &DARK_COLOR, sizeof(DARK_COLOR)));
    // }
  }
