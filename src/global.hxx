// clang-format off
// Airglow - https://github.com/mthierman/Airglow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#pragma once

#include <Windows.h>

#include <set>

namespace glow::messages {
enum struct notice : unsigned int {
    SINGLE_INSTANCE = WM_APP,
    TOGGLE_SETTINGS,
    SAVE_SETTINGS,
    CREATE_BROWSER,
    CLOSE_BROWSER,
    ACTIVE_BROWSER,
    WEB_MESSAGE_RECEIVED,
};
}

namespace airglow {
struct Keys {
    Keys();

    std::set<unsigned int> set {};
};
}; // namespace airglow
