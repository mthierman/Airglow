import "#css/index.css";
import {
    applyColors,
    defaultFavicon,
    defaultLayout,
    defaultPage,
    defaultState,
    initialize,
} from "#libs/index";
import { parseUrl } from "#libs/url";
import { SyntheticEvent, useEffect, useRef, useState } from "react";

export default function App() {
    const form = useRef<HTMLFormElement | null>(null);
    const first = useRef<HTMLInputElement | null>(null);
    const second = useRef<HTMLInputElement | null>(null);
    const [firstValue, setFirstValue] = useState("");
    const [secondValue, setSecondValue] = useState("");

    const [state, setState] = useState(defaultState());
    const [layout, setLayout] = useState(defaultLayout());
    const [firstBrowser, setFirstBrowser] = useState(defaultPage());
    const [secondBrowser, setSecondBrowser] = useState(defaultPage());

    const [offsetHeight, setOffsetHeight] = useState(0);
    const [focus, setFocus] = useState("first");

    useEffect(() => {
        initialize();
    }, []);

    useEffect(() => {
        applyColors(state.colors);
    }, [state.colors]);

    useEffect(() => {
        window.chrome.webview.postMessage({ height: offsetHeight });
    }, [offsetHeight]);

    useEffect(() => {
        setOffsetHeight(form.current!.offsetHeight);

        const onMessage = (event: Event) => {
            const data: App.Window = (event as MessageEvent).data;

            if (Object.hasOwn(data, "m_state")) {
                setState(data.m_state);
            }

            if (Object.hasOwn(data, "m_layout")) {
                setLayout(data.m_layout);
                setFocus(data.m_layout.focus);
            }

            if (Object.hasOwn(data, "m_first")) {
                setFirstBrowser(data.m_first);
                setFirstValue(data.m_first.source);
            }

            if (Object.hasOwn(data, "m_second")) {
                setSecondBrowser(data.m_second);
                setSecondValue(data.m_second.source);
            }

            if (Object.hasOwn(data, "navigate")) {
                const [first, second] = data.navigate as Pair;
                console.log(first);
                console.log(second);
                if (first.length !== 0) {
                    window.chrome.webview.postMessage({ first: parseUrl(first).href });
                } else if (state.home[0].length !== 0) {
                    window.chrome.webview.postMessage({ first: parseUrl(state.home[0]).href });
                }

                if (second.length !== 0) {
                    window.chrome.webview.postMessage({ second: parseUrl(second).href });
                } else if (state.home[1].length !== 0) {
                    window.chrome.webview.postMessage({ second: parseUrl(state.home[1]).href });
                }
            }

            if (Object.hasOwn(data, "focus")) {
                if (focus === "first") {
                    first.current!.focus();
                    first.current!.select();
                }
                if (focus === "second") {
                    second.current!.focus();
                    second.current!.select();
                }
            }
        };

        const onEscape = (event: KeyboardEvent) => {
            if (event.key === "Escape") {
                if (document.activeElement === first.current) {
                    setFirstValue(firstBrowser.source);
                    first.current?.setSelectionRange(
                        first.current.value.length,
                        first.current.value.length,
                    );
                } else if (document.activeElement === second.current) {
                    setSecondValue(secondBrowser.source);
                    second.current?.setSelectionRange(
                        second.current.value.length,
                        second.current.value.length,
                    );
                }
            }
        };

        const onFocus = () => {
            if (document.activeElement === first.current) {
                setFocus("first");
            } else if (document.activeElement === second.current) {
                setFocus("second");
            }
        };

        const onWindowBlur = () => {
            first.current?.blur();
            second.current?.blur();
        };

        window.chrome.webview.addEventListener("message", onMessage);
        document.addEventListener("keydown", onEscape);
        first.current?.addEventListener("focus", onFocus);
        second.current?.addEventListener("focus", onFocus);
        window.addEventListener("blur", onWindowBlur);

        return () => {
            window.chrome.webview.removeEventListener("message", onMessage);
            document.removeEventListener("keydown", onEscape);
            first.current?.removeEventListener("focus", onFocus);
            second.current?.removeEventListener("focus", onFocus);
            window.removeEventListener("blur", onWindowBlur);
        };
    });

    const handleSubmit = (event: SyntheticEvent) => {
        event.preventDefault();

        if (document.activeElement === first.current) {
            if (first.current) {
                const value = first.current.value;
                if (value !== "")
                    window.chrome.webview.postMessage({
                        first: parseUrl(value).href,
                    });
            }
        }

        if (document.activeElement === second.current) {
            if (second.current) {
                const value = second.current.value;
                if (value !== "")
                    window.chrome.webview.postMessage({
                        second: parseUrl(value).href,
                    });
            }
        }
    };

    const handleClick = async (event: SyntheticEvent) => {
        let nativeEvent = event.nativeEvent as MouseEvent;

        if (document.activeElement === first.current) {
            if (nativeEvent.ctrlKey) await navigator.clipboard.writeText(firstBrowser.source);
        } else if (document.activeElement === second.current) {
            if (nativeEvent.ctrlKey) await navigator.clipboard.writeText(secondBrowser.source);
        }
    };

    return (
        <>
            <form
                className="url-form"
                ref={form}
                id="form"
                method="post"
                autoComplete="off"
                spellCheck="false"
                onSubmit={handleSubmit}>
                <label
                    className={`${layout.swap ? "order-1" : "order-0"} ${
                        !layout.split && layout.swap ? "hidden" : "url-label"
                    } ${focus === "first" ? "url-focus" : "url-blur"}
                }`}>
                    <img
                        className="url-favicon"
                        width="16"
                        height="16"
                        src={
                            firstBrowser.favicon.length !== 0
                                ? firstBrowser.favicon
                                : defaultFavicon()
                        }
                    />
                    <input
                        className="url-input"
                        ref={first}
                        id="first"
                        type="text"
                        placeholder={firstBrowser.source}
                        title={firstBrowser.source}
                        value={firstValue}
                        onChange={(e) => setFirstValue(e.target.value)}
                        onClick={handleClick}
                    />
                </label>
                <label
                    className={`${layout.swap ? "order-0" : "order-1"} ${
                        !layout.split && !layout.swap ? "hidden" : "url-label"
                    } ${focus === "second" ? "url-focus" : "url-blur"}
                }`}>
                    <img
                        className="url-favicon"
                        width="16"
                        height="16"
                        src={
                            secondBrowser.favicon.length !== 0
                                ? secondBrowser.favicon
                                : defaultFavicon()
                        }
                    />
                    <input
                        className="url-input"
                        ref={second}
                        id="second"
                        type="text"
                        placeholder={secondBrowser.source}
                        title={secondBrowser.source}
                        value={secondValue}
                        onChange={(e) => setSecondValue(e.target.value)}
                        onClick={handleClick}></input>
                </label>
                <input type="submit" hidden />
            </form>
        </>
    );
}
