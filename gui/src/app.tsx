import { useEffect, useState } from "react";
import icon from "../../data/icons/new.svg?raw";

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
    const [homepageMain, setHomepageMain] = useState("");
    const [homepageSide, setHomepageSide] = useState("");
    const [theme, setTheme] = useState("");
    const [themeIcon, setThemeIcon] = useState("");

    if (window.chrome.webview) {
        window.chrome.webview.addEventListener("message", (arg: any) => {
            if (arg.data.colors) {
                document.documentElement.style.setProperty(
                    "--accent",
                    arg.data.colors.accent,
                );
                document.documentElement.style.setProperty(
                    "--accentDark1",
                    arg.data.colors.accentDark1,
                );
                document.documentElement.style.setProperty(
                    "--accentDark2",
                    arg.data.colors.accentDark2,
                );
                document.documentElement.style.setProperty(
                    "--accentDark3",
                    arg.data.colors.accentDark2,
                );
                document.documentElement.style.setProperty(
                    "--accentLight1",
                    arg.data.colors.accentLight1,
                );
                document.documentElement.style.setProperty(
                    "--accentLight2",
                    arg.data.colors.accentLight2,
                );
                document.documentElement.style.setProperty(
                    "--accentLight3",
                    arg.data.colors.accentLight3,
                );
            }
            if (arg.data.settings) {
                setHomepageMain(arg.data.settings.homepageMain);
                setHomepageSide(arg.data.settings.homepageSide);
                setTheme(arg.data.settings.theme);
            }
        });
    }

    useEffect(() => {
        if (theme) {
            if (theme === "dark") {
                setThemeIcon("🌙");
            }
            if (theme === "light") {
                setThemeIcon("🔆");
            }
        }
    }, [theme]);

    const handleForm = (e: any) => {
        e.preventDefault();

        const form = document.getElementsByName(
            "settings",
        )[0] as HTMLFormElement;

        const rawData = new FormData(e.target);
        const data = Object.fromEntries(rawData.entries());

        if (data.homepageMain.toString() != "") {
            window.chrome.webview.postMessage({
                homepageMain: data.homepageMain,
            });
        }

        if (data.homepageSide.toString() != "") {
            window.chrome.webview.postMessage({
                homepageSide: data.homepageSide,
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
                                🏠 Home
                            </div>
                            <div className="select-text rounded-lg bg-neutral-50 px-2 py-1 text-sm text-neutral-400 dark:bg-neutral-800">
                                {homepageMain}
                            </div>
                        </div>
                        <input
                            type="text"
                            name="homepageMain"
                            id="homepageMain"
                            placeholder={homepageMain}
                            pattern=".*[.].*"></input>
                    </label>

                    <label>
                        <div className="grid grid-flow-col items-end justify-between gap-6 truncate">
                            <div className="select-none text-3xl font-light">
                                🔧 Sidebar
                            </div>
                            <div className="select-text rounded-lg bg-neutral-50 px-2 py-1 text-sm text-neutral-400 dark:bg-neutral-800">
                                {homepageSide}
                            </div>
                        </div>
                        <input
                            type="text"
                            name="homepageSide"
                            id="homepageSide"
                            placeholder={homepageSide}
                            pattern=".*[.].*"></input>
                    </label>
                </div>

                <div className="grid grid-flow-col justify-stretch gap-6">
                    <div className="select-none">
                        <div>
                            <div className="capitalize">
                                {themeIcon} {theme} mode
                            </div>
                        </div>
                    </div>

                    <button id="submitUrl" type="submit">
                        Save
                    </button>
                </div>
            </form>
        </div>
    );
}
