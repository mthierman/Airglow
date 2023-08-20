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
    const [mainHomepage, setMainHomepage] = useState("");
    const [sideHomepage, setSideHomepage] = useState("");
    const [appTheme, setAppTheme] = useState("");
    const [themeIcon, setThemeIcon] = useState("");

    if (window.chrome.webview) {
        window.chrome.webview.addEventListener("message", (arg: any) => {
            if (arg.data.colors) {
                document.documentElement.style.setProperty(
                    "--colorAccent",
                    arg.data.colors.colorAccent,
                );
                document.documentElement.style.setProperty(
                    "--colorAccentDark1",
                    arg.data.colors.colorAccentDark1,
                );
                document.documentElement.style.setProperty(
                    "--colorAccentDark2",
                    arg.data.colors.colorAccentDark2,
                );
                document.documentElement.style.setProperty(
                    "--colorAccentDark3",
                    arg.data.colors.colorAccentDark3,
                );
                document.documentElement.style.setProperty(
                    "--colorAccentLight1",
                    arg.data.colors.colorAccentLight1,
                );
                document.documentElement.style.setProperty(
                    "--colorAccentLight2",
                    arg.data.colors.colorAccentLight2,
                );
                document.documentElement.style.setProperty(
                    "--colorAccentLight3",
                    arg.data.colors.colorAccentLight3,
                );
            }
            if (arg.data.settings) {
                setMainHomepage(arg.data.settings.mainHomepage);
                setSideHomepage(arg.data.settings.sideHomepage);
                setAppTheme(arg.data.settings.appTheme);
            }
        });
    }

    useEffect(() => {
        if (appTheme) {
            if (appTheme === "dark") {
                setThemeIcon("🌙︎");
            }
            if (appTheme === "light") {
                setThemeIcon("🔆︎");
            }
        }
    }, [appTheme]);

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
        <div>
            <form
                className="grid h-full content-between gap-6 p-6"
                name="settings"
                id="settings"
                method="post"
                onSubmit={handleForm}
                autoComplete="off"
                spellCheck="false">
                <div className="grid gap-6 self-start">
                    <label className="grid gap-2 whitespace-nowrap">
                        <div className="grid grid-flow-col items-end justify-between gap-6 truncate">
                            <div className="select-none text-3xl">📡 Home</div>
                            <div className="select-text text-accentDark2 dark:text-accent">
                                {mainHomepage}
                            </div>
                        </div>
                        <input
                            className="rounded-lg border-[1px] bg-transparent p-2 shadow-md shadow-neutral-400 outline-none focus:border-accentDark2 dark:shadow-black dark:focus:border-accent"
                            type="text"
                            name="mainHomepage"
                            id="mainHomepage"></input>
                    </label>

                    <label className="grid gap-2 whitespace-nowrap">
                        <div className="grid grid-flow-col items-end justify-between gap-6 truncate">
                            <div className="select-none text-3xl">
                                🛰️ Sidebar
                            </div>
                            <div className="select-text text-accentDark2 dark:text-accent">
                                {sideHomepage}
                            </div>
                        </div>
                        <input
                            className="rounded-lg border-[1px] bg-transparent p-2 shadow-md shadow-neutral-400 outline-none focus:border-accentDark2 dark:shadow-black dark:focus:border-accent"
                            type="text"
                            name="sideHomepage"
                            id="sideHomepage"></input>
                    </label>
                </div>

                <div className="grid grid-flow-col justify-between">
                    <div className="select-none self-end capitalize">
                        {themeIcon} {appTheme} mode
                    </div>

                    <button
                        className="rounded-lg border-[1px] bg-transparent p-2 shadow-md shadow-neutral-400 duration-75 hover:border-accentDark2 hover:text-accentDark2 active:scale-95 dark:shadow-black dark:hover:border-accent dark:hover:text-accent"
                        id="submitUrl"
                        type="submit">
                        Save
                    </button>
                </div>
            </form>
        </div>
    );
}
