import { SyntheticEvent, useState, useRef, useEffect } from "react";
// import * as url from "@libs/url";
// import { getSessionStorage, getSystemColorsStorage } from "@libs/storage";

export default function App() {
    const container = useRef<HTMLDivElement | null>(null);

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

    return <div ref={container} id="container" className=""></div>;
}
