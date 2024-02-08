import { SyntheticEvent, useState, useRef, useEffect } from "react";
import * as url from "@libs/url";
import { initialize, applyColors, defaultFavicon } from "@libs/storage";

export default function App() {
    const form = useRef<HTMLFormElement | null>(null);
    const inputFirst = useRef<HTMLInputElement | null>(null);
    const inputSecond = useRef<HTMLInputElement | null>(null);

    const [layout, setLayout] = useState<App.Layout>({
        bar: 0,
        border: 0,
        horizontal: false,
        split: false,
        swapped: false,
    });
    const [colors, setColors] = useState<App.Colors>({
        accent: "",
        accentDark1: "",
        accentDark2: "",
        accentDark3: "",
        accentLight1: "",
        accentLight2: "",
        accentLight3: "",
    });
    const [focus, setFocus] = useState("");
    const [firstCurrent, setFirstCurrent] = useState("");
    const [secondCurrent, setSecondCurrent] = useState("");
    const [favicon, setFavicon] = useState<App.Favicon>({
        first: defaultFavicon(),
        second: defaultFavicon(),
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
            // console.log(data);

            if (Object.hasOwn(data, "m_url")) {
                const [dataFirst, dataSecond] = data.m_url.current;
                setFirstCurrent(dataFirst);
                sessionStorage.setItem("first", dataFirst);
                setSecondCurrent(dataSecond);
                sessionStorage.setItem("second", dataSecond);
            }

            if (Object.hasOwn(data, "m_faviconUrl")) {
                const [dataFirst, dataSecond] = data.m_faviconUrl;

                if (dataFirst.length === 0) {
                    defaultFavicon();
                } else {
                    setFavicon((prevState) => ({ ...prevState, first: dataFirst }));
                }
                if (dataSecond.length === 0) {
                    defaultFavicon();
                } else {
                    setFavicon((prevState) => ({ ...prevState, second: dataSecond }));
                }
            }

            if (Object.hasOwn(data, "navigate")) {
                const [first, second] = data.navigate;
                window.chrome.webview.postMessage({ first: url.parseUrl(first).href });
                window.chrome.webview.postMessage({ second: url.parseUrl(second).href });
            }

            if (Object.hasOwn(data, "m_focus")) {
                const dataFocus = data.m_focus;
                if (dataFocus === "first") {
                    setFocus(dataFocus);
                } else if (dataFocus === "second") {
                    setFocus(dataFocus);
                }
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

            if (Object.hasOwn(data, "m_colors")) {
                const dataColors = data.m_colors.colors;
                setColors(dataColors);
                sessionStorage.setItem("colors", JSON.stringify(dataColors));
            }

            if (Object.hasOwn(data, "m_layout")) {
                const dataLayout = data.m_layout;
                setLayout(dataLayout);
                sessionStorage.setItem("layout", JSON.stringify(dataLayout));
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
                    } ${focus === "first" ? "focus" : ""}
                    }`}>
                    <img className="url-favicon" width="16" height="16" src={favicon.first} />
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
                    } ${focus === "second" ? "focus" : ""}
                    }`}>
                    <img className="url-favicon" width="16" height="16" src={favicon.second} />
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
