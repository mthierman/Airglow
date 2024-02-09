import { SyntheticEvent, useState, useRef, useEffect } from "react";
import { parseUrl } from "@libs/url";
import { initialize, applyColors } from "@libs/index";

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
    const [url, setUrl] = useState<App.Pair>({
        first: "",
        second: "",
    });
    // const [firstCurrent, setFirstCurrent] = useState("");
    // const [secondCurrent, setSecondCurrent] = useState("");

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
                setColors(data.m_colors.colors);
                sessionStorage.setItem("colors", JSON.stringify(data.m_colors.colors));
            }

            if (Object.hasOwn(data, "m_url")) {
                const [first, second] = data.m_url.home;
                setUrl((prevState) => ({ ...prevState, first: first, second: second }));
                sessionStorage.setItem("first", first);
                sessionStorage.setItem("second", second);
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

        window.chrome.webview.addEventListener("message", onMessage);
        document.addEventListener("keydown", onEscape);

        return () => {
            window.chrome.webview.removeEventListener("message", onMessage);
            document.removeEventListener("keydown", onEscape);
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

        if (document.activeElement === inputFirst.current) {
            submitFirst();
        } else if (document.activeElement === inputSecond.current) {
            submitSecond();
        } else {
            submitFirst();
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
                ref={inputFirst}
                id="first"
                type="text"
                value={url.first}
                placeholder={sessionStorage.getItem("first")!}
                title={sessionStorage.getItem("first")!}
                onChange={(e) => setUrl((prevState) => ({ ...prevState, first: e.target.value }))}
                onClick={handleClick}></input>
            <h1 className="settings-title">🌃Second Home</h1>
            <input
                className="settings-input"
                ref={inputSecond}
                id="second"
                type="text"
                value={url.second}
                placeholder={sessionStorage.getItem("second")!}
                title={sessionStorage.getItem("second")!}
                onChange={(e) => setUrl((prevState) => ({ ...prevState, second: e.target.value }))}
                onClick={handleClick}></input>
            <input className="settings-submit" type="submit" />
        </form>
    );
}
