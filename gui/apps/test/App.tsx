import { useState, useRef, useEffect, useLayoutEffect } from "react";

export default function App() {
    const container = useRef<HTMLDivElement | null>(null);

    const [devicePixelRatio, setDevicePixelRatio] = useState(window.devicePixelRatio);
    const [offsetHeight, setOffsetHeight] = useState(0);
    const [offsetWidth, setOffsetWidth] = useState(0);

    // useEffect(() => {
    //     window.chrome.webview.postMessage({ initialized: true });
    // }, []);

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
        setOffsetHeight(container.current?.offsetHeight!);
        setOffsetWidth(container.current?.offsetWidth!);
        window.chrome.webview.postMessage({
            devicePixelRatio: devicePixelRatio,
            offsetHeight: offsetHeight * devicePixelRatio,
            offsetWidth: offsetWidth * devicePixelRatio,
        });
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
        <div ref={container} id="container" className="w-fit bg-red-400 p-4">
            <div>
                <button className="border-2 border-red-400 p-2">Test</button>
                <button className="border-2 border-red-400 p-2">Test</button>
                <button className="border-2 border-red-400 p-2">Test</button>
                <button className="border-2 border-red-400 p-2">Test</button>
            </div>
        </div>
    );
}
