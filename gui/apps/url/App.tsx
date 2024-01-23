import { SyntheticEvent, useState, useRef, useEffect } from "react";
import * as url from "@libs/url";
import { getLayoutStorage, getSystemColorsStorage } from "@libs/storage";

export default function App() {
    const [position, setPosition] = useState<App.Layout>(getLayoutStorage());
    const [systemColors, setSystemColors] = useState<App.SystemColors>(getSystemColorsStorage());

    const form = useRef<HTMLFormElement | null>(null);
    const first = useRef<HTMLInputElement | null>(null);
    const second = useRef<HTMLInputElement | null>(null);

    const [firstCurrent, setFirstCurrent] = useState("");
    const [secondCurrent, setSecondCurrent] = useState("");

    const [firstFavicon, setFirstFavicon] = useState("");
    const [secondFavicon, setSecondFavicon] = useState("");

    useEffect(() => {
        window.chrome.webview.postMessage({ initialized: true });
    }, []);

    useEffect(() => {
        document.documentElement.style.setProperty("--accent", systemColors.accent);
        document.documentElement.style.setProperty("--accentDark1", systemColors.accentDark1);
        document.documentElement.style.setProperty("--accentDark2", systemColors.accentDark2);
        document.documentElement.style.setProperty("--accentDark3", systemColors.accentDark3);
        document.documentElement.style.setProperty("--accentLight1", systemColors.accentLight1);
        document.documentElement.style.setProperty("--accentLight2", systemColors.accentLight2);
        document.documentElement.style.setProperty("--accentLight3", systemColors.accentLight3);
    }, [systemColors]);

    useEffect(() => {
        setPosition((prevState) => ({ ...prevState, bar: form.current!.offsetHeight }));
        window.chrome.webview.postMessage({ height: position.bar });
        sessionStorage.setItem("position", JSON.stringify(position));
    }, [position.bar]);

    useEffect(() => {
        const onMessage = (event: Event) => {
            const data = (event as MessageEvent).data;
            // console.log(data);

            if (data.layout) {
                setPosition(data.layout);
                sessionStorage.setItem("position", JSON.stringify(data.layout));
                setPosition((prevState) => ({
                    ...prevState,
                    bar: form.current!.offsetHeight,
                }));
            }

            if (data.systemColors) {
                setSystemColors(data.systemColors);
                sessionStorage.setItem("systemColors", JSON.stringify(data.systemColors));
            }

            if (data.first) {
                setFirstCurrent(data.first);
                sessionStorage.setItem("first", data.first);
                setFirstFavicon(url.getFavicon(data.first));
            }

            if (data.second) {
                setSecondCurrent(data.second);
                sessionStorage.setItem("second", data.second);
                setSecondFavicon(url.getFavicon(data.second));
            }
        };

        const onEscape = (event: KeyboardEvent) => {
            if (event.key === "Escape") {
                if (document.activeElement === first.current) {
                    setFirstCurrent(sessionStorage.getItem("first")!);
                } else if (document.activeElement === second.current) {
                    setSecondCurrent(sessionStorage.getItem("second")!);
                }
            }
        };

        window.chrome.webview.addEventListener("message", onMessage);
        document.addEventListener("keydown", onEscape);

        return () => {
            window.chrome.webview.removeEventListener("message", onMessage);
            document.removeEventListener("keydown", onEscape);
        };
    });

    const handleSubmit = (event: SyntheticEvent) => {
        event.preventDefault();

        if (document.activeElement === first.current) {
            const parsed = url.parseUrl(first.current?.value!).href;
            setFirstCurrent(parsed);
            sessionStorage.setItem("first", parsed);
            window.chrome.webview.postMessage({ first: parsed });
        }

        if (document.activeElement === second.current) {
            const parsed = url.parseUrl(second.current?.value!).href;
            setSecondCurrent(parsed);
            sessionStorage.setItem("second", parsed);
            window.chrome.webview.postMessage({ second: parsed });
        }
    };

    const handleClick = async (event: SyntheticEvent) => {
        let nativeEvent = event.nativeEvent as MouseEvent;

        if (document.activeElement === first.current) {
            if (nativeEvent.ctrlKey) await navigator.clipboard.writeText(firstCurrent);
        } else if (document.activeElement === second.current) {
            if (nativeEvent.ctrlKey) await navigator.clipboard.writeText(secondCurrent);
        }
    };

    return (
        <>
            <form
                ref={form}
                id="form"
                className="grid grid-flow-col"
                method="post"
                autoComplete="off"
                spellCheck="false"
                onSubmit={handleSubmit}>
                <label
                    className={`url ${position.swapped ? "order-1" : "order-0"} ${
                        !position.split && position.swapped ? "hidden" : ""
                    }`}>
                    <img width="16" height="16" src={firstFavicon} className="favicon" />
                    <input
                        ref={first}
                        id="first"
                        className="input "
                        type="text"
                        value={firstCurrent}
                        placeholder={sessionStorage.getItem("first")!}
                        title={sessionStorage.getItem("first")!}
                        onChange={(e) => setFirstCurrent(e.target.value)}
                        onClick={handleClick}
                    />
                </label>
                <label
                    className={`url ${position.swapped ? "order-0" : "order-1"} ${
                        !position.split && !position.swapped ? "hidden" : ""
                    }`}>
                    <img width={16} height={16} src={secondFavicon} className="favicon" />
                    <input
                        ref={second}
                        id="second"
                        className="input"
                        type="text"
                        value={secondCurrent}
                        placeholder={sessionStorage.getItem("second")!}
                        title={sessionStorage.getItem("second")!}
                        onChange={(e) => setSecondCurrent(e.target.value)}
                        onClick={handleClick}></input>
                </label>
                <input type="submit" hidden />
            </form>
        </>
    );
}
