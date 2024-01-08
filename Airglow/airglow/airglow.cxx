// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#include <airglow/airglow.hxx>

namespace airglow
{

auto data_path() -> std::filesystem::path
{
    auto path{filesystem::known_folder() / "Airglow"};

    if (!std::filesystem::exists(path)) std::filesystem::create_directory(path);

    return path;
}

auto json_path() -> std::filesystem::path
{
    auto path{data_path() / "Airglow.json"};

    return path;
}

auto App::run() -> int
{
    SetEnvironmentVariableA("WEBVIEW2_DEFAULT_BACKGROUND_COLOR", "0");
    SetEnvironmentVariableA("WEBVIEW2_ADDITIONAL_BROWSER_ARGUMENTS",
                            "--allow-file-access-from-files");
    std::string mainUrl{""};
    std::string sideUrl{""};

    try
    {
        auto argv = console::argv();
        if (!argv.at(1).empty()) mainUrl = argv.at(1);
        if (!argv.at(2).empty()) sideUrl = argv.at(2);
    }
    catch (const std::out_of_range& e)
    {
        console::debug(e.what());
    }

    m_browser = std::make_unique<Browser>(m_hwnd.get(), "Browser", mainUrl, sideUrl);
    (*m_browser)();

    // console::debug(gui::webview_version());

    // m_settings = std::make_unique<Settings>(m_hwnd.get(), "Settings");
    // (*m_settings)();

    // console::create_process("server.exe");

    return gui::message_loop();
}

auto App::create_window() -> void
{
    CreateWindowExA(0, m_wcex.lpszClassName, m_wcex.lpszClassName, 0, CW_USEDEFAULT, CW_USEDEFAULT,
                    CW_USEDEFAULT, CW_USEDEFAULT, HWND_MESSAGE, nullptr, GetModuleHandleA(nullptr),
                    this);
}

auto App::handle_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    switch (uMsg)
    {
    case WM_NOTIFY: return on_notify(hWnd, wParam, lParam);
    }

    return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

auto App::on_notify(HWND hWnd, WPARAM wParam, LPARAM lParam) -> int
{
    auto nmhdr{*std::bit_cast<LPNMHDR>(lParam)};

    switch (nmhdr.code)
    {
    case CUSTOM_CREATE_WINDOW: m_windows.insert(nmhdr.hwndFrom); break;
    case CUSTOM_CLOSE_WINDOW: m_windows.erase(nmhdr.hwndFrom); break;
    }

    if (m_windows.empty())
    {
        m_hwnd.reset();
        PostQuitMessage(0);
    }

    return 0;
}

Browser::Browser(HWND app, std::string className, std::string mainUrl, std::string sideUrl)
    : gui::Window(className)
{
    m_app = app;
    if (!mainUrl.empty()) m_mainUrl = mainUrl;
    if (!sideUrl.empty()) m_sideUrl = sideUrl;
}

auto Browser::operator()(bool show) -> void
{
    gui::Window::operator()(show);
    dwm_caption_color(false);
    dwm_dark_mode(true);
    dwm_system_backdrop(DWM_SYSTEMBACKDROP_TYPE::DWMSBT_MAINWINDOW);

    m_main = std::make_unique<wv::Main>(+Browsers::main, m_hwnd.get(), m_mainUrl);
    (*m_main)();
    m_main->create_webview();

    m_side = std::make_unique<wv::Side>(+Browsers::side, m_hwnd.get(), m_sideUrl);
    (*m_side)();
    m_side->create_webview();

    m_url = std::make_unique<wv::URL>(+Browsers::url, m_hwnd.get(), url_path());
    (*m_url)();
    m_url->create_webview();
}

auto Browser::handle_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    switch (uMsg)
    {
    case WM_CREATE: return on_create(hWnd, wParam, lParam);
    case WM_CLOSE: return on_close(hWnd, wParam, lParam);
    case WM_KEYDOWN: return on_key_down(hWnd, wParam, lParam);
    case WM_NOTIFY: return on_notify(hWnd, wParam, lParam);
    case WM_SIZE: return on_size(hWnd, wParam, lParam);
    }

