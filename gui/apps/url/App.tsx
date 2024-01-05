import { SyntheticEvent, useState, useRef, useEffect } from "react";
import * as url from "@libs/url";

export default function App() {
    const containerRef = useRef<HTMLDivElement | null>(null);
    const [height, setHeight] = useState(0);

    const mainRef = useRef<HTMLInputElement | null>(null);
    const sideRef = useRef<HTMLInputElement | null>(null);
    const [mainUrl, setMainUrl] = useState("");
    const [sideUrl, setSideUrl] = useState("");

    useEffect(() => {
        setHeight(containerRef?.current?.offsetHeight!);
        if (height) window.chrome.webview.postMessage({ height: height });
    });

    if (window.chrome.webview) {
        window.chrome.webview.addEventListener("message", (event: Event) => {
            const data = (event as MessageEvent).data;
            if (data.mainUrl) {
                setMainUrl(data.mainUrl);
                console.log(data.mainUrl);
            }
            if (data.sideUrl) {
                setSideUrl(data.sideUrl);
                console.log(data.sideUrl);
            }
        });
    }

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
            if (mainRef.current?.value !== "") {
                let parsed = url.parseUrl(mainRef.current?.value!).href;
                setMainUrl(parsed);
                if (window.chrome.webview) window.chrome.webview.postMessage({ mainUrl: parsed });
            }
        }

        if (input.id === "sideForm") {
            if (sideRef.current?.value !== "") {
                let parsed = url.parseUrl(sideRef.current?.value!).href;
                setSideUrl(parsed);
                if (window.chrome.webview) window.chrome.webview.postMessage({ sideUrl: parsed });
            }
        }
    };

    // const handleClipboard = async (url: string) => {
    //     await navigator.clipboard.writeText(url);
    // };

    return (
        <div ref={containerRef} id="container" className="flex bg-transparent">
            <form
                className="flex-grow"
                id="mainForm"
                method="post"
                onSubmit={handleSubmit}
                autoComplete="off"
                spellCheck="false">
                <input
                    className="w-full text-ellipsis bg-transparent p-2 outline-none"
                    type="text"
                    id="mainUrl"
                    title={mainUrl}
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
                    className="w-full text-ellipsis bg-transparent p-2 outline-none"
                    type="text"
                    id="sideUrl"
                    title={sideUrl}
                    value={sideUrl}
                    ref={sideRef}
                    onChange={handleChange}></input>
            </form>
        </div>
    );
}
