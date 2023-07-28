import { useEffect, useState } from "react";
import icon from "../../data/icons/icon.svg?raw";

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
    const [mainUrl, setMainUrl] = useState("");
    const [sideUrl, setSideUrl] = useState("");
    const [theme, setTheme] = useState("");
    const [themeIcon, setThemeIcon] = useState("");
    const [position, setPosition] = useState("");

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
                setMainUrl(arg.data.settings.mainUrl);
                setSideUrl(arg.data.settings.sideUrl);
                setTheme(arg.data.settings.theme);
                setPosition(arg.data.settings.position);
            }
        });
    }

    useEffect(() => {
        if (mainUrl && sideUrl)
            window.chrome.webview.postMessage({
                mainUrl: mainUrl,
                sideUrl: sideUrl,
            });
    }, [mainUrl, sideUrl]);

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

        if (data.mainUrl.toString() != "") {
            setMainUrl(data.mainUrl.toString());
        }

        if (data.sideUrl.toString() != "") {
            setSideUrl(data.sideUrl.toString());
        }

        form.reset();
    };

    return (
        <div>
            <form
                autoComplete="off"
                name="settings"
                spellCheck="false"
                id="settings"
                method="post"
                onSubmit={handleForm}>
                <div className="grid gap-6 self-start">
                    <div className="grid grid-flow-col justify-between">
                        <div className="select-none capitalize text-accentDark2">
                            {themeIcon} {theme} mode
                        </div>
                        <div className="select-none text-accentDark2">
                            <span>Dimensions: </span>
                            {position[2]} x {position[3]}
                            <br />
                            <span>Position: </span>
                            x: {position[0]} y: {position[1]}
                        </div>
                    </div>

                    <label>
                        <div className="grid grid-flow-col justify-between gap-6 truncate">
                            <div className="select-none text-accent">
                                🏠 Home
                            </div>
                            <div className="select-text text-sm text-accentDark1">
                                {mainUrl}
                            </div>
                        </div>
                        <input
                            type="text"
                            name="mainUrl"
                            id="mainUrl"
                            placeholder={mainUrl}
                            pattern=".*[.].*"></input>
                    </label>

                    <label>
                        <div className="grid grid-flow-col justify-between gap-6 truncate">
                            <span className="select-none text-accent">
                                🔧 Sidebar
                            </span>
                            <div className="select-text text-sm text-accentDark1">
                                {sideUrl}
                            </div>
                        </div>
                        <input
                            type="text"
                            name="sideUrl"
                            id="sideUrl"
                            placeholder={sideUrl}
                            pattern=".*[.].*"></input>
                    </label>
                </div>

                <button id="submitUrl" type="submit">
                    Save
                </button>
            </form>
        </div>
    );
}