    return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

auto Browser::on_create(HWND hWnd, WPARAM wParam, LPARAM lParam) -> int
{
    NMHDR nmhdr;
    nmhdr.code = CUSTOM_CREATE_WINDOW;
    nmhdr.hwndFrom = m_hwnd.get();
    nmhdr.idFrom = m_id;
    SendMessageA(m_app, WM_NOTIFY, nmhdr.idFrom, std::bit_cast<LPARAM>(&nmhdr));

    return 0;
}

auto Browser::on_close(HWND hWnd, WPARAM wParam, LPARAM lParam) -> int
{
    NMHDR nmhdr;
    nmhdr.code = CUSTOM_CLOSE_WINDOW;
    nmhdr.hwndFrom = m_hwnd.get();
    nmhdr.idFrom = m_id;
    SendMessageA(m_app, WM_NOTIFY, nmhdr.idFrom, std::bit_cast<LPARAM>(&nmhdr));

    m_hwnd.reset();

    return 0;
}

auto Browser::on_key_down(HWND hWnd, WPARAM wParam, LPARAM lParam) -> int
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

auto Browser::on_notify(HWND hWnd, WPARAM wParam, LPARAM lParam) -> int
{
    auto nMsg{*std::bit_cast<NotificationMsg*>(lParam)};

    switch (nMsg.nmhdr.code)
    {

    case CUSTOM_POST_HEIGHT:
    {
        m_bar = std::stoi(nMsg.message);
        SendMessageA(hWnd, WM_SIZE, 0, 0);
        break;
    }

    case CUSTOM_POST_MAINURL:
    {
        if (m_main) m_main->navigate(nMsg.message);
        break;
    }

    case CUSTOM_POST_SIDEURL:
    {
        if (m_main) m_side->navigate(nMsg.message);
        break;
    }

    case CUSTOM_RECEIVE_MAINURL:
    {
        nlohmann::json j{{"mainUrl", nMsg.message}};
        if (m_url) m_url->post_json(text::widen(j.dump()).c_str());
        break;
    }

    case CUSTOM_RECEIVE_SIDEURL:
    {
        nlohmann::json j{{"sideUrl", nMsg.message}};
        if (m_url) m_url->post_json(text::widen(j.dump()).c_str());
        break;
    }
    }

    return 0;
}

auto Browser::on_size(HWND hWnd, WPARAM wParam, LPARAM lParam) -> int
{
    WindowDimensions dimensions;
    GetClientRect(hWnd, &dimensions.rect);
    dimensions.scale = scale();
    dimensions.hwnd = hWnd;
    EnumChildWindows(hWnd, EnumChildProc, std::bit_cast<LPARAM>(&dimensions));
    Sleep(1);

    return 0;
}

auto CALLBACK Browser::EnumChildProc(HWND hWnd, LPARAM lParam) -> BOOL
{
    auto dimensions{*std::bit_cast<WindowDimensions*>(lParam)};
    auto self{gui::instance<Browser>(dimensions.hwnd)};

    if (self)
    {
        auto gwlId{static_cast<int64_t>(GetWindowLongPtrA(hWnd, GWL_ID))};

        auto r = &dimensions.rect;
        auto width{r->right - r->left};
        auto height{r->bottom - r->top};

        auto border{static_cast<int>(s_border * dimensions.scale)};
        auto bar{static_cast<int>(self->m_bar * dimensions.scale)};

        auto defer{true};

        auto hdwp{BeginDeferWindowPos(4)};

        if (gwlId == +Browsers::main)
            if (hdwp)
                hdwp = DeferWindowPos(hdwp, hWnd, nullptr, 0, 0, (width / 2) - border, height - bar,
                                      SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOOWNERZORDER |
                                          SWP_NOREDRAW | SWP_NOCOPYBITS);

        if (gwlId == +Browsers::side)
            if (hdwp)
                hdwp = DeferWindowPos(hdwp, hWnd, nullptr, (width / 2) + border, 0,
                                      (width / 2) - border, height - bar,
                                      SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOOWNERZORDER |
                                          SWP_NOREDRAW | SWP_NOCOPYBITS);

        if (gwlId == +Browsers::url)
            if (hdwp)
                hdwp = DeferWindowPos(hdwp, hWnd, nullptr, 0, r->bottom - bar, width, bar,
                                      SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOOWNERZORDER |
                                          SWP_NOREDRAW | SWP_NOCOPYBITS);

        if (hdwp) EndDeferWindowPos(hdwp);
    }

    return TRUE;
}

auto Browser::url_path() -> std::string
{
#if _DEBUG
    std::string path{"https://localhost:8000/url/index.html"};
#else
    auto path{"file:///" + filesystem::known_folder().string() + "\\Airglow\\gui\\url\\index.html"};
#endif

    return path;
}

auto Browser::save_settings() -> void
{
    auto path{filesystem::portable()};
    if (!path.empty())
    {
        auto settingsFile{path / "settings.json"};

        try
        {
            if (!std::filesystem::exists(settingsFile))
            {
                try
                {
                    nlohmann::json j = *this;
                    std::ofstream f(settingsFile);
                    f << std::setw(4) << j << "\n";
                    f.close();
                }
                catch (const std::exception& e)
                {
                    std::println("{}", e.what());
                }
            }
            else
            {
                try
                {
                    nlohmann::json j = *this;
                    std::ofstream f(settingsFile);
                    f << std::setw(4) << j << "\n";
                    f.close();
                }
                catch (const std::exception& e)
                {
                    std::println("{}", e.what());
                }
            }
        }
        catch (const std::filesystem::filesystem_error& e)
        {
            std::println("{}", e.what());
        }
    }
}

auto Browser::load_settings() -> void
{
    auto path{filesystem::portable()};
    if (!path.empty())
    {
        auto settingsFile{path / "settings.json"};

        try
        {
            if (!std::filesystem::exists(settingsFile))
            {
                try
                {
                    nlohmann::json j = *this;
                    std::ofstream f(settingsFile);
                    f << std::setw(4) << j << "\n";
                    f.close();
                }
                catch (const std::exception& e)
                {
                    std::println("{}", e.what());
                }
            }
            else
            {
                try
                {
                    //
                }
                catch (const std::exception& e)
                {
                    std::println("{}", e.what());
                }
            }
        }
        catch (const std::filesystem::filesystem_error& e)
        {
            std::println("{}", e.what());
        }
    }
}

void to_json(nlohmann::json& j, const Browser& browser)
{
    j = nlohmann::json{
        {"dpi", browser.m_dpi},
        {"scale", browser.m_scale},
        {"darkMode", browser.m_dark_mode},
        {"maximized", browser.m_maximized},
        {"fullscreen", browser.m_fullscreen},
        {"gui", browser.m_gui},
        {"split", browser.m_split},
        {"swapped", browser.m_swapped},
        {"horizontal", browser.m_horizontal},
        {"mainHome", browser.m_mainHome},
        {"sideHome", browser.m_sideHome},
        {"mainCurrent", browser.m_mainCurrent},
        {"sideCurrent", browser.m_sideCurrent},
    };
}

void from_json(const nlohmann::json& j, Browser& browser)
{
    j.at("dpi").get_to(browser.m_dpi);
    j.at("scale").get_to(browser.m_scale);
    j.at("darkMode").get_to(browser.m_dark_mode);
    j.at("maximized").get_to(browser.m_maximized);
    j.at("fullscreen").get_to(browser.m_fullscreen);
    j.at("gui").get_to(browser.m_gui);
    j.at("split").get_to(browser.m_split);
    j.at("swapped").get_to(browser.m_swapped);
    j.at("horizontal").get_to(browser.m_horizontal);
    j.at("mainHome").get_to(browser.m_mainHome);
    j.at("sideHome").get_to(browser.m_sideHome);
    j.at("mainCurrent").get_to(browser.m_mainCurrent);
    j.at("sideCurrent").get_to(browser.m_sideCurrent);
}

Settings::Settings(HWND app, std::string className) : gui::Window(className) { m_app = app; }

auto Settings::operator()(bool show) -> void
{
    gui::Window::operator()(show);
    dwm_caption_color(false);
    dwm_dark_mode(true);
    dwm_system_backdrop(DWM_SYSTEMBACKDROP_TYPE::DWMSBT_MAINWINDOW);

    m_browser = std::make_unique<wv::Settings>(+Browsers::settings, m_hwnd.get(),
                                               "https://localhost:8000/settings/index.html");
    (*m_browser)();
    m_browser->create_webview();
}

auto Settings::handle_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    switch (uMsg)
    {
    case WM_CREATE: return on_create(hWnd, wParam, lParam);
    case WM_CLOSE: return on_close(hWnd, wParam, lParam);
    case WM_SIZE: return on_size(hWnd, wParam, lParam);
    }

