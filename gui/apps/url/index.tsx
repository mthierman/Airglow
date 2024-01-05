import { StrictMode } from "react";
import { createRoot } from "react-dom/client";
import App from "./App";
import "@css/tailwind.css";
import "@css/index.css";

// https://javascript.plainenglish.io/how-to-detect-page-zoom-levels-in-modern-browsers-bdb6404496b
// https://forum.bubble.io/t/is-there-a-way-to-detect-browser-zoom-level/241928
// https://stackoverflow.com/questions/1713771/how-to-detect-page-zoom-level-in-all-modern-browsers
window.addEventListener("resize", () => {
    let devicePixelRatio = window.devicePixelRatio;
    console.log(devicePixelRatio);
    window.chrome.webview.postMessage({ devicePixelRatio: devicePixelRatio });
});

createRoot(document.getElementById("root") as HTMLElement).render(
    <StrictMode>
        <App />
    </StrictMode>,
);
