// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#pragma once

// #include <nlohmann/json.hpp>

#include <config/airglow.hxx>

//==============================================================================
namespace airglow
{

//==============================================================================
struct Settings
{
    std::string name{"test"};
    std::string version{"test"};
    int width{0};
    int height{0};
};

//==============================================================================
inline void to_json(nlohmann::json& j, const Settings& s)
{
    j = nlohmann::json{
        {"name", s.name}, {"version", s.version}, {"width", s.width}, {"height", s.height}};
}

//==============================================================================
inline void from_json(const nlohmann::json& j, Settings& s)
{
    j.at("name").get_to(s.name);
    j.at("version").get_to(s.version);
    j.at("width").get_to(s.width);
    j.at("height").get_to(s.height);
}

//==============================================================================
} // namespace airglow
