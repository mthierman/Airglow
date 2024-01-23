import { StrictMode } from "react";
import { createRoot } from "react-dom/client";
import App from "./App";
import "@css/index.css";
import "@css/home.css";

createRoot(document.getElementById("root") as HTMLElement).render(
    <StrictMode>
        <App />
    </StrictMode>,
);
