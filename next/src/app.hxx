// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#include <config/airglow.hxx>

#include <gui/app.hxx>
#include <gui/gui.hxx>
#include <gui/webview.hxx>

//==============================================================================
namespace airglow
{

struct WinMain
{
    WinMain();
};

//==============================================================================
struct App final : public glow::gui::App
{
    using glow::gui::App::App;
    auto run() -> void;

  private:
    auto handle_message(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT override;
    static auto enum_child_proc(HWND hwnd, LPARAM lParam) -> BOOL;

    //==============================================================================
    auto on_notify() -> int;
    auto on_window_pos_changed() -> int;

    std::unique_ptr<glow::gui::WebView> wv1{
        std::make_unique<glow::gui::WebView>("webview1", m_hwnd.get(), 1)};

    std::unique_ptr<glow::gui::WebView> wv2{
        std::make_unique<glow::gui::WebView>("webview2", m_hwnd.get(), 2)};
};

//==============================================================================
} // namespace airglow
