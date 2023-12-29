import { StrictMode } from "react";
import { createRoot } from "react-dom/client";
import { FluentProvider, webDarkTheme, webLightTheme } from "@fluentui/react-components";

import App from "./app";
import "./index.css";

createRoot(document.getElementById("root") as HTMLElement).render(
    <StrictMode>
        <FluentProvider theme={webDarkTheme}>
            <App />
        </FluentProvider>
    </StrictMode>,
);
