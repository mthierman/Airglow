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

namespace msg
{
inline constexpr int64_t notify{(WM_APP)};
inline constexpr int64_t window_create{(WM_APP + 1)};
inline constexpr int64_t window_close{(WM_APP + 2)};
inline constexpr int64_t url_height{(WM_APP + 3)};
inline constexpr int64_t post_mainurl{(WM_APP + 4)};
inline constexpr int64_t post_sideurl{(WM_APP + 5)};
inline constexpr int64_t receive_mainurl{(WM_APP + 6)};
inline constexpr int64_t receive_sideurl{(WM_APP + 7)};
} // namespace msg
