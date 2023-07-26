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

if (window.chrome.webview) {
    window.chrome.webview.addEventListener("message", (arg: any) => {
        if (arg.data.colors) {
            let colors = arg.data.colors;
            console.log(colors);
            document.documentElement.style.setProperty(
                "--accentColor",
                colors.accent,
            );
        }
        if (arg.data.settings) {
            let settings = arg.data.settings;
            console.log(settings);
        }
    });
}

export default function App() {
    const [mainUrl, setMainUrl] = useState("");
    const [sideUrl, setSideUrl] = useState("");
    const [theme, setTheme] = useState("");

    const handleForm = (e: any) => {
        e.preventDefault();

        const rawData = new FormData(e.target);
        const data = Object.fromEntries(rawData.entries());

        if (data.mainUrl.toString() != "") {
            setMainUrl(data.mainUrl.toString());
            window.chrome.webview.postMessage(
                `mainUrl ${data.mainUrl.toString()}`,
            );
        }
        if (data.sideUrl.toString() != "") {
            setSideUrl(data.sideUrl.toString());
            window.chrome.webview.postMessage(
                `sideUrl ${data.sideUrl.toString()}`,
            );
        }
        if (data.theme.toString() != "") {
            setTheme(data.theme.toString());
            window.chrome.webview.postMessage(`theme ${data.theme.toString()}`);
        }
    };

    useEffect(() => {
        const getData = async () => {
            try {
                const response = await fetch(`https://airglow/Airglow.json`);
                let data = await response.json();
                let settings = data.settings;
                setMainUrl(settings.mainUrl);
                setSideUrl(settings.sideUrl);
                setTheme(settings.theme);
            } catch (error) {
                console.error("error", error);
            }
        };
        getData();
    }, []);

    return (
        <div>
            <form
                autoComplete="on"
                id="settings"
                method="post"
                onSubmit={handleForm}>
                <div id="url">
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
                    <div>Theme: {theme}</div>
                </div>

                <button id="submitUrl" type="submit">
                    Save
                </button>
            </form>
        </div>
    );
}
