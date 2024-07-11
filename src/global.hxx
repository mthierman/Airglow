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
    CREATE_WINDOW,
    CLOSE_WINDOW,
    ACTIVATE_WINDOW,
    SETTING_CHANGE,
    DPI_CHANGE,
    LAYOUT_CHANGE,
    WEB_MESSAGE_RECEIVED,
    SOURCE_CHANGED,
    FAVICON_CHANGED,
    TITLE_CHANGED,
    HOME_CHANGED,
    FOCUS_CHANGED,
};
}

struct Keys {
    Keys();

    std::set<unsigned int> set {};
};
