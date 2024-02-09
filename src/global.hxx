// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#pragma once

#include <set>
#include <string>
#include <utility>

#include <nlohmann/json.hpp>

#include <glow/glow.hxx>

enum class CODE : unsigned int
{
    SETTINGS_TOGGLE = WM_APP,
    SETTINGS_SAVE,
    WINDOW_NEW,
    WINDOW_CLOSE,
    BROWSER_INIT,
    SETTING_CHANGE,
    DPI_CHANGE,
    LAYOUT_CHANGE,
    WEB_MESSAGE_RECEIVED,
    SOURCE_CHANGED,
    FAVICON_CHANGED,
    TITLE_CHANGED,
    HOME_CHANGED,
    FOCUS_CHANGED
};

struct Keys
{
    Keys();

    std::set<unsigned int> set;
};

struct URL
{
    URL();

    std::pair<std::string, std::string> home;
    std::pair<std::string, std::string> current;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(URL, home, current)
};

struct Positions
{
    glow::Position full;
    glow::Position empty;
    glow::Position left;
    glow::Position right;
    glow::Position top;
    glow::Position bottom;
};

struct Layout
{
    Layout();

    int bar;
    int border;
    bool horizontal;
    bool split;
    bool swapped;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Layout, bar, border, horizontal, split, swapped)
};

struct Dimensions
{
    Dimensions();

    float devicePixelRatio;
    int offsetHeight;
    int offsetWidth;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Dimensions, devicePixelRatio, offsetHeight, offsetWidth)
};
