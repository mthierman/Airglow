import "@css/index.css";
import {
    applyColors,
    applyFavicon,
    defaultLayout,
    defaultPage,
    defaultState,
    initialize,
} from "@libs/index";
import { parseUrl } from "@libs/url";
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

    useEffect(() => {
        initialize();
        applyFavicon();
    }, []);

    useEffect(() => {
        applyColors(state.colors);
    }, [state.colors]);

    useEffect(() => {
        window.chrome.webview.postMessage({ height: offsetHeight });
    }, [offsetHeight]);

    useEffect(() => {
        if (form.current) {
            setOffsetHeight(form.current.offsetHeight);
        }

        const onMessage = (event: Event) => {
            const data: App.Window = (event as MessageEvent).data;
            console.log(data);

            if (Object.hasOwn(data, "m_state")) {
                sessionStorage.setItem("state", JSON.stringify(data.m_state));
                setState(data.m_state);
            }

            if (Object.hasOwn(data, "m_layout")) {
                sessionStorage.setItem("layout", JSON.stringify(data.m_layout));
                setLayout(data.m_layout);
            }

            if (Object.hasOwn(data, "m_first")) {
                sessionStorage.setItem("first", JSON.stringify(data.m_first));
                setFirstBrowser(data.m_first);
            }

            if (Object.hasOwn(data, "m_second")) {
                sessionStorage.setItem("second", JSON.stringify(data.m_second));
                setSecondBrowser(data.m_second);
            }

            if (Object.hasOwn(data, "navigate")) {
                const [first, second] = data.navigate as Pair;
                if (first.length !== 0) {
                    window.chrome.webview.postMessage({ first: parseUrl(first).href });
                }
                if (second.length !== 0) {
                    window.chrome.webview.postMessage({ second: parseUrl(second).href });
                }
            }

            // if (Object.hasOwn(data, "focus")) {
            //     if (focus === "first") {
            //         inputFirst.current!.focus();
            //         inputFirst.current!.select();
            //     } else if (focus === "second") {
            //         inputSecond.current!.focus();
            //         inputSecond.current!.select();
            //     }
            // }
        };

        // const onEscape = (event: KeyboardEvent) => {
        //     if (event.key === "Escape") {
        //         if (document.activeElement === inputFirst.current) {
        //             setUrl((prevState) => ({
        //                 ...prevState,
        //                 first: sessionStorage.getItem("first")!,
        //             }));
        //             inputFirst.current?.setSelectionRange(
        //                 inputFirst.current.value.length,
        //                 inputFirst.current.value.length,
        //             );
        //         } else if (document.activeElement === inputSecond.current) {
        //             setUrl((prevState) => ({
        //                 ...prevState,
        //                 second: sessionStorage.getItem("second")!,
        //             }));
        //             inputSecond.current?.setSelectionRange(
        //                 inputSecond.current.value.length,
        //                 inputSecond.current.value.length,
        //             );
        //         }
        //     }
        // };

        // const onFocus = () => {
        //     if (document.activeElement === inputFirst.current) {
        //         setFocus("first");
        //     } else if (document.activeElement === inputSecond.current) {
        //         setFocus("second");
        //     }
        // };

        // const onWindowBlur = () => {
        //     inputFirst.current?.blur();
        //     inputSecond.current?.blur();
        // };

        window.chrome.webview.addEventListener("message", onMessage);
        // document.addEventListener("keydown", onEscape);
        // inputFirst.current?.addEventListener("focus", onFocus);
        // inputSecond.current?.addEventListener("focus", onFocus);
        // window.addEventListener("blur", onWindowBlur);

        return () => {
            window.chrome.webview.removeEventListener("message", onMessage);
            // document.removeEventListener("keydown", onEscape);
            // inputFirst.current?.removeEventListener("focus", onFocus);
            // inputSecond.current?.removeEventListener("focus", onFocus);
            // window.removeEventListener("blur", onWindowBlur);
        };
    });

    const submitFirst = () => {
        const parsed = parseUrl(first.current?.value!).href;
        // setUrl((prevState) => ({ ...prevState, first: parsed }));
        // sessionStorage.setItem("first", parsed);
        window.chrome.webview.postMessage({ first: parsed });
    };

    const submitSecond = () => {
        const parsed = parseUrl(second.current?.value!).href;
        // setUrl((prevState) => ({ ...prevState, second: parsed }));
        // sessionStorage.setItem("second", parsed);
        window.chrome.webview.postMessage({ second: parsed });
    };

    const handleSubmit = (event: SyntheticEvent) => {
        event.preventDefault();

        if (document.activeElement === first.current && first.current!.value !== "") {
            submitFirst();
        }

        if (document.activeElement === second.current && second.current!.value !== "") {
            submitSecond();
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
                        !layout.split && layout.swap ? "hidden" : "url"
                    } ${layout.focus === "first" ? "url-focus" : "url-blur"}
                }`}>
                    <img
                        className="url-favicon"
                        width="16"
                        height="16"
                        src={firstBrowser.favicon}
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
                        !layout.split && !layout.swap ? "hidden" : "url"
                    } ${layout.focus === "second" ? "url-focus" : "url-blur"}
                }`}>
                    <img
                        className="url-favicon"
                        width="16"
                        height="16"
                        src={secondBrowser.favicon}
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
