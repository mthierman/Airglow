import { useState, useRef, useEffect, useLayoutEffect } from "react";

export default function App() {
    const container = useRef<HTMLDivElement | null>(null);
    const [height, setHeight] = useState(0);
    const [devicePixelRatio, setDevicePixelRatio] = useState(window.devicePixelRatio);

    useLayoutEffect(() => {
        const onResize = () => {
            setDevicePixelRatio(window.devicePixelRatio);
        };

        addEventListener("resize", onResize);

        return () => {
            removeEventListener("resize", onResize);
        };
    });

    useEffect(() => {
        setHeight(container.current?.offsetHeight!);
        window.chrome.webview.postMessage({ offsetHeight: height * devicePixelRatio });
    });

    useEffect(() => {
        window.chrome.webview.postMessage({ initialized: true });
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
