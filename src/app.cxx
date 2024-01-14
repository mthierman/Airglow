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
        SetEnvironmentVariableA("WEBVIEW2_DEFAULT_BACKGROUND_COLOR", "0");
        SetEnvironmentVariableA("WEBVIEW2_ADDITIONAL_BROWSER_ARGUMENTS",
                                "--allow-file-access-from-files");

        auto argv = glow::console::argv();

        if (argv.size() == 2) { m_mainUrl = argv.at(1); }

        if (argv.size() > 2)
        {
            m_mainUrl = argv.at(1);
            m_sideUrl = argv.at(2);
        }

        m_mainWindow = std::make_unique<Window>(hwnd());
        m_mainWindow->reveal();

        m_settingsWindow = std::make_unique<Settings>(hwnd());
        m_settingsWindow->reveal();

        // for (auto i = 0; i < 4; i++)
        // {
        //     m_windowVector.emplace_back(std::make_unique<Window>(hwnd()))->reveal();
        // }
    }
    catch (std::exception& e)
    {
        glow::console::message_box(e.what());
    }

    return glow::gui::message_loop();
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
    auto nmhdr{*std::bit_cast<LPNMHDR>(lParam)};

    switch (nmhdr.code)
    {
    case msg::window_create: m_windowSet.insert(nmhdr.idFrom); break;
    case msg::window_close: m_windowSet.erase(nmhdr.idFrom); break;
    }

    if (m_windowSet.empty()) { return close(); }

    else return 0;
}
