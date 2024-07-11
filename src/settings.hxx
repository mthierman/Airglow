// clang-format off
// Airglow - https://github.com/mthierman/Airglow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#pragma once

#include <memory>

#include <glow/window.hxx>
#include <nlohmann/json.hpp>

// #include "browser.hxx"
// #include "global.hxx"
// #include "state.hxx"

namespace airglow{
using json = nlohmann::json;

struct Settings final : glow::window::Window {
    Settings();

    // std::unique_ptr<Browser> m_browser {};
    // Keys m_keys {};

private:
    // static auto EnumChildProc(::HWND hWnd, ::LPARAM lParam) -> ::BOOL;

    // NLOHMANN_DEFINE_TYPE_INTRUSIVE(Settings, m_state)
};
};
