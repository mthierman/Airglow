import { useEffect, useState } from "react";
import iconRaw from "../../../data/release.svg?raw";

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

    if (window.chrome.webview) {
        window.chrome.webview.addEventListener("message", (arg: any) => {
            if (arg.data === "focus") {
                const mainInput = document.getElementById(
                    "mainCurrentPage",
                ) as HTMLInputElement;
                const sideInput = document.getElementById(
                    "sideCurrentPage",
                ) as HTMLInputElement;

                if (!settings.webviewGui && !settings.webviewSwapped)
                    mainInput.focus();
                if (!settings.webviewGui && settings.webviewSwapped)
                    sideInput.focus();
            }
            if (arg.data.settings) setSettings(arg.data.settings);
            if (arg.data.colors) setColors(arg.data.colors);
        });
    }

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

        const form = document.getElementsByName("url")[0] as HTMLFormElement;

        const rawData = new FormData(e.target);
        const data = Object.fromEntries(rawData.entries());

        if (data.mainCurrentPage.toString() != "") {
            window.chrome.webview.postMessage({
                mainCurrentPage: data.mainCurrentPage.toString().trim(),
            });
        }

        if (data.sideCurrentPage.toString() != "") {
            window.chrome.webview.postMessage({
                sideCurrentPage: data.sideCurrentPage.toString().trim(),
            });
        }

        form.reset();
    };

    const handleDevtools = (webview: string) => {
        if (webview === "main")
            window.chrome.webview.postMessage({
                mainDevtools: true,
            });

        if (webview === "side")
            window.chrome.webview.postMessage({
                sideDevtools: true,
            });
    };

    const handleHome = (webview: string) => {
        if (webview === "main")
            window.chrome.webview.postMessage({
                mainHome: true,
            });

        if (webview === "side")
            window.chrome.webview.postMessage({
                sideHome: true,
            });
    };

    const handleClipboard = async (page: string) => {
        await navigator.clipboard.writeText(page);
    };

    const isBlank = (page: string) => {
        return page.includes("Airglow/gui") ||
            page === "https://localhost:8000/"
            ? true
            : false;
    };

    return (
        <div className="flex h-full flex-col overflow-clip">
            <form
                className={
                    "flex flex-grow font-semibold shadow-lg shadow-neutral-400 dark:shadow-neutral-950 " +
                    (settings.webviewSwapped ? "flex-row-reverse" : "")
                }
                name="url"
                id="url"
                method="post"
                onSubmit={handleForm}
                autoComplete="off"
                spellCheck="false">
                <label
                    className={
                        "flex min-w-0 flex-1 select-none items-center bg-accent px-2 py-1 text-black" +
                        (!settings.webviewSplit && settings.webviewSwapped
                            ? "hidden"
                            : "")
                    }>
                    <span className="hover:cursor-pointer">🔍</span>
                    <input
                        className="flex min-w-0 flex-1 bg-transparent px-2 text-center outline-none"
                        type="text"
                        name="mainCurrentPage"
                        id="mainCurrentPage"></input>
                    <div className="select-none self-center bg-accent hover:cursor-pointer">
                        <a
                            className="hover:cursor-pointer"
                            onClick={() => {
                                handleHome("main");
                            }}>
                            📡
                        </a>
                    </div>
                </label>

                <label
                    className={
                        "flex min-w-0 flex-1 select-none items-center bg-neutral-200 px-2 py-1 dark:bg-neutral-800 " +
                        (!settings.webviewSplit && !settings.webviewSwapped
                            ? "hidden"
                            : "")
                    }>
                    <span className="hover:cursor-pointer">🔍</span>
                    <input
                        className="flex min-w-0 flex-1 bg-transparent px-2 text-center outline-none"
                        type="text"
                        name="sideCurrentPage"
                        id="sideCurrentPage"></input>
                    <div className="select-none self-center bg-neutral-200 hover:cursor-pointer dark:bg-neutral-800">
                        <a
                            className="hover:cursor-pointer"
                            onClick={() => {
                                handleHome("side");
                            }}>
                            🛰️
                        </a>
                    </div>
                </label>

                <input type="submit" hidden />
            </form>

            <div
                className={
                    settings.webviewSwapped
                        ? "flex flex-row-reverse p-2 text-sm"
                        : "flex p-2 text-sm"
                }>
                <address
                    className={
                        "flex min-w-0 flex-1 flex-shrink items-center not-italic " +
                        (!settings.webviewSplit && settings.webviewSwapped
                            ? "hidden"
                            : "")
                    }>
                    <span className="flex-grow truncate px-4 text-center">
                        <a
                            className="select-none pr-2 hover:cursor-pointer"
                            onClick={() => {
                                handleDevtools("main");
                            }}>
                            {isBlank(settings.mainCurrentPage) ? `` : `🔧`}
                        </a>
                        {isBlank(settings.mainCurrentPage) ? (
                            ``
                        ) : (
                            <a
                                className="hover:cursor-pointer"
                                onClick={() => {
                                    handleClipboard(settings.mainCurrentPage);
                                }}>
                                {settings.mainCurrentPage}
                            </a>
                        )}
                    </span>
                </address>
                <address
                    className={
                        "flex min-w-0 flex-1 flex-shrink items-center not-italic " +
                        (!settings.webviewSplit && !settings.webviewSwapped
                            ? "hidden"
                            : "")
                    }>
                    <span className="flex-grow truncate px-4 text-center">
                        <a
                            className="select-none pr-2 hover:cursor-pointer"
                            onClick={() => {
                                handleDevtools("side");
                            }}>
                            {isBlank(settings.sideCurrentPage) ? `` : `🔧`}
                        </a>
                        {isBlank(settings.sideCurrentPage) ? (
                            ``
                        ) : (
                            <a
                                className="hover:cursor-pointer"
                                onClick={() => {
                                    handleClipboard(settings.sideCurrentPage);
                                }}>
                                {settings.sideCurrentPage}
                            </a>
                        )}
                    </span>
                </address>
            </div>
        </div>
    );
}
