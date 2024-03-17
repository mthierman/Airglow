import { appFavicon } from "libs/common";
import { StrictMode } from "react";
import { createRoot } from "react-dom/client";
import App from "./App";

appFavicon();

createRoot(document.getElementById("root") as HTMLElement).render(
    <StrictMode>
        <App />
    </StrictMode>,
);
