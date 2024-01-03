// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#pragma once

#define CUSTOM_CREATE_WINDOW (WM_APP + 1)
#define CUSTOM_CLOSE_WINDOW (WM_APP + 2)

#include <cstdint>
#include <type_traits>

namespace airglow
{

enum struct WebViews : int64_t
{
    browser1 = 1,
    browser2 = 2,
    bar1 = 3,
    bar2 = 4,
    settings = 5,
};

constexpr auto operator+(WebViews w) noexcept
{
    return static_cast<std::underlying_type_t<WebViews>>(w);
}

} // namespace Airglow
