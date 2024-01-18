// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#pragma once

#include <Windows.h>

#include <cstdint>
#include <set>

struct Keys
{
    Keys();

    std::set<unsigned int> set;
};

namespace msg
{
inline constexpr unsigned int notify{WM_APP};

inline constexpr unsigned int window_create{WM_APP + 1};
inline constexpr unsigned int window_close{WM_APP + 2};

inline constexpr unsigned int url_created{WM_APP + 3};
inline constexpr unsigned int url_height{WM_APP + 4};

inline constexpr unsigned int post_first{WM_APP + 5};
inline constexpr unsigned int post_second{WM_APP + 6};

inline constexpr unsigned int receive_first{WM_APP + 7};
inline constexpr unsigned int receive_second{WM_APP + 8};

inline constexpr unsigned int toggle_settings{WM_APP + 9};
inline constexpr unsigned int save_settings{WM_APP + 10};
} // namespace msg
