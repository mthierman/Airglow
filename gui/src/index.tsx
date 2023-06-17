import { StrictMode } from "react";
import { createRoot } from "react-dom/client";
import App from "./app";
import "./index.css";

const root = createRoot(document.getElementById("root") as HTMLElement).render(
    <StrictMode>
        <App />
    </StrictMode>
);

document.onreadystatechange = () => {
    if (document.readyState === "complete") {
        let scheme = document.createElement("meta");
        scheme.setAttribute("name", "color-scheme");
        scheme.setAttribute("content", "light dark");
        document.getElementsByTagName("head")[0].appendChild(scheme);
    }
};

document.documentElement.style.setProperty("color-scheme", "light dark");
