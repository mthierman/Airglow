import { StrictMode } from "react";
import { createRoot } from "react-dom/client";
import App from "./app";
import "../app.css";

const root = createRoot(document.getElementById("root") as HTMLElement).render(
    <StrictMode>
        <App />
    </StrictMode>,
);
