import { useEffect, useState } from "react";
import icon from "./icon.svg?raw";

if (document.readyState === "interactive") {
    const favicon = document.createElement("link");
    favicon.type = "image/svg+xml";
    favicon.rel = "icon";
    favicon.href = `data:image/svg+xml,${encodeURIComponent(icon)}`;
    document.head.appendChild(favicon);
}

if (window.chrome.webview) {
    window.chrome.webview.addEventListener("message", (arg: any) => {
        console.log(arg.data);

        let settings = arg.data.settings;

        if (arg.data == "accentColor") console.log(arg.data.accentColor);
        document.documentElement.style.setProperty(
            "--accentColor",
            settings.accentColor,
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
            // window.chrome.webview.addEventListener("message", (arg: any) => {
            //     getData();
            // });
            try {
                const response = await fetch(`https://airglow/Config.json`);
                let data = await response.json();
                setMainUrl(data.mainUrl);
                setSideUrl(data.sideUrl);
                // setAccentColor(data.accentColor);
                // document.documentElement.style.setProperty(
                //     "--accentColor",
                //     data.accentColor,
                // );
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
