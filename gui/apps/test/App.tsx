import { SyntheticEvent, useState, useRef, useEffect } from "react";
// import * as url from "@libs/url";

interface Position {
    swapped: boolean;
    split: boolean;
    horizontal: boolean;
}

export default function App() {
    const container = useRef<HTMLDivElement | null>(null);
    const [height, setHeight] = useState(0);
    const [main, setMain] = useState("first");
    const [side, setSide] = useState("second");

    useEffect(() => {
        setHeight(container.current!.offsetHeight);
        window.chrome.webview.postMessage({ height: height });
    }, [height]);

    const [position, setPosition] = useState<Position>({
        split: false,
        swapped: false,
        horizontal: false,
    });

    useEffect(() => {
        const onMessage = (event: Event) => {
            const data = (event as MessageEvent).data;
            console.log(data);
            setPosition(data);
            // console.log(position);
        };

        window.chrome.webview.addEventListener("message", onMessage);

        return () => {
            window.chrome.webview.removeEventListener("message", onMessage);
        };
    }, [position]);

    return (
        // <div ref={container} id="container" className={`flex bg-transparent`}>
        <div
            ref={container}
            id="container"
            className={`flex bg-transparent ${position.swapped ? "flex-row-reverse" : "flex-row"}`}>
            <form
                className="flex flex-grow"
                id="mainForm"
                method="post"
                // onSubmit={handleSubmit}
                // ref={mainForm}
                autoComplete="off"
                spellCheck="false">
                <input
                    className="flex-grow text-ellipsis bg-transparent p-2 text-center outline-none"
                    type="text"
                    id="mainUrl"
                    value={main}
                    // placeholder={mainUrlPlaceholder}
                    // title={mainUrlPlaceholder}
                    // ref={mainInput}
                    // onChange={handleChange}
                ></input>
            </form>

            <form
                className="flex flex-grow"
                id="sideForm"
                method="post"
                // onSubmit={handleSubmit}
                // ref={sideForm}
                autoComplete="off"
                spellCheck="false">
                <input
                    className="flex-grow text-ellipsis bg-transparent p-2 text-center outline-none"
                    type="text"
                    id="sideUrl"
                    value={side}
                    // placeholder={sideUrlPlaceholder}
                    // title={sideUrlPlaceholder}
                    // ref={sideInput}
                    // onChange={handleChange}
                ></input>
            </form>
        </div>
    );
}
