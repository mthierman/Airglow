import { StrictMode } from "react";
import { createRoot } from "react-dom/client";
import App from "./app";
import "./index.css";

import { FluentProvider, webDarkTheme, webLightTheme } from "@fluentui/react-components";

const root = createRoot(document.getElementById("app") as HTMLElement).render(
    <StrictMode>
        <FluentProvider theme={webDarkTheme}>
            <App />
        </FluentProvider>
    </StrictMode>,
);
