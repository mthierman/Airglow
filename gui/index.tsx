import { StrictMode } from "react";
import { createRoot } from "react-dom/client";
import { Button } from "./components/button";
import "./index.css";

function Index() {
    return <Button text="Index" />;
}

createRoot(document.getElementById("root") as HTMLElement).render(
    <StrictMode>
        <Index />
    </StrictMode>,
);
