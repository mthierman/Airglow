import { useEffect, useState } from "react";
import icon from "../../data/icons/icon.svg?raw";

document.onreadystatechange = () => {
    if (document.readyState === "complete") {
        const favicon = document.createElement("link");
        favicon.type = "image/svg+xml";
        favicon.rel = "icon";
        favicon.href = `data:image/svg+xml,${encodeURIComponent(icon)}`;
        document.head.appendChild(favicon);

        onkeydown = (e) => {
            if (e.key === "F1") {
                e.preventDefault();
                window.chrome.webview.postMessage("split");
            }
            if (e.key === "F2") {
                e.preventDefault();
                window.chrome.webview.postMessage("swapped");
            }
            if (e.key === "F3") {
                e.preventDefault();
            }
            if (e.key === "F4") {
                e.preventDefault();
                window.chrome.webview.postMessage("menu");
            }
            if (e.key === "F6") {
                e.preventDefault();
                window.chrome.webview.postMessage("maximize");
            }
            if (e.key === "F9") {
                e.preventDefault();
                window.chrome.webview.postMessage("topmost");
            }
            if (e.key === "F11") {
                e.preventDefault();
                window.chrome.webview.postMessage("fullscreen");
            }
            if (e.ctrlKey && e.key === "w") {
                e.preventDefault();
                window.chrome.webview.postMessage("close");
            } else {
                // window.chrome.webview.postMessage(e.key);
            }
        };
    }
};

if (window.chrome.webview) {
    window.chrome.webview.addEventListener("message", (arg: any) => {
        let colors = arg.data.colors;
        document.documentElement.style.setProperty(
            "--accentColor",
            colors.accent,
        );
    });
}

export default function App() {
    const [mainUrl, setMainUrl] = useState("");
    const [sideUrl, setSideUrl] = useState("");

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
    };

    useEffect(() => {
        const getData = async () => {
            try {
                const response = await fetch(`https://airglow/Airglow.json`);
                let data = await response.json();
                let settings = data.settings;
                setMainUrl(settings.mainUrl);
                setSideUrl(settings.sideUrl);
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
                </div>

                <button id="submitUrl" type="submit">
                    Save
                </button>
            </form>
        </div>
    );
}
