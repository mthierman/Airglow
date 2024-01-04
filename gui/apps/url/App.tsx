import { SyntheticEvent } from "react";
import { useState } from "react";
import * as url from "@libs/url";

if (window.chrome.webview) {
    window.chrome.webview.addEventListener("message", (event: Event) => {
        const data = (event as MessageEvent).data;
        console.log(data);
    });
}

export default function App() {
    const [mainUrl, setMainUrl] = useState("");
    const [sideUrl, setSideUrl] = useState("");

    const handleChange = (event: SyntheticEvent) => {
        let input = event.target as HTMLInputElement;

        if (input.id === "mainUrl") {
            setMainUrl(input.value);
        }

        if (input.id === "sideUrl") {
            setSideUrl(input.value);
        }
    };

    const handleSubmit = (event: SyntheticEvent) => {
        event.preventDefault();
        let input = event.target as HTMLFormElement;

        if (input.id === "mainForm") {
            setMainUrl(url.parseUrl(mainUrl).href);
            if (window.chrome.webview) window.chrome.webview.postMessage({ mainUrl: mainUrl });
        }

        if (input.id === "sideForm") {
            setSideUrl(url.parseUrl(sideUrl).href);
            if (window.chrome.webview) window.chrome.webview.postMessage({ sideUrl: sideUrl });
        }
    };

    // const handleClipboard = async (url: string) => {
    //     await navigator.clipboard.writeText(url);
    // };

    return (
        <div className="flex gap-2 bg-transparent p-2">
            <form
                className="flex-grow"
                id="mainForm"
                method="post"
                onSubmit={handleSubmit}
                autoComplete="off"
                spellCheck="false">
                <input
                    className="w-full rounded-lg bg-flexoki-base-paper p-2 shadow-md shadow-flexoki-base-50 outline-none dark:bg-flexoki-base-950 dark:shadow-flexoki-base-black"
                    type="text"
                    id="mainUrl"
                    value={mainUrl}
                    onChange={handleChange}></input>
            </form>

            <form
                className="flex-grow"
                id="sideForm"
                method="post"
                onSubmit={handleSubmit}
                autoComplete="off"
                spellCheck="false">
                <input
                    className="w-full rounded-lg bg-flexoki-base-paper p-2 shadow-md shadow-flexoki-base-50 outline-none dark:bg-flexoki-base-950 dark:shadow-flexoki-base-black"
                    type="text"
                    id="sideUrl"
                    value={sideUrl}
                    onChange={handleChange}></input>
            </form>
        </div>
    );
}
