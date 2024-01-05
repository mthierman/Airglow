import { SyntheticEvent, useState, useRef, useEffect } from "react";
import * as url from "@libs/url";

export default function App() {
    const containerRef = useRef<HTMLDivElement | null>(null);
    const mainForm = useRef<HTMLFormElement | null>(null);
    const sideForm = useRef<HTMLFormElement | null>(null);
    const [height, setHeight] = useState(0);
    const mainInput = useRef<HTMLInputElement | null>(null);
    const sideInput = useRef<HTMLInputElement | null>(null);
    const [main, setMain] = useState("");
    const [side, setSide] = useState("");
    const [mainUrlPlaceholder, setMainUrlPlaceholder] = useState("");
    const [sideUrlPlaceholder, setSideUrlPlaceholder] = useState("");

    useEffect(() => {
        setHeight(containerRef?.current?.offsetHeight!);
        if (height) window.chrome.webview.postMessage({ height: height });
    });

    if (window.chrome.webview) {
        window.chrome.webview.addEventListener("message", (event: Event) => {
            const data = (event as MessageEvent).data;
            if (data.mainUrl) {
                setMain(data.mainUrl);
                setMainUrlPlaceholder(data.mainUrl);
                mainInput.current?.blur();
            }
            if (data.sideUrl) {
                setSide(data.sideUrl);
                setSideUrlPlaceholder(data.sideUrl);
                sideInput.current?.blur();
            }
        });
    }

    const handleChange = (event: SyntheticEvent) => {
        let input = event.target as HTMLInputElement;
        if (input.id === "mainUrl") {
            setMain(input.value);
        }
        if (input.id === "sideUrl") {
            setSide(input.value);
        }
    };

    const handleSubmit = (event: SyntheticEvent) => {
        event.preventDefault();
        let form = event.target as HTMLFormElement;

        if (form.id === "mainForm") {
            if (mainInput.current?.value !== "") {
                let parsed = url.parseUrl(mainInput.current?.value!).href;
                if (window.chrome.webview) window.chrome.webview.postMessage({ mainUrl: parsed });
            }
        }

        if (form.id === "sideForm") {
            if (sideInput.current?.value !== "") {
                let parsed = url.parseUrl(sideInput.current?.value!).href;
                if (window.chrome.webview) window.chrome.webview.postMessage({ sideUrl: parsed });
            }
        }
    };

    document.addEventListener("keydown", (event: KeyboardEvent) => {
        const key = event.key;
        if (event.ctrlKey && key === "r") event.preventDefault();
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
    });

    useEffect(() => {
        const input = mainInput.current;
        if (input) {
            input.addEventListener("click", async (event: MouseEvent) => {
                if (event.ctrlKey) await navigator.clipboard.writeText(mainUrlPlaceholder);
            });
        }
    });

    useEffect(() => {
        const input = sideInput.current;
        if (input) {
            input.addEventListener("click", async (event: MouseEvent) => {
                if (event.ctrlKey) await navigator.clipboard.writeText(sideUrlPlaceholder);
            });
        }
    });

    useEffect(() => {
        const form = mainForm.current;
        if (form) {
            form.addEventListener("focusout", () => {
                mainInput.current?.blur();
            });
        }
    });

    useEffect(() => {
        const form = sideForm.current;
        if (form) {
            form.addEventListener("focusout", () => {
                sideInput.current?.blur();
            });
        }
    });

    return (
        <div ref={containerRef} id="container" className="flex bg-transparent">
            <form
                className="flex flex-grow"
                id="mainForm"
                method="post"
                onSubmit={handleSubmit}
                ref={mainForm}
                autoComplete="off"
                spellCheck="false">
                <input
                    className="flex-grow text-ellipsis bg-transparent p-2 outline-none"
                    type="text"
                    id="mainUrl"
                    value={main}
                    placeholder={mainUrlPlaceholder}
                    title={mainUrlPlaceholder}
                    ref={mainInput}
                    onChange={handleChange}></input>
            </form>

            <form
                className="flex flex-grow"
                id="sideForm"
                method="post"
                onSubmit={handleSubmit}
                ref={sideForm}
                autoComplete="off"
                spellCheck="false">
                <input
                    className="flex-grow text-ellipsis bg-transparent p-2 outline-none"
                    type="text"
                    id="sideUrl"
                    value={side}
                    placeholder={sideUrlPlaceholder}
                    title={sideUrlPlaceholder}
                    ref={sideInput}
                    onChange={handleChange}></input>
            </form>
        </div>
    );
}
