// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#pragma once

#include <Windows.h>

#include <map>
#include <memory>

#include <glow/glow.hxx>

#include "settings.hxx"
#include "state.hxx"
#include "window.hxx"

using json = nlohmann::json;

struct App final : public glow::App
{
    auto WndProc(::UINT uMsg, ::WPARAM wParam, ::LPARAM lParam) -> ::LRESULT override;
    auto on_create(::WPARAM wParam, ::LPARAM lParam) -> int;
    auto on_notify(::WPARAM wParam, ::LPARAM lParam) -> int;

    auto parse_args() -> void;
    auto new_window() -> void;
    auto data() -> std::filesystem::path;
    auto file() -> std::filesystem::path;
    auto save() -> void;
    auto load() -> void;

    glow::GdiPlus m_gdiInit{};
    glow::CoInitialize m_coInit{};
    State m_state{};
    std::unique_ptr<Settings> m_settings{};
    std::map<size_t, std::unique_ptr<Window>> m_windows{};
    size_t m_active{};
};
