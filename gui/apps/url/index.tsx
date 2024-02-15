import { applyFavicon } from "#libs/index";
import { StrictMode } from "react";
import { createRoot } from "react-dom/client";
import App from "./App";

applyFavicon();

createRoot(document.getElementById("root") as HTMLElement).render(
    <StrictMode>
        <App />
    </StrictMode>,
);
