// clang-format off
// ╔──────────────╗
// │ ╔═╗╦  ╔═╗╦ ╦ │  Glow - https://github.com/mthierman/Glow
// │ ║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚──────────────╝
// clang-format on

#pragma once

#include <glow/gui.hxx>

struct Frame
{
    Frame();
    virtual ~Frame();

    auto show_normal() -> void;
    auto show() -> void;
    auto hide() -> void;

    static auto CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    virtual auto handle_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT;

    virtual auto on_close(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> int;
    virtual auto on_destroy(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> int;

    inline static ATOM m_atom;
    wil::unique_hwnd m_hwnd{};

    wil::unique_hcursor m_hCursor{static_cast<HCURSOR>(
        LoadImageA(nullptr, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED | LR_DEFAULTSIZE))};
    wil::unique_hicon m_hIcon{static_cast<HICON>(
        LoadImageA(nullptr, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_SHARED | LR_DEFAULTSIZE))};
    wil::unique_hicon m_appIcon{static_cast<HICON>(LoadImageA(
        GetModuleHandleA(nullptr), MAKEINTRESOURCEA(101), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE))};
    wil::unique_hbrush m_hbrBackground{static_cast<HBRUSH>(GetStockObject(NULL_BRUSH))};
};
