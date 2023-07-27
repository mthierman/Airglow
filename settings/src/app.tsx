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
                    "--accentColor",
                    arg.data.colors.accent,
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

        const rawData = new FormData(e.target);
        const data = Object.fromEntries(rawData.entries());

        if (data.mainUrl.toString() != "") {
            setMainUrl(data.mainUrl.toString());
        }

        if (data.sideUrl.toString() != "") {
            setSideUrl(data.sideUrl.toString());
        }
    };

    return (
        <div>
            <form
                autoComplete="on"
                id="settings"
                className="grid h-full content-between gap-6"
                method="post"
                onSubmit={handleForm}>
                <div className="grid gap-6 self-start">
                    <label>
                        <h1>
                            <span className="select-none">🏠 https://</span>
                            {mainUrl}
                        </h1>
                        <input
                            type="text"
                            name="mainUrl"
                            id="mainUrl"
                            placeholder={mainUrl}
                            pattern=".*[.].*"></input>
                    </label>

                    <label>
                        <h1>
                            <span className="select-none">🔧 https://</span>
                            {sideUrl}
                        </h1>
                        <input
                            type="text"
                            name="sideUrl"
                            id="sideUrl"
                            placeholder={sideUrl}
                            pattern=".*[.].*"></input>
                    </label>

                    <div>
                        <h1>{themeIcon} Position</h1>
                        {position[2]} x {position[3]}
                        <br />
                        x: {position[0]} y: {position[1]}
                    </div>
                </div>

                <button id="submitUrl" type="submit">
                    Save
                </button>
            </form>
        </div>
    );
}
