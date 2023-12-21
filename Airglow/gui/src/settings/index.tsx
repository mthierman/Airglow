// clang-format off
// ╔─────────────────────────────╗
// │                             │
// │     ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦     │  Airglow - https://github.com/mthierman/Airglow
// │     ╠═╣║╠╦╝║ ╦║  ║ ║║║║     │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │     ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝     │  SPDX-License-Identifier: MIT
// │                             │
// ╚─────────────────────────────╝
// clang-format on

import { StrictMode } from "react";
import { createRoot } from "react-dom/client";
import App from "./app";
import "../app.css";

const root = createRoot(document.getElementById("root") as HTMLElement).render(
    <StrictMode>
        <App />
    </StrictMode>,
);
