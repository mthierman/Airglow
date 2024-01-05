import { SyntheticEvent, useState, useRef, useEffect } from "react";
import * as url from "@libs/url";

export default function App() {
    const containerRef = useRef<HTMLDivElement | null>(null);
    const [height, setHeight] = useState(0);
    useEffect(() => {
        setHeight(containerRef?.current?.offsetHeight!);
        if (height) window.chrome.webview.postMessage({ height: height });
    });

    const mainInput = useRef<HTMLInputElement | null>(null);
    const sideInput = useRef<HTMLInputElement | null>(null);
    const [mainUrl, setMainUrl] = useState("");
    const [sideUrl, setSideUrl] = useState("");
    const [mainUrlPlaceholder, setMainUrlPlaceholder] = useState("");
    const [sideUrlPlaceholder, setSideUrlPlaceholder] = useState("");

    if (window.chrome.webview) {
        window.chrome.webview.addEventListener("message", (event: Event) => {
            const data = (event as MessageEvent).data;
            if (data.mainUrl) {
                setMainUrl(data.mainUrl);
                setMainUrlPlaceholder(data.mainUrl);
                mainInput.current?.blur();
            }
            if (data.sideUrl) {
                setSideUrl(data.sideUrl);
                setSideUrlPlaceholder(data.sideUrl);
                sideInput.current?.blur();
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
        let form = event.target as HTMLFormElement;

        if (form.id === "mainForm") {
            if (mainInput.current?.value !== "") {
                let parsed = url.parseUrl(mainInput.current?.value!).href;
                if (window.chrome.webview) window.chrome.webview.postMessage({ mainUrl: parsed });
                // form.reset();
            }
        }

        if (form.id === "sideForm") {
            if (sideInput.current?.value !== "") {
                let parsed = url.parseUrl(sideInput.current?.value!).href;
                if (window.chrome.webview) window.chrome.webview.postMessage({ sideUrl: parsed });
                // form.reset();
            }
        }
    };

    document.addEventListener("keydown", (event: KeyboardEvent) => {
        const key = event.key;
        if (key === "Escape") {
            console.log(event.key);
            switch (key) {
                case "Escape":
                    if (document.activeElement === mainInput.current) {
                        mainInput.current!.value = mainUrlPlaceholder;
                        break;
                    }
                    if (document.activeElement === sideInput.current) {
                        sideInput.current!.value = sideUrlPlaceholder;
                        break;
                    }
            }
        }
    });

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
                    value={mainUrl}
                    // defaultValue={mainUrl}
                    placeholder={mainUrlPlaceholder}
                    title={mainUrl}
                    ref={mainInput}
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
                    value={sideUrl}
                    // defaultValue={sideUrl}
                    placeholder={sideUrlPlaceholder}
                    title={sideUrl}
                    ref={sideInput}
                    onChange={handleChange}></input>
            </form>
        </div>
    );
}
