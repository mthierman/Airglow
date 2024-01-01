import { StrictMode } from "react";
import { createRoot } from "react-dom/client";
import { Button } from "./components/button";
import "./index.css";

function AddressBar() {
    return <Button text="AddressBar" />;
}

createRoot(document.getElementById("root") as HTMLElement).render(
    <StrictMode>
        <AddressBar />
    </StrictMode>,
);