    return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

auto Settings::on_create(HWND hWnd, WPARAM wParam, LPARAM lParam) -> int
{
    NMHDR nmhdr;
    nmhdr.code = CUSTOM_CREATE_WINDOW;
    nmhdr.hwndFrom = m_hwnd.get();
    nmhdr.idFrom = m_id;
    SendMessageA(m_app, WM_NOTIFY, nmhdr.idFrom, std::bit_cast<LPARAM>(&nmhdr));

    return 0;
}

auto Settings::on_close(HWND hWnd, WPARAM wParam, LPARAM lParam) -> int
{
    NMHDR nmhdr;
    nmhdr.code = CUSTOM_CLOSE_WINDOW;
    nmhdr.hwndFrom = m_hwnd.get();
    nmhdr.idFrom = m_id;
    SendMessageA(m_app, WM_NOTIFY, nmhdr.idFrom, std::bit_cast<LPARAM>(&nmhdr));

    m_hwnd.reset();

    return 0;
}

auto Settings::on_size(HWND hWnd, WPARAM wParam, LPARAM lParam) -> int
{
    RECT rect{0};
    GetClientRect(hWnd, &rect);
    EnumChildWindows(hWnd, EnumChildProc, std::bit_cast<LPARAM>(&rect));
    Sleep(1);

    return 0;
}

auto CALLBACK Settings::EnumChildProc(HWND hWnd, LPARAM lParam) -> BOOL
{
    auto gwlId{static_cast<int64_t>(GetWindowLongPtrA(hWnd, GWL_ID))};
    auto rect{*std::bit_cast<RECT*>(lParam)};

    auto defer{true};

    if (defer)
    {
        auto hdwp{BeginDeferWindowPos(4)};

        if (gwlId == +Browsers::settings)
            if (hdwp)
                hdwp = DeferWindowPos(hdwp, hWnd, nullptr, 0, 0, rect.right - rect.left,
                                      rect.bottom - rect.top,
                                      SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOOWNERZORDER |
                                          SWP_NOREDRAW | SWP_NOCOPYBITS);

        if (hdwp) EndDeferWindowPos(hdwp);
    }

    else
    {
        if (gwlId == +Browsers::settings)
            SetWindowPos(hWnd, nullptr, 0, 0, rect.right - rect.left, rect.bottom - rect.top,
                         SWP_NOZORDER);
    }

    return TRUE;
}

} // namespace airglow
