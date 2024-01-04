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

enum struct Browsers : int64_t
{
    settings = 1,
    browser1 = 2,
    browser2 = 3,
    addressBar = 4,
};

constexpr auto operator+(Browsers browsers) noexcept
{
    return static_cast<std::underlying_type_t<Browsers>>(browsers);
}

} // namespace airglow
