// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#include "window.hxx"

Window::Window(HWND app, std::string mainUrl, std::string sideUrl) : BaseWindow("Airglow")
{
    m_app = app;
    notify(m_app, msg::window_create);

    m_mainUrl = mainUrl;
    m_sideUrl = sideUrl;

    dwm_caption_color(false);
    dwm_dark_mode(true);
    dwm_system_backdrop(DWM_SYSTEMBACKDROP_TYPE::DWMSBT_MAINWINDOW);

    m_main = std::make_unique<MainBrowser>(hwnd(), m_mainUrl);
    m_main->reveal();

    m_side = std::make_unique<SideBrowser>(hwnd(), m_sideUrl);
    m_side->reveal();

    m_url = std::make_unique<URLBrowser>(hwnd());
    m_url->reveal();
}

auto Window::default_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    switch (uMsg)
    {
    case WM_CLOSE: return on_close(wParam, lParam);
    case WM_KEYDOWN: return on_key_down(wParam, lParam);
    case WM_NOTIFY: return on_notify(wParam, lParam);
    case WM_SIZE: return on_size(wParam, lParam);
    }

    return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

auto Window::on_close(WPARAM wParam, LPARAM lParam) -> int
{
    notify(m_app, msg::window_close);

    return close();
}

auto Window::on_size(WPARAM wParam, LPARAM lParam) -> int
{
    WindowDimensions dimensions;
    GetClientRect(hwnd(), &dimensions.rect);
    dimensions.scale = scale();
    dimensions.hwnd = hwnd();
    EnumChildWindows(hwnd(), EnumChildProc, std::bit_cast<LPARAM>(&dimensions));
    Sleep(1);

    return 0;
}

auto Window::on_key_down(WPARAM wParam, LPARAM lParam) -> int
{
    switch (wParam)
    {
    case VK_PAUSE: OutputDebugStringA("PAUSE"); break;
    case 0x4C:
        if (GetKeyState(VK_CONTROL) & 0x8000) OutputDebugStringA("L");
        break;
    case 0x57:
        if (GetKeyState(VK_CONTROL) & 0x8000)
        {
            OutputDebugStringA("W");
            PostMessageA(m_hwnd.get(), WM_CLOSE, 0, 0);
        }
        break;
    case VK_F1: OutputDebugStringA("F1"); break;
    case VK_F2: OutputDebugStringA("F2"); break;
    case VK_F3: OutputDebugStringA("F3"); break;
    case VK_F4: OutputDebugStringA("F4"); break;
    case VK_F6: OutputDebugStringA("F6"); break;
    case VK_F8: OutputDebugStringA("F8"); break;
    case VK_F11: OutputDebugStringA("F11"); break;
    }

    return 0;
}

auto Window::on_notify(WPARAM wParam, LPARAM lParam) -> int
{
    auto nMsg{*std::bit_cast<NotificationMsg*>(lParam)};

    switch (nMsg.nmhdr.code)
    {
    case msg::post_height:
    {
        m_bar = std::stoi(nMsg.message);
        SendMessageA(hwnd(), WM_SIZE, 0, 0);
        break;
    }

    case msg::post_mainurl:
    {
        if (m_main) m_main->navigate(nMsg.message);
        break;
    }

    case msg::post_sideurl:
    {
        if (m_main) m_side->navigate(nMsg.message);
        break;
    }

    case msg::receive_mainurl:
    {
        nlohmann::json j{{"mainUrl", nMsg.message}};
        if (m_url) m_url->post_json(glow::text::widen(j.dump()).c_str());
        break;
    }

    case msg::receive_sideurl:
    {
        nlohmann::json j{{"sideUrl", nMsg.message}};
        if (m_url) m_url->post_json(glow::text::widen(j.dump()).c_str());
        break;
    }
    }

    return 0;
}

auto CALLBACK Window::EnumChildProc(HWND hWnd, LPARAM lParam) -> BOOL
{
    auto dimensions{*std::bit_cast<WindowDimensions*>(lParam)};
    auto self{glow::gui::instance_from_window_long_ptr<Window>(dimensions.hwnd)};

    if (self)
    {
        auto gwlId{static_cast<uint64_t>(GetWindowLongPtrA(hWnd, GWL_ID))};

        auto r = &dimensions.rect;
        auto width{r->right - r->left};
        auto height{r->bottom - r->top};

        auto border{static_cast<int>(s_border * dimensions.scale)};
        auto bar{static_cast<int>(self->m_bar * dimensions.scale)};

        auto defer{true};

        auto hdwp{BeginDeferWindowPos(4)};

        if (gwlId == self->m_main->id())
            if (hdwp)
                hdwp = DeferWindowPos(hdwp, hWnd, nullptr, 0, 0, (width / 2) - border, height - bar,
                                      SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOOWNERZORDER |
                                          SWP_NOREDRAW | SWP_NOCOPYBITS);

        if (gwlId == self->m_side->id())
            if (hdwp)
                hdwp = DeferWindowPos(hdwp, hWnd, nullptr, (width / 2) + border, 0,
                                      (width / 2) - border, height - bar,
                                      SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOOWNERZORDER |
                                          SWP_NOREDRAW | SWP_NOCOPYBITS);

        if (gwlId == self->m_url->id())
            if (hdwp)
                hdwp = DeferWindowPos(hdwp, hWnd, nullptr, 0, r->bottom - bar, width, bar,
                                      SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOOWNERZORDER |
                                          SWP_NOREDRAW | SWP_NOCOPYBITS);

        if (hdwp) EndDeferWindowPos(hdwp);
    }

    return TRUE;
}

auto Window::url_path() -> std::string
{
#if _DEBUG
    std::string path{"https://localhost:8000/url/index.html"};
#else
    auto path{"file:///" + filesystem::known_folder().string() + "\\Airglow\\gui\\url\\index.html"};
#endif

    return path;
}

auto Window::data_path() -> std::filesystem::path
{
    auto path{glow::filesystem::known_folder() / "Airglow"};

    if (!std::filesystem::exists(path)) std::filesystem::create_directory(path);

    return path;
}

auto Window::json_path() -> std::filesystem::path
{
    auto path{data_path() / "Airglow.json"};

    return path;
}

auto Window::save_settings() -> void
{
    nlohmann::json position = m_position;
    std::ofstream f(m_settingsFile);
    f << std::setw(4) << position << "\n";
    f.close();
}

auto Window::load_settings() -> void {}
