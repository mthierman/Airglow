// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#pragma once

#include <string>
#include <utility>

#include <glow/glow.hxx>

struct State
{
    bool withArgs{};
    std::pair<std::string, std::string> args{};
    glow::Colors colors{};
    std::pair<std::string, std::string> home{};

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(State, args, colors, home)
};
