import { SyntheticEvent, useState, useRef, useEffect } from "react";
// import * as url from "@libs/url";
// import { getSessionStorage, getSystemColorsStorage } from "@libs/storage";

export default function App() {
    const container = useRef<HTMLDivElement | null>(null);
    let initialized = false;
    useEffect(() => {
        if (!initialized) {
            window.chrome.webview.postMessage({ initialized: true });

            window.chrome.webview.postMessage({
                offsetHeight: Math.round(
                    container.current!.offsetHeight *
                        (Math.round(window.devicePixelRatio * 1e2) / 1e2),
                ),
            });

            initialized = true;
        }
    }, []);

    useEffect(() => {
        const onMessage = (event: Event) => {
            const data = (event as MessageEvent).data;
            console.log(data);
        };

        window.chrome.webview.addEventListener("message", onMessage);

        return () => {
            window.chrome.webview.removeEventListener("message", onMessage);
        };
    });

    useEffect(() => {
        const onResize = (event: Event) => {
            // const data = (event as MessageEvent).data;
            // console.log(data);

            window.chrome.webview.postMessage({
                offsetHeight: Math.round(
                    container.current!.offsetHeight *
                        (Math.round(window.devicePixelRatio * 1e2) / 1e2),
                ),
            });
        };

        addEventListener("resize", onResize);

        return () => {
            removeEventListener("resize", onResize);
        };
    });

    return (
        <div ref={container} id="container" className="p-4">
            <div>
                <button className="border-2 border-red-400 p-2">Test</button>
                <button className="border-2 border-red-400 p-2">Test</button>
                <button className="border-2 border-red-400 p-2">Test</button>
                <button className="border-2 border-red-400 p-2">Test</button>
            </div>
        </div>
    );
}
