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
            settings.appTheme === "dark"
                ? setThemeIcon(`🌙`)
                : setThemeIcon(`☀️`);
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
            <div className="flex flex-row">
                <label className="flex flex-grow flex-col whitespace-nowrap">
                    <div className="select-none bg-accentLight3 p-2 text-2xl font-semibold shadow-lg shadow-neutral-300 outline-none dark:bg-accentDark3 dark:shadow-neutral-950">
                        📡 Home
                    </div>
                    <input
                        className="bg-transparent p-2 outline-none"
                        type="text"
                        name="mainHomepage"
                        id="mainHomepage"
                        placeholder={settings.mainHomepage}></input>
                </label>

                <label className="flex flex-grow flex-col whitespace-nowrap">
                    <div className="select-none bg-neutral-200 p-2 text-2xl font-semibold shadow-lg shadow-neutral-300 outline-none dark:bg-neutral-800 dark:shadow-neutral-950">
                        🛰️ Sidebar
                    </div>
                    <input
                        className="bg-transparent p-2 outline-none"
                        type="text"
                        name="sideHomepage"
                        id="sideHomepage"
                        placeholder={settings.sideHomepage}></input>
                </label>
            </div>

            <div className="grid grid-flow-col justify-between gap-4 p-4">
                <div className="select-none self-end capitalize">
                    {themeIcon} {settings.appTheme} mode
                </div>

                <button
                    className="rounded-lg p-2 shadow-lg shadow-neutral-300 duration-100 hover:scale-95 active:scale-90 dark:shadow-neutral-950 active:bg-green-600"
                    id="submitUrl"
                    type="submit">
                    Save
                </button>
            </div>
        </form>
    );
}
