// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#pragma once

#include <cstdint>
#include <string>
#include <type_traits>

#define CUSTOM_CREATE_WINDOW (WM_APP + 1)
#define CUSTOM_CLOSE_WINDOW (WM_APP + 2)
#define CUSTOM_POST_MAINURL (WM_APP + 3)
#define CUSTOM_POST_SIDEURL (WM_APP + 4)
#define CUSTOM_RECEIVE_MAINURL (WM_APP + 5)
#define CUSTOM_RECEIVE_SIDEURL (WM_APP + 6)

namespace airglow
{

enum struct Browsers : int64_t
{
    settings = 1,
    browser1 = 2,
    browser2 = 3,
    url = 4,
};

constexpr auto operator+(Browsers browsers) noexcept
{
    return static_cast<std::underlying_type_t<Browsers>>(browsers);
}

struct NotificationMsg
{
    NMHDR nmhdr{};
    std::string message{};
};

struct WindowDimensions
{
    RECT rect{};
    float scale{};
};

} // namespace airglow
