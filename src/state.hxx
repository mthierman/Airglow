// clang-format off
// Airglow - https://github.com/mthierman/Airglow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#pragma once

#include <string>
#include <utility>

namespace airglow {
struct State {
    bool withArgs {};
    std::pair<std::string, std::string> args {};
    // glow::Colors colors {};
    std::pair<std::string, std::string> home {};

    // NLOHMANN_DEFINE_TYPE_INTRUSIVE(State, args, colors, home)
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(State, args, home)
};
}; // namespace airglow
