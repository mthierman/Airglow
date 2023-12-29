// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#pragma once

#include <nlohmann/json.hpp>

#include <config/airglow.hxx>

namespace Airglow
{
using json = nlohmann::json;

struct Settings
{
    std::string m_name{PROJECT_NAME};
    std::string m_version{PROJECT_VERSION};
    int m_width{};
    int m_height{};
};

void to_json(json& j, const Settings& settings);
void from_json(const json& j, Settings& settings);

} // namespace Airglow
