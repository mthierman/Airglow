import { useEffect, useState } from "react";
import icon from "../../../data/icons/icon.svg?raw";

document.onreadystatechange = () => {
    if (document.readyState === "complete") {
        const favicon = document.createElement("link");
        favicon.type = "image/svg+xml";
        favicon.rel = "icon";
        favicon.href = `data:image/svg+xml,${encodeURIComponent(icon)}`;
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

    const [settings, setSettings] = useState({
        appTheme: "",
        windowPosition: [0, 0, 0, 0],
        windowMaximized: false,
        windowFullscreen: false,
        windowTopmost: false,
        webviewGui: false,
        webviewSplit: false,
        webviewSwapped: false,
        webviewHorizontal: false,
        mainHomepage: "",
        mainCurrentPage: "",
        sideHomepage: "",
        sideCurrentPage: "",
    });

    const [themeIcon, setThemeIcon] = useState("");

    if (window.chrome.webview) {
        window.chrome.webview.addEventListener("message", (arg: any) => {
            if (arg.data.settings) setSettings(arg.data.settings);
            if (arg.data.colors) setColors(arg.data.colors);
        });
    }

    useEffect(() => {
        if (settings.appTheme) {
            if (settings.appTheme === "dark") {
                setThemeIcon(`🌙`);
            }
            if (settings.appTheme === "light") {
                setThemeIcon(`☀️`);
            }
        }
    }, [settings.appTheme]);

    useEffect(() => {
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
    }, [colors]);

    const handleForm = (e: any) => {
        e.preventDefault();

        const form = document.getElementsByName(
            "settings",
        )[0] as HTMLFormElement;

        const rawData = new FormData(e.target);
        const data = Object.fromEntries(rawData.entries());

        if (data.mainHomepage.toString() != "") {
            window.chrome.webview.postMessage({
                mainHomepage: data.mainHomepage.toString().trim(),
            });
        }

        if (data.sideHomepage.toString() != "") {
            window.chrome.webview.postMessage({
                sideHomepage: data.sideHomepage.toString().trim(),
            });
        }

        form.reset();
    };

    return (
        <form
            className="grid h-full content-between"
            name="settings"
            id="settings"
            method="post"
            onSubmit={handleForm}
            autoComplete="off"
            spellCheck="false">
            <div className="grid gap-4 p-4">
                <label className="grid whitespace-nowrap">
                    <div className="grid grid-flow-col items-end justify-between">
                        <div className="select-none text-xl font-semibold">
                            📡 Home
                        </div>
                        <div className="select-text truncate font-extralight">
                            {settings.mainHomepage}
                        </div>
                    </div>
                    <input
                        className="bg-transparent p-2 text-center shadow-lg shadow-neutral-300 outline-none dark:shadow-neutral-950"
                        type="text"
                        name="mainHomepage"
                        id="mainHomepage"></input>
                </label>

                <label className="grid whitespace-nowrap">
                    <div className="grid grid-flow-col items-end justify-between">
                        <div className="select-none text-xl font-semibold">
                            🛰️ Sidebar
                        </div>
                        <div className="select-text truncate font-extralight">
                            {settings.sideHomepage}
                        </div>
                    </div>
                    <input
                        className="bg-transparent p-2 text-center shadow-lg shadow-neutral-300 outline-none dark:shadow-neutral-950"
                        type="text"
                        name="sideHomepage"
                        id="sideHomepage"></input>
                </label>
            </div>

            <div className="grid grid-flow-col justify-between gap-4 p-4">
                <div className="select-none self-end capitalize">
                    {themeIcon} {settings.appTheme} mode
                </div>

                <button
                    className="rounded-lg p-2 shadow-lg shadow-neutral-300 duration-75 active:scale-95 dark:shadow-neutral-950"
                    id="submitUrl"
                    type="submit">
                    Save
                </button>
            </div>
        </form>
    );
}
