// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#pragma once

#include <Windows.h>

#include <set>
#include <string>
#include <utility>

#include <nlohmann/json.hpp>

#include <glow/glow.hxx>

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

auto log(std::string string) -> void;

enum class msg : unsigned int
{
    notify = WM_APP,
    settings_toggle,
    settings_save,
    window_new,
    window_close,
    setting_change,
    dpi_change,
    layout_change,
    web_message_received,
    source_changed,
    favicon_changed,
    title_changed,
    home_changed,
    focus_changed
};
