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
#include <string>

#include <nlohmann/json.hpp>

struct Keys
{
    Keys();

    std::set<unsigned int> set;
};

struct URL
{
    URL();

    std::map<std::string, std::string> home;
    std::map<std::string, std::string> current;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(URL, home, current)
};

auto log(std::string string) -> void;

namespace msg
{
inline constexpr unsigned int notify{WM_APP};

inline constexpr unsigned int window_create{WM_APP + 1};
inline constexpr unsigned int window_close{WM_APP + 2};
inline constexpr unsigned int url_create{WM_APP + 3};

inline constexpr unsigned int web_message_received{WM_APP + 5};
inline constexpr unsigned int source_changed{WM_APP + 6};
inline constexpr unsigned int home_changed{WM_APP + 7};

inline constexpr unsigned int toggle_settings{WM_APP + 8};
inline constexpr unsigned int save_settings{WM_APP + 9};
} // namespace msg
