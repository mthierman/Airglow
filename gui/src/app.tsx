import { useState } from "react";
import iconRaw from "../../data/icons/icon.svg?raw";

document.onreadystatechange = () => {
    if (document.readyState === "complete") {
        const favicon = document.createElement("link");
        favicon.type = "image/svg+xml";
        favicon.rel = "icon";
        favicon.href = `data:image/svg+xml,${encodeURIComponent(iconRaw)}`;
        document.head.appendChild(favicon);
    }
};

export default function App() {
    const [colors, setColors] = useState({
        accent: "",
        accentDark1: "",
        accentDark2: "",
        accentDark3: "",
        accentLight1: "",
        accentLight2: "",
        accentLight3: "",
        foreground: "",
        background: "",
    });

    const accentColors = () => {
        document.documentElement.style.setProperty("--accent", colors.accent);

        document.documentElement.style.setProperty(
            "--accentDark1",
            colors.accentDark1,
        );

        document.documentElement.style.setProperty(
            "--accentDark2",
            colors.accentDark2,
        );

        document.documentElement.style.setProperty(
            "--accentDark3",
            colors.accentDark3,
        );

        document.documentElement.style.setProperty(
            "--accentLight1",
            colors.accentLight1,
        );

        document.documentElement.style.setProperty(
            "--accentLight2",
            colors.accentLight2,
        );

        document.documentElement.style.setProperty(
            "--accentLight3",
            colors.accentLight3,
        );
    };

    if (window.chrome.webview) {
        window.chrome.webview.addEventListener("message", (arg: any) => {
            if (arg.data.colors) {
                setColors(arg.data.colors);
                accentColors();
            }
        });
    }

    return <h1>Press ⏸️ to set homepage</h1>;
}
