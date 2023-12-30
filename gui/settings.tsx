import { StrictMode } from "react";
import { createRoot } from "react-dom/client";
import { Button } from "./components/button";
import "./index.css";

function Settings() {
    return <Button text="Settings" />;
}

createRoot(document.getElementById("root") as HTMLElement).render(
    <StrictMode>
        <Settings />
    </StrictMode>,
);
