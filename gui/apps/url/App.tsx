import { SyntheticEvent, useState, useRef } from "react";
import * as url from "@libs/url";

export default function App() {
    const mainRef = useRef<HTMLInputElement | null>(null);
    const sideRef = useRef<HTMLInputElement | null>(null);
    const [mainUrl, setMainUrl] = useState("");
    const [sideUrl, setSideUrl] = useState("");

    if (window.chrome.webview) {
        window.chrome.webview.addEventListener("message", (event: Event) => {
            const message = event as MessageEvent;
            const data = message.data;
            if (data.mainUrl) {
                setMainUrl(data.mainUrl);
                console.log(data.mainUrl);
            }
            if (data.sideUrl) {
                setSideUrl(data.sideUrl);
                console.log(data.sideUrl);
            }
            // const data = (event as MessageEvent).data;
            // console.log(data);
        });
    }

    const handleChange = (event: SyntheticEvent) => {
        let input = event.target as HTMLInputElement;

        // console.log(mainRef.current?.value);
        // console.log(sideRef.current?.value);

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
            if (mainRef.current?.value !== "") {
                let parsed = url.parseUrl(mainRef.current?.value!).href;
                setMainUrl(parsed);
                // console.log(parsed);
                if (window.chrome.webview) window.chrome.webview.postMessage({ mainUrl: parsed });
            }
        }

        if (input.id === "sideForm") {
            if (sideRef.current?.value !== "") {
                let parsed = url.parseUrl(sideRef.current?.value!).href;
                setSideUrl(parsed);
                // console.log(parsed);
                if (window.chrome.webview) window.chrome.webview.postMessage({ sideUrl: parsed });
            }
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
                    ref={mainRef}
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
                    ref={sideRef}
                    onChange={handleChange}></input>
            </form>
        </div>
    );
}
