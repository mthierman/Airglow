import { useState, useRef, useEffect } from "react";

export default function App() {
    const container = useRef<HTMLDivElement | null>(null);
    const [height, setHeight] = useState(null);

    useEffect(() => {
        console.log(container.current?.offsetHeight);
        window.chrome.webview.postMessage({ offsetHeight: container.current?.offsetHeight });
    });

    useEffect(() => {
        window.chrome.webview.postMessage({ initialized: true });
    }, []);

    // useEffect(() => {
    //     const onMessage = (event: Event) => {
    //         const data = (event as MessageEvent).data;
    //         console.log(data);
    //     };

    //     window.chrome.webview.addEventListener("message", onMessage);

    //     return () => {
    //         window.chrome.webview.removeEventListener("message", onMessage);
    //     };
    // });

    useEffect(() => {
        const onResize = () => {
            let height = container.current?.offsetHeight;
            let width = container.current?.offsetWidth;
            // let scale = Math.round(window.devicePixelRatio * 1e2) / 1e2;
            let scale = Math.ceil(window.devicePixelRatio);
            console.log(`${height} x ${scale}`);

            window.chrome.webview.postMessage({ offsetHeight: height! * scale });
            window.chrome.webview.postMessage({ offsetWidth: width! * scale });
            window.chrome.webview.postMessage({
                devicePixelRatio: scale,
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

// offsetHeight: Math.round(
//     container.current!.offsetHeight *
//         (Math.round(window.devicePixelRatio * 1e2) / 1e2),
