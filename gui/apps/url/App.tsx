// get [form.current?.offsetHeight] working so it updates when style changes (and maybe zoom?)

import { SyntheticEvent, useState, useRef, useEffect } from "react";
import * as url from "@libs/url";
import {
    getLayoutStorage,
    getSessionStorage,
    getColorStorage,
    applyColors,
    initialize,
} from "@libs/storage";
import iconRaw from "../../../data/release.svg?raw";

export default function App() {
    const [layout, setLayout] = useState<App.Layout>(getLayoutStorage());
    const [colors, setColors] = useState<App.Colors>(getColorStorage());
    const form = useRef<HTMLFormElement | null>(null);
    const inputFirst = useRef<HTMLInputElement | null>(null);
    const inputSecond = useRef<HTMLInputElement | null>(null);
    const [selectedCurrent, setSelectedCurrent] = useState("");
    const [firstCurrent, setFirstCurrent] = useState("");
    const [secondCurrent, setSecondCurrent] = useState("");
    const [firstFavicon, setFirstFavicon] = useState(getSessionStorage("firstFavicon", ""));
    const [secondFavicon, setSecondFavicon] = useState(getSessionStorage("secondFavicon", ""));

    useEffect(() => {
        window.chrome.webview.postMessage({ height: form.current?.offsetHeight });
        initialize();
    }, []);

    useEffect(() => {
        applyColors(colors);
    }, [colors]);

    // useEffect(() => {
    //     setLayout((prevState) => ({ ...prevState, bar: form.current!.offsetHeight }));
    //     window.chrome.webview.postMessage({ height: layout.bar });
    //     sessionStorage.setItem("layout", JSON.stringify(layout));
    // }, [layout.bar]);

    // useEffect(() => {
    //     setLayout((prevState) => ({ ...prevState, bar: form.current!.offsetHeight }));
    //     window.chrome.webview.postMessage({ height: layout.bar });
    //     sessionStorage.setItem("layout", JSON.stringify(layout));
    // }, [form.current?.offsetHeight]);

    useEffect(() => {
        const onMessage = (event: Event) => {
            const data: App.Window = (event as MessageEvent).data;

            if (Object.hasOwn(data, "m_colors")) {
                const colors = data.m_colors.colors;
                setColors(colors);
                sessionStorage.setItem("colors", JSON.stringify(colors));
            }

            if (Object.hasOwn(data, "m_layout")) {
                const layout = data.m_layout;
                setLayout(layout);
                sessionStorage.setItem("layout", JSON.stringify(layout));
            }

            if (Object.hasOwn(data, "m_url")) {
                const [first, second] = data.m_url.current;
                if (first === "about:blank") {
                    setFirstFavicon(`data:image/svg+xml,${encodeURIComponent(iconRaw)}`);
                }
                if (second === "about:blank") {
                    setSecondFavicon(`data:image/svg+xml,${encodeURIComponent(iconRaw)}`);
                }
                setFirstCurrent(first);
                sessionStorage.setItem("first", first);
                setSecondCurrent(second);
                sessionStorage.setItem("second", second);
                // window.chrome.webview.postMessage({ first: firstCurrent });
                // window.chrome.webview.postMessage({ second: secondCurrent });
            }

            if (Object.hasOwn(data, "m_faviconUrl")) {
                const [first, second] = data.m_faviconUrl;
                setFirstFavicon(first);
                sessionStorage.setItem("firstFavicon", first);
                setSecondFavicon(second);
                sessionStorage.setItem("secondFavicon", second);
            }

            if (Object.hasOwn(data, "m_focus")) {
                const focus = data.m_focus;
                if (focus === "first") {
                    setSelectedCurrent("first");
                    inputFirst.current!.focus();
                    inputFirst.current!.select();
                } else if (focus === "second") {
                    setSelectedCurrent("second");
                    inputSecond.current!.focus();
                    inputSecond.current!.select();
                } else if (focus === "url") {
                    if (selectedCurrent === "first") {
                        inputFirst.current!.focus();
                        inputFirst.current!.select();
                    } else {
                        inputSecond.current!.focus();
                        inputSecond.current!.select();
                    }
                }
            }
        };

        const onEscape = (event: KeyboardEvent) => {
            if (event.key === "Escape") {
                if (document.activeElement === inputFirst.current) {
                    setFirstCurrent(sessionStorage.getItem("first")!);
                } else if (document.activeElement === inputSecond.current) {
                    setSecondCurrent(sessionStorage.getItem("second")!);
                }
            }
        };

        const onFocus = () => {
            if (document.activeElement === inputFirst.current) {
                setSelectedCurrent("first");
            } else if (document.activeElement === inputSecond.current) {
                setSelectedCurrent("second");
            }
        };

        window.chrome.webview.addEventListener("message", onMessage);
        document.addEventListener("keydown", onEscape);
        inputFirst.current!.addEventListener("focus", onFocus);
        inputSecond.current!.addEventListener("focus", onFocus);

        return () => {
            window.chrome.webview.removeEventListener("message", onMessage);
            document.removeEventListener("keydown", onEscape);
            inputFirst.current!.removeEventListener("focus", onFocus);
            inputSecond.current!.removeEventListener("focus", onFocus);
        };
    });

    const submitFirst = () => {
        const parsed = url.parseUrl(inputFirst.current?.value!).href;
        setFirstCurrent(parsed);
        sessionStorage.setItem("first", parsed);
        window.chrome.webview.postMessage({ first: parsed });
    };

    const submitSecond = () => {
        const parsed = url.parseUrl(inputSecond.current?.value!).href;
        setSecondCurrent(parsed);
        sessionStorage.setItem("second", parsed);
        window.chrome.webview.postMessage({ second: parsed });
    };

    const handleSubmit = (event: SyntheticEvent) => {
        event.preventDefault();

        if (document.activeElement === inputFirst.current && inputFirst.current!.value !== "") {
            submitFirst();
        }

        if (document.activeElement === inputSecond.current && inputSecond.current!.value !== "") {
            submitSecond();
        }
    };

    const handleClick = async (event: SyntheticEvent) => {
        let nativeEvent = event.nativeEvent as MouseEvent;

        if (document.activeElement === inputFirst.current) {
            if (nativeEvent.ctrlKey) await navigator.clipboard.writeText(firstCurrent);
        } else if (document.activeElement === inputSecond.current) {
            if (nativeEvent.ctrlKey) await navigator.clipboard.writeText(secondCurrent);
        }
    };

    return (
        <>
            <form
                className="grid grid-flow-col"
                ref={form}
                id="form"
                method="post"
                autoComplete="off"
                spellCheck="false"
                onSubmit={handleSubmit}>
                <label
                    className={`${layout.swapped ? "order-1" : "order-0"} ${
                        !layout.split && layout.swapped ? "hidden" : "url"
                    }`}>
                    <img className="url-favicon" width="16" height="16" src={firstFavicon} />
                    <input
                        className="url-input"
                        ref={inputFirst}
                        id="first"
                        type="text"
                        value={firstCurrent}
                        placeholder={sessionStorage.getItem("first")!}
                        title={sessionStorage.getItem("first")!}
                        onChange={(e) => setFirstCurrent(e.target.value)}
                        onClick={handleClick}
                    />
                </label>
                <label
                    className={`${layout.swapped ? "order-0" : "order-1"} ${
                        !layout.split && !layout.swapped ? "hidden" : "url"
                    }`}>
                    <img className="url-favicon" width="16" height="16" src={secondFavicon} />
                    <input
                        className="url-input"
                        ref={inputSecond}
                        id="second"
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
