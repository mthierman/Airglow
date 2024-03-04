import "#css/index.css";
import {
    defaultFavicon,
    defaultLayout,
    defaultPage,
    defaultState,
    useColors,
    useInitializer,
    useScale,
} from "#libs/common";
import url from "#libs/url";
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
    const [focus, setFocus] = useState("first");

    useInitializer();

    useColors(state.colors);

    const scale = useScale();
    if (form.current) {
        window.chrome.webview.postMessage({
            height: Math.round(form.current.offsetHeight * scale),
        });
    }

    useEffect(() => {
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

        document.addEventListener("keydown", onEscape);

        return () => {
            document.removeEventListener("keydown", onEscape);
        };
    }, [firstBrowser.source, secondBrowser.source]);

    useEffect(() => {
        const inputs = {
            first: first.current,
            second: second.current,
        };

        const onFocus = () => {
            if (document.activeElement === inputs.first) {
                setFocus("first");
            } else if (document.activeElement === inputs.second) {
                setFocus("second");
            }
        };

        inputs.first?.addEventListener("focus", onFocus);
        inputs.second?.addEventListener("focus", onFocus);

        return () => {
            inputs.first?.removeEventListener("focus", onFocus);
            inputs.second?.removeEventListener("focus", onFocus);
        };
    }, []);

    useEffect(() => {
        const onWindowBlur = () => {
            first.current?.blur();
            second.current?.blur();
        };

        window.addEventListener("blur", onWindowBlur);

        return () => {
            window.removeEventListener("blur", onWindowBlur);
        };
    }, []);

    useEffect(() => {
        const onMessage = (event: Event) => {
            const data = (event as MessageEvent).data as App.Window;

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
                if (first.length !== 0) {
                    window.chrome.webview.postMessage({ first: url(first) });
                } else if (state.home[0].length !== 0) {
                    window.chrome.webview.postMessage({ first: url(state.home[0]) });
                }

                if (second.length !== 0) {
                    window.chrome.webview.postMessage({ second: url(second) });
                } else if (state.home[1].length !== 0) {
                    window.chrome.webview.postMessage({ second: url(state.home[1]) });
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

        window.chrome.webview.addEventListener("message", onMessage);

        return () => {
            window.chrome.webview.removeEventListener("message", onMessage);
        };
    });

    const handleSubmit = (event: SyntheticEvent) => {
        event.preventDefault();

        if (document.activeElement === first.current) {
            if (first.current) {
                const value = first.current.value;
                if (value !== "")
                    window.chrome.webview.postMessage({
                        first: url(value),
                    });
            }
        }

        if (document.activeElement === second.current) {
            if (second.current) {
                const value = second.current.value;
                if (value !== "")
                    window.chrome.webview.postMessage({
                        second: url(value),
                    });
            }
        }
    };

    const handleClick = async (event: SyntheticEvent) => {
        const nativeEvent = event.nativeEvent as MouseEvent;

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
                        onClick={(e) => {
                            void handleClick(e);
                        }}
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
                        onClick={(e) => {
                            void handleClick(e);
                        }}
                    />
                </label>
                <input type="submit" hidden />
            </form>
        </>
    );
}
