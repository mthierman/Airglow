import { useEffect, useState } from "react";
// import { Button } from "@components/Button";

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

    if (window.chrome.webview) {
        window.chrome.webview.addEventListener("message", (arg: any) => {
            if (arg.data.colors) {
                setColors(arg.data.colors);
            }
        });
    }

    useEffect(() => {
        document.documentElement.style.setProperty("--accent", colors.accent);
        document.documentElement.style.setProperty("--accentDark1", colors.accentDark1);
        document.documentElement.style.setProperty("--accentDark2", colors.accentDark2);
        document.documentElement.style.setProperty("--accentDark3", colors.accentDark3);
        document.documentElement.style.setProperty("--accentLight1", colors.accentLight1);
        document.documentElement.style.setProperty("--accentLight2", colors.accentLight2);
        document.documentElement.style.setProperty("--accentLight3", colors.accentLight3);
    }, [colors]);

    return (
        <div className="pointer-events-none flex h-full select-none items-center justify-center p-4 text-center text-2xl font-bold">
            Press ⏸️ to set homepage
        </div>
    );
}
