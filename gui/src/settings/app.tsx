import { useEffect, useState } from "react";
import iconRaw from "../../../data/icon.svg?raw";

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

    const handleClipboard = async (page: string) => {
        await navigator.clipboard.writeText(page);
    };

    const handleClear = () => {
        window.chrome.webview.postMessage({
            clear: true,
        });
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
            <div className="grid grid-flow-col grid-cols-2">
                <label className="flex flex-col">
                    <div className="z-50 grid gap-2 bg-accent p-2 text-2xl font-semibold leading-none text-black shadow-lg shadow-neutral-400 outline-none dark:shadow-neutral-950">
                        <span className="select-none hover:cursor-pointer">
                            📡 Home
                        </span>
                        <span className="truncate text-sm">
                            <a
                                className="hover:cursor-pointer"
                                onClick={() => {
                                    handleClipboard(settings.mainHomepage);
                                }}>
                                {settings.mainHomepage === ""
                                    ? "unset"
                                    : settings.mainHomepage}
                            </a>
                        </span>
                    </div>
                    <input
                        className="z-40 bg-accent p-2 text-black shadow-lg shadow-neutral-400 outline-none dark:shadow-neutral-950"
                        type="text"
                        name="mainHomepage"
                        id="mainHomepage"></input>
                </label>

                <label className="flex flex-col">
                    <div className="z-50 grid gap-2 truncate bg-neutral-200 p-2 text-2xl font-semibold leading-none shadow-lg shadow-neutral-400 outline-none dark:bg-neutral-800 dark:shadow-neutral-950">
                        <span className="select-none hover:cursor-pointer">
                            🛰️ Sidebar
                        </span>
                        <span className="truncate text-sm">
                            <a
                                className="hover:cursor-pointer"
                                onClick={() => {
                                    handleClipboard(settings.sideHomepage);
                                }}>
                                {settings.sideHomepage === ""
                                    ? "unset"
                                    : settings.sideHomepage}
                            </a>
                        </span>
                    </div>
                    <input
                        className="z-40 bg-neutral-200 p-2 shadow-lg shadow-neutral-400 outline-none dark:bg-neutral-800 dark:shadow-neutral-950"
                        type="text"
                        name="sideHomepage"
                        id="sideHomepage"></input>
                </label>
            </div>

            <div className="flex items-center gap-4 p-4 text-lg leading-none">
                <div className="flex flex-col items-center gap-4">
                    <div className="select-none capitalize">
                        {themeIcon + " " + settings.appTheme + " Mode"}
                    </div>

                    <button
                        className="rounded-lg bg-red-800 p-2 text-white shadow-lg shadow-neutral-400 duration-100 hover:scale-95 active:scale-90 active:bg-green-600 dark:shadow-neutral-950"
                        id="clear"
                        type="button"
                        onClick={handleClear}>
                        Clear User Data
                    </button>
                </div>

                <button
                    className="ml-auto mt-auto rounded-lg p-2 shadow-lg shadow-neutral-400 duration-100 hover:scale-95 active:scale-90 active:bg-green-600 dark:shadow-neutral-950"
                    id="submitUrl"
                    type="submit">
                    Save
                </button>
            </div>
        </form>
    );
}
