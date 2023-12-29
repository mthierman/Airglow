// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#include "Settings.hxx"

namespace Airglow
{

void to_json(json& j, const Settings& settings)
{
    j = json{{"name", settings.m_name},
             {"version", settings.m_version},
             {"width", settings.m_width},
             {"height", settings.m_height}};
}

void from_json(const json& j, Settings& settings)
{
    j.at("name").get_to(settings.m_name);
    j.at("version").get_to(settings.m_version);
    j.at("width").get_to(settings.m_width);
    j.at("height").get_to(settings.m_height);
}

} // namespace Airglow
