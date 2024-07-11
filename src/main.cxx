// clang-format off
// Airglow - https://github.com/mthierman/Airglow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#include "app.hxx"

#include <glow/system.hxx>

auto main(int argc, char* argv[]) -> int {
    auto coInit { glow::system::co_initialize() };

    auto args { std::vector<std::string>(argv, argv + argc) };

    glow::system::Event singleInstance;

    if (singleInstance.create("SingleInstance")) {
        return EXIT_SUCCESS;
    }

    App app(args, singleInstance);

    return app();
}
