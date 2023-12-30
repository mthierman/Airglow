import { StrictMode } from "react";
import { createRoot } from "react-dom/client";
import { Button } from "./components/button";
import "./index.css";

function App() {
    return <Button text="Index" />;
}

createRoot(document.getElementById("root") as HTMLElement).render(
    <StrictMode>
        <App />
    </StrictMode>,
);
