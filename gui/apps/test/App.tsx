import { SyntheticEvent, useState, useRef, useEffect } from "react";
// import * as url from "@libs/url";

interface Position {
    bar: number;
    border: number;
    horizontal: boolean;
    split: boolean;
    swapped: boolean;
}

export default function App() {
    const container = useRef<HTMLDivElement | null>(null);
    const [position, setPosition] = useState<Position>({
        bar: 0,
        border: 0,
        horizontal: false,
        split: false,
        swapped: false,
    });
    const [first, setFirst] = useState("");
    const [second, setSecond] = useState("");

    useEffect(() => {
        setPosition((prevState) => ({ ...prevState, bar: container.current!.offsetHeight }));
        window.chrome.webview.postMessage({ height: position.bar });
    }, [position.bar]);

    useEffect(() => {
        const onMessage = (event: Event) => {
            const data = (event as MessageEvent).data;
            console.log(data);

            if (data.layout) {
                setPosition(data.layout);
            }

            if (data.first) {
                setFirst(data.first);
            }

            if (data.second) {
                setSecond(data.second);
            }
        };

        window.chrome.webview.addEventListener("message", onMessage);

        return () => {
            window.chrome.webview.removeEventListener("message", onMessage);
        };
    });

    return (
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
                    value={first}
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
                    value={second}
                    // placeholder={sideUrlPlaceholder}
                    // title={sideUrlPlaceholder}
                    // ref={sideInput}
                    // onChange={handleChange}
                ></input>
            </form>
        </div>
    );
}
