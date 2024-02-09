import { SyntheticEvent, useState, useRef, useEffect } from "react";
import { parseUrl } from "@libs/url";
import { initialize, applyColors, defaultFavicon } from "@libs/index";

export default function App() {
    const form = useRef<HTMLFormElement | null>(null);
    const inputFirst = useRef<HTMLInputElement | null>(null);
    const inputSecond = useRef<HTMLInputElement | null>(null);

    const [colors, setColors] = useState<App.Colors>({
        accent: "",
        accentDark1: "",
        accentDark2: "",
        accentDark3: "",
        accentLight1: "",
        accentLight2: "",
        accentLight3: "",
    });
    const [favicon, setFavicon] = useState<App.Pair>({
        first: defaultFavicon(),
        second: defaultFavicon(),
    });
    const [focus, setFocus] = useState("");
    const [layout, setLayout] = useState<App.Layout>({
        bar: 0,
        border: 0,
        horizontal: false,
        split: false,
        swapped: false,
    });
    const [url, setUrl] = useState<App.Pair>({
        first: "",
        second: "",
    });

    useEffect(() => {
        window.chrome.webview.postMessage({ height: form.current?.offsetHeight });
        initialize();
    }, []);

    useEffect(() => {
        applyColors(colors);
    }, [colors]);

    useEffect(() => {
        const onMessage = (event: Event) => {
            const data: App.Window = (event as MessageEvent).data;
            console.log(data);

            if (Object.hasOwn(data, "m_url")) {
                const [first, second] = data.m_url.current;
                setUrl((prevState) => ({ ...prevState, first: first, second: second }));
                sessionStorage.setItem("first", first);
                sessionStorage.setItem("second", second);
            }

            if (Object.hasOwn(data, "m_faviconUrl")) {
                const [first, second] = data.m_faviconUrl;

                if (first.length === 0) {
                    defaultFavicon();
                } else {
                    setFavicon((prevState) => ({ ...prevState, first: first }));
                }
                if (second.length === 0) {
                    defaultFavicon();
                } else {
                    setFavicon((prevState) => ({ ...prevState, second: second }));
                }
            }

            if (Object.hasOwn(data, "m_focus")) {
                if (data.m_focus === "first") {
                    setFocus(data.m_focus);
                } else if (data.m_focus === "second") {
                    setFocus(data.m_focus);
                }
            }

            if (Object.hasOwn(data, "m_colors")) {
                setColors(data.m_colors.colors);
                sessionStorage.setItem("colors", JSON.stringify(data.m_colors.colors));
            }

            if (Object.hasOwn(data, "m_layout")) {
                setLayout(data.m_layout);
                sessionStorage.setItem("layout", JSON.stringify(data.m_layout));
            }

            if (Object.hasOwn(data, "navigate")) {
                const [first, second] = data.navigate;
                window.chrome.webview.postMessage({ first: parseUrl(first).href });
                window.chrome.webview.postMessage({ second: parseUrl(second).href });
            }

            if (Object.hasOwn(data, "focus")) {
                if (focus === "first") {
                    inputFirst.current!.focus();
                    inputFirst.current!.select();
                } else if (focus === "second") {
                    inputSecond.current!.focus();
                    inputSecond.current!.select();
                }
            }
        };

        const onEscape = (event: KeyboardEvent) => {
            if (event.key === "Escape") {
                if (document.activeElement === inputFirst.current) {
                    setUrl((prevState) => ({
                        ...prevState,
                        first: sessionStorage.getItem("first")!,
                    }));
                } else if (document.activeElement === inputSecond.current) {
                    setUrl((prevState) => ({
                        ...prevState,
                        second: sessionStorage.getItem("second")!,
                    }));
                }
            }
        };

        const onFocus = () => {
            if (document.activeElement === inputFirst.current) {
                setFocus("first");
            } else if (document.activeElement === inputSecond.current) {
                setFocus("second");
            }
        };

        const onWindowBlur = () => {
            inputFirst.current?.blur();
            inputSecond.current?.blur();
        };

        window.chrome.webview.addEventListener("message", onMessage);
        document.addEventListener("keydown", onEscape);
        inputFirst.current?.addEventListener("focus", onFocus);
        inputSecond.current?.addEventListener("focus", onFocus);
        window.addEventListener("blur", onWindowBlur);

        return () => {
            window.chrome.webview.removeEventListener("message", onMessage);
            document.removeEventListener("keydown", onEscape);
            inputFirst.current?.removeEventListener("focus", onFocus);
            inputSecond.current?.removeEventListener("focus", onFocus);
            window.removeEventListener("blur", onWindowBlur);
        };
    });

    const submitFirst = () => {
        const parsed = parseUrl(inputFirst.current?.value!).href;
        setUrl((prevState) => ({ ...prevState, first: parsed }));
        sessionStorage.setItem("first", parsed);
        window.chrome.webview.postMessage({ first: parsed });
    };

    const submitSecond = () => {
        const parsed = parseUrl(inputSecond.current?.value!).href;
        setUrl((prevState) => ({ ...prevState, second: parsed }));
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
            if (nativeEvent.ctrlKey) await navigator.clipboard.writeText(url.first);
        } else if (document.activeElement === inputSecond.current) {
            if (nativeEvent.ctrlKey) await navigator.clipboard.writeText(url.second);
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
                    } ${focus === "first" ? "focus" : ""}
                    }`}>
                    <img className="url-favicon" width="16" height="16" src={favicon.first} />
                    <input
                        className="url-input"
                        ref={inputFirst}
                        id="first"
                        type="text"
                        value={url.first}
                        placeholder={sessionStorage.getItem("first")!}
                        title={sessionStorage.getItem("first")!}
                        onChange={(e) =>
                            setUrl((prevState) => ({ ...prevState, first: e.target.value }))
                        }
                        onClick={handleClick}
                    />
                </label>
                <label
                    className={`${layout.swapped ? "order-0" : "order-1"} ${
                        !layout.split && !layout.swapped ? "hidden" : "url"
                    } ${focus === "second" ? "focus" : ""}
                    }`}>
                    <img className="url-favicon" width="16" height="16" src={favicon.second} />
                    <input
                        className="url-input"
                        ref={inputSecond}
                        id="second"
                        type="text"
                        value={url.second}
                        placeholder={sessionStorage.getItem("second")!}
                        title={sessionStorage.getItem("second")!}
                        onChange={(e) =>
                            setUrl((prevState) => ({ ...prevState, second: e.target.value }))
                        }
                        onClick={handleClick}></input>
                </label>
                <input type="submit" hidden />
            </form>
        </>
    );
}
