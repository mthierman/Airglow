import { SyntheticEvent, useState, useRef, useEffect } from "react";
import * as url from "@libs/url";
import { getColorStorage, applyColors, initialize } from "@libs/index";

export default function App() {
    const [colors, setColors] = useState<App.Colors>(getColorStorage());
    const form = useRef<HTMLFormElement | null>(null);
    const first = useRef<HTMLInputElement | null>(null);
    const second = useRef<HTMLInputElement | null>(null);
    const [firstCurrent, setFirstCurrent] = useState("");
    const [secondCurrent, setSecondCurrent] = useState("");

    useEffect(() => {
        initialize();
    }, []);

    useEffect(() => {
        applyColors(colors);
    }, [colors]);

    useEffect(() => {
        const onMessage = (event: Event) => {
            const data: App.Settings = (event as MessageEvent).data;

            if (Object.hasOwn(data, "m_colors")) {
                const colors = data.m_colors.colors;
                setColors(colors);
                sessionStorage.setItem("colors", JSON.stringify(colors));
            }

            if (Object.hasOwn(data, "m_url")) {
                const [first, second] = data.m_url.home;
                setFirstCurrent(first);
                sessionStorage.setItem("first", first);
                setSecondCurrent(second);
                sessionStorage.setItem("second", second);
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

    const submitFirst = () => {
        const parsed = url.parseUrl(first.current?.value!).href;
        setFirstCurrent(parsed);
        sessionStorage.setItem("first", parsed);
        window.chrome.webview.postMessage({ first: parsed });
    };

    const submitSecond = () => {
        const parsed = url.parseUrl(second.current?.value!).href;
        setSecondCurrent(parsed);
        sessionStorage.setItem("second", parsed);
        window.chrome.webview.postMessage({ second: parsed });
    };

    const handleSubmit = (event: SyntheticEvent) => {
        event.preventDefault();

        if (document.activeElement === first.current) {
            submitFirst();
        } else if (document.activeElement === second.current) {
            submitSecond();
        } else {
            submitFirst();
            submitSecond();
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
        <form
            className="grid gap-4 p-4"
            ref={form}
            id="form"
            method="post"
            autoComplete="off"
            spellCheck="false"
            onSubmit={handleSubmit}>
            <h1 className="settings-title">🌆First Home</h1>
            <input
                className="settings-input"
                ref={first}
                id="first"
                type="text"
                value={firstCurrent}
                placeholder={sessionStorage.getItem("first")!}
                title={sessionStorage.getItem("first")!}
                onChange={(e) => setFirstCurrent(e.target.value)}
                onClick={handleClick}></input>
            <h1 className="settings-title">🌃Second Home</h1>
            <input
                className="settings-input"
                ref={second}
                id="second"
                type="text"
                value={secondCurrent}
                placeholder={sessionStorage.getItem("second")!}
                title={sessionStorage.getItem("second")!}
                onChange={(e) => setSecondCurrent(e.target.value)}
                onClick={handleClick}></input>
            <input className="settings-submit" type="submit" />
        </form>
    );
}
