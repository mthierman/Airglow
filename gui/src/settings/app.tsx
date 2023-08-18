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
                setThemeIcon("🌙");
            }
            if (appTheme === "light") {
                setThemeIcon("🔆");
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
                mainHomepage: data.mainHomepage,
            });
        }

        if (data.sideHomepage.toString() != "") {
            window.chrome.webview.postMessage({
                sideHomepage: data.sideHomepage,
            });
        }

        form.reset();
    };

    return (
        <div>
            <form
                name="settings"
                id="settings"
                method="post"
                onSubmit={handleForm}
                autoComplete="off"
                spellCheck="false">
                <div className="grid gap-6 self-start">
                    <label>
                        <div className="grid grid-flow-col items-end justify-between gap-6 truncate">
                            <div className="select-none text-3xl font-light">
                                🌌 Home
                            </div>
                            <div className="select-text text-neutral-400">
                                {mainHomepage}
                            </div>
                        </div>
                        <input
                            type="text"
                            name="mainHomepage"
                            id="mainHomepage"
                            placeholder={mainHomepage}
                            pattern=".*[.].*"></input>
                    </label>

                    <label>
                        <div className="grid grid-flow-col items-end justify-between gap-6 truncate">
                            <div className="select-none text-3xl font-light">
                                🛰️ Sidebar
                            </div>
                            <div className="select-text text-neutral-400">
                                {sideHomepage}
                            </div>
                        </div>
                        <input
                            type="text"
                            name="sideHomepage"
                            id="sideHomepage"
                            placeholder={sideHomepage}
                            pattern=".*[.].*"></input>
                    </label>
                </div>

                <div className="grid grid-flow-col justify-between">
                    <div className="select-none self-end capitalize">
                        {themeIcon} {appTheme} mode
                    </div>

                    <button id="submitUrl" type="submit">
                        Save
                    </button>
                </div>
            </form>
        </div>
    );
}
