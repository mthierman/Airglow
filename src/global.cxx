// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#include "global.hxx"

Keys::Keys()
{
    set.emplace(VK_PAUSE);
    set.emplace(0x30);
    set.emplace(0x4C);
    set.emplace(0x4E);
    set.emplace(0x57);
    set.emplace(VK_F1);
    set.emplace(VK_F2);
    set.emplace(VK_F3);
    set.emplace(VK_F4);
    set.emplace(VK_F6);
    set.emplace(VK_F7);
    set.emplace(VK_F8);
    set.emplace(VK_F9);
    set.emplace(VK_F10);
    set.emplace(VK_F11);
}

URL::URL() : home{"about:blank", "about:blank"}, current{"about:blank", "about:blank"} {}
