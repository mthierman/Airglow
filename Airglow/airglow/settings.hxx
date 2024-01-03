// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#pragma once

#include <fstream>
#include <print>

#include <nlohmann/json.hpp>

#include <glow/filesystem.hxx>

#include <airglow/config.hxx>

namespace airglow
{

struct Settings
{
    std::string m_name{config::name};
    std::string m_version{config::version};
    int m_width{};
    int m_height{};
};

void to_json(nlohmann::json& j, const Settings& settings);
void from_json(const nlohmann::json& j, Settings& settings);

auto save_settings(Settings settings) -> void;
auto load_settings(Settings settings) -> void;

} // namespace airglow
