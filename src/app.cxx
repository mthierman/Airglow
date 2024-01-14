// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#include "app.hxx"

auto App::operator()() -> int
{
    try
    {
        env();
        args();

        m_mainWindow = std::make_unique<Window>(hwnd(), m_mainUrl, m_sideUrl);
        m_mainWindow->reveal();

        m_settingsWindow = std::make_unique<Settings>(hwnd());
        m_settingsWindow->reveal();
    }
    catch (std::exception& e)
    {
        glow::console::message_box(e.what());
    }

    return glow::gui::message_loop();
}

auto App::env() -> void
{
    SetEnvironmentVariableA("WEBVIEW2_DEFAULT_BACKGROUND_COLOR", "0");
    SetEnvironmentVariableA("WEBVIEW2_ADDITIONAL_BROWSER_ARGUMENTS",
                            "--allow-file-access-from-files");
}

auto App::args() -> void
{
    if (m_argv.size() == 2) { m_mainUrl = m_argv.at(1); }

    if (m_argv.size() > 2)
    {
        m_mainUrl = m_argv.at(1);
        m_sideUrl = m_argv.at(2);
    }
}

auto App::wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    switch (uMsg)
    {
    case WM_NOTIFY: return on_notify(wParam, lParam);
    }

    return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

auto App::on_notify(WPARAM wParam, LPARAM lParam) -> int
{
    auto nmhdr{std::bit_cast<LPNMHDR>(lParam)};

    switch (nmhdr->code)
    {
    case msg::window_create: m_windowSet.insert(nmhdr->idFrom); break;
    case msg::window_close: m_windowSet.erase(nmhdr->idFrom); break;
    }

    if (m_windowSet.empty()) { return close(); }

    else return 0;
}

auto App::data_path() -> std::filesystem::path
{
    auto path{glow::filesystem::known_folder() / "Airglow"};

    if (!std::filesystem::exists(path)) std::filesystem::create_directory(path);

    return path;
}

auto App::json_path() -> std::filesystem::path
{
    auto path{data_path() / "Airglow.json"};

    return path;
}

auto App::save_settings() -> void
{
    // nlohmann::json position = m_position;
    // std::ofstream f(m_settingsFile);
    // f << std::setw(4) << position << "\n";
    // f.close();
}

auto App::load_settings() -> void {}
