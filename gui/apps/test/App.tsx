import { useState, useRef, useEffect, useLayoutEffect } from "react";

export default function App() {
    const [devicePixelRatio, setDevicePixelRatio] = useState(window.devicePixelRatio);
    const [offsetHeight, setOffsetHeight] = useState(0);
    const [offsetWidth, setOffsetWidth] = useState(0);

    useEffect(() => {
        window.chrome.webview.postMessage({ initialized: true });
    }, []);

    useLayoutEffect(() => {
        const onResize = () => {
            setDevicePixelRatio(window.devicePixelRatio);
            console.log(document.body.offsetHeight);
            console.log(document.body.offsetWidth);
        };

        addEventListener("resize", onResize);

        return () => {
            removeEventListener("resize", onResize);
        };
    });

    useEffect(() => {
        setOffsetHeight(document.body.offsetHeight);
        setOffsetWidth(document.body.offsetWidth);
        window.chrome.webview.postMessage({
            devicePixelRatio: devicePixelRatio,
            offsetHeight: offsetHeight * devicePixelRatio,
            offsetWidth: offsetWidth * devicePixelRatio,
        });
        // window.chrome.webview.postMessage({
        //     dimensions: {
        //         devicePixelRatio: devicePixelRatio,
        //         offsetHeight: offsetHeight * devicePixelRatio,
        //         offsetWidth: offsetWidth * devicePixelRatio,
        //     },
        // });
    });

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

    return (
        <div id="container" className="grid bg-red-400">
            <button className="border-2 border-green-400 p-2">Test</button>
            <button className="border-2 border-green-400 p-2">Test</button>
            <button className="border-2 border-green-400 p-2">Test</button>
            <button className="border-2 border-green-400 p-2">Test</button>
        </div>
    );
}
