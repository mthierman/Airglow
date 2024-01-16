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
#include <string>
#include <type_traits>

namespace key
{
inline constexpr unsigned int l{0x4C};
inline constexpr unsigned int w{0x57};
inline constexpr unsigned int pause{VK_PAUSE};
inline constexpr unsigned int f1{VK_F1};
inline constexpr unsigned int f2{VK_F2};
inline constexpr unsigned int f3{VK_F3};
inline constexpr unsigned int f4{VK_F4};
inline constexpr unsigned int f5{VK_F5};
inline constexpr unsigned int f6{VK_F6};
inline constexpr unsigned int f7{VK_F7};
inline constexpr unsigned int f8{VK_F8};
inline constexpr unsigned int f9{VK_F9};
inline constexpr unsigned int f10{VK_F10};
inline constexpr unsigned int f11{VK_F11};
inline constexpr unsigned int f12{VK_F12};
} // namespace key

namespace msg
{
inline constexpr unsigned int notify{WM_APP};
inline constexpr unsigned int window_create{WM_APP + 1};
inline constexpr unsigned int window_close{WM_APP + 2};
inline constexpr unsigned int url_created{WM_APP + 3};
inline constexpr unsigned int url_height{WM_APP + 4};
inline constexpr unsigned int post_mainurl{WM_APP + 5};
inline constexpr unsigned int post_sideurl{WM_APP + 6};
inline constexpr unsigned int receive_mainurl{WM_APP + 7};
inline constexpr unsigned int receive_sideurl{WM_APP + 8};
} // namespace msg
