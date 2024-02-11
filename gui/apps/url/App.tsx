import "@css/index.css";
import { applyColors, applyFavicon, getLayout, getState, initialize } from "@libs/index";
import { parseUrl } from "@libs/url";
import { useEffect, useRef, useState } from "react";

export default function App() {
    const form = useRef<HTMLFormElement | null>(null);
    const first = useRef<HTMLInputElement | null>(null);
    const second = useRef<HTMLInputElement | null>(null);
    const [state, setState] = useState<App.State>(getState());
    const [layout, setLayout] = useState<App.Layout>(getLayout());
    const [offsetHeight, setOffsetHeight] = useState(0);

    useEffect(() => {
        initialize();
        applyFavicon();
    }, []);

    useEffect(() => {
        console.log(state);
        // sessionStorage.setItem("state", JSON.stringify(state));
    }, [state]);

    useEffect(() => {
        console.log(layout);
        // sessionStorage.setItem("layout", JSON.stringify(layout));
    }, [layout]);

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

        if (!layout.init) {
            setLayout((prevState) => ({ ...prevState, init: true }));
            // sessionStorage.setItem("layout", JSON.stringify(layout));
            window.chrome.webview.postMessage({ first: parseUrl(state.args[0]).href });
            window.chrome.webview.postMessage({ second: parseUrl(state.args[1]).href });
        }

        const onMessage = (event: Event) => {
            const data: App.Window = (event as MessageEvent).data;
            console.log(data);

            if (Object.hasOwn(data, "m_state")) {
                // sessionStorage.setItem("state", JSON.stringify(data.m_state));
                setState(data.m_state);
            }

            if (Object.hasOwn(data, "m_layout")) {
                // sessionStorage.setItem("layout", JSON.stringify(data.m_layout));
                setLayout(data.m_layout);
            }

            // if (Object.hasOwn(data, "m_url")) {
            //     const [first, second] = data.m_url.current;
            //     setUrl((prevState) => ({ ...prevState, first: first, second: second }));
            //     sessionStorage.setItem("first", first);
            //     sessionStorage.setItem("second", second);
            // }

            // if (Object.hasOwn(data, "m_faviconUrl")) {
            //     const [first, second] = data.m_faviconUrl;

            //     first.length === 0
            //         ? setFavicon((prevState) => ({ ...prevState, first: defaultFavicon() }))
            //         : setFavicon((prevState) => ({ ...prevState, first: first }));

            //     second.length === 0
            //         ? setFavicon((prevState) => ({ ...prevState, second: defaultFavicon() }))
            //         : setFavicon((prevState) => ({ ...prevState, second: second }));
            // }

            // if (Object.hasOwn(data, "m_focus")) {
            //     if (data.m_focus === "first") {
            //         setFocus(data.m_focus);
            //     } else if (data.m_focus === "second") {
            //         setFocus(data.m_focus);
            //     }
            // }

            // if (Object.hasOwn(data, "m_colors")) {
            //     setColors(data.m_colors.colors);
            // }

            // if (Object.hasOwn(data, "m_layout")) {
            //     setLayout(data.m_layout);
            // }

            // if (Object.hasOwn(data, "navigate")) {
            //     const [first, second] = data.navigate;
            //     window.chrome.webview.postMessage({ first: parseUrl(first).href });
            //     window.chrome.webview.postMessage({ second: parseUrl(second).href });
            // }

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

    // const submitFirst = () => {
    //     const parsed = parseUrl(inputFirst.current?.value!).href;
    //     setUrl((prevState) => ({ ...prevState, first: parsed }));
    //     sessionStorage.setItem("first", parsed);
    //     window.chrome.webview.postMessage({ first: parsed });
    // };

    // const submitSecond = () => {
    //     const parsed = parseUrl(inputSecond.current?.value!).href;
    //     setUrl((prevState) => ({ ...prevState, second: parsed }));
    //     sessionStorage.setItem("second", parsed);
    //     window.chrome.webview.postMessage({ second: parsed });
    // };

    // const handleSubmit = (event: SyntheticEvent) => {
    //     event.preventDefault();

    //     if (document.activeElement === inputFirst.current && inputFirst.current!.value !== "") {
    //         submitFirst();
    //     }

    //     if (document.activeElement === inputSecond.current && inputSecond.current!.value !== "") {
    //         submitSecond();
    //     }
    // };

    // const handleClick = async (event: SyntheticEvent) => {
    //     let nativeEvent = event.nativeEvent as MouseEvent;

    //     if (document.activeElement === inputFirst.current) {
    //         if (nativeEvent.ctrlKey) await navigator.clipboard.writeText(url.first);
    //     } else if (document.activeElement === inputSecond.current) {
    //         if (nativeEvent.ctrlKey) await navigator.clipboard.writeText(url.second);
    //     }
    // };

    return (
        <>
            <form
                className="url-form"
                ref={form}
                id="form"
                method="post"
                autoComplete="off"
                spellCheck="false"
                // onSubmit={handleSubmit}
            >
                <label
                // className={`${layout.swapped ? "order-1" : "order-0"} ${
                //     !layout.split && layout.swapped ? "hidden" : "url"
                // } ${focus === "first" ? "url-focus" : "url-blur"}
                // }`}
                >
                    <img
                        className="url-favicon"
                        width="16"
                        height="16"
                        // src={favicon.first}
                    />
                    <input
                        className="url-input"
                        ref={first}
                        id="first"
                        type="text"
                        // value={url.first}
                        // placeholder={sessionStorage.getItem("first")!}
                        // title={sessionStorage.getItem("first")!}
                        // onChange={(e) =>
                        //     setUrl((prevState) => ({ ...prevState, first: e.target.value }))
                        // }
                        // onClick={handleClick}
                    />
                </label>
                <label
                // className={`${layout.swapped ? "order-0" : "order-1"} ${
                //     !layout.split && !layout.swapped ? "hidden" : "url"
                // } ${focus === "second" ? "url-focus" : "url-blur"}
                // }`}
                >
                    <img
                        className="url-favicon"
                        width="16"
                        height="16"
                        // src={favicon.second}
                    />
                    <input
                        className="url-input"
                        ref={second}
                        id="second"
                        type="text"
                        // value={url.second}
                        // placeholder={sessionStorage.getItem("second")!}
                        // title={sessionStorage.getItem("second")!}
                        // onChange={(e) =>
                        //     setUrl((prevState) => ({ ...prevState, second: e.target.value }))
                        // }
                        // onClick={handleClick}
                    ></input>
                </label>
                <input type="submit" hidden />
            </form>
        </>
    );
}
