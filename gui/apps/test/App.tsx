import { SyntheticEvent, useState, useRef, useEffect } from "react";
import * as url from "@libs/url";
import { getSystemColorsStorage } from "@libs/storage";

export default function App() {
    const [systemColors, setSystemColors] = useState<App.SystemColors>(getSystemColorsStorage());

    const form = useRef<HTMLFormElement | null>(null);
    const first = useRef<HTMLInputElement | null>(null);
    const second = useRef<HTMLInputElement | null>(null);

    const [firstCurrent, setFirstCurrent] = useState("");
    const [secondCurrent, setSecondCurrent] = useState("");

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
    });

    useEffect(() => {
        const onMessage = (event: Event) => {
            const data = (event as MessageEvent).data;
            console.log(data);

            if (data.systemColors) {
                setSystemColors(data.systemColors);
                sessionStorage.setItem("systemColors", JSON.stringify(data.systemColors));
            }

            if (data.first) {
                setFirstCurrent(data.first);
                sessionStorage.setItem("first", data.first);
            }

            if (data.second) {
                setSecondCurrent(data.second);
                sessionStorage.setItem("second", data.second);
            }
        };

        const onFocusOut = () => {
            if (document.activeElement === first.current) {
                first.current!.blur();
            } else if (document.activeElement === second.current) {
                second.current!.blur();
            }
        };

        const onEscape = (event: KeyboardEvent) => {
            const key = event.key;
            if (key === "Escape") {
                if (document.activeElement === first.current) {
                    first.current!.value = sessionStorage.getItem("first")!;
                } else if (document.activeElement === second.current) {
                    second.current!.value = sessionStorage.getItem("second")!;
                }
            }
        };

        window.chrome.webview.addEventListener("message", onMessage);
        form.current!.addEventListener("focusout", onFocusOut);
        document.addEventListener("keydown", onEscape);

        return () => {
            window.chrome.webview.removeEventListener("message", onMessage);
            form.current!.removeEventListener("focusout", onFocusOut);
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

    const handleChange = (event: SyntheticEvent) => {
        let input = event.target as HTMLInputElement;

        if (document.activeElement === first.current) {
            setFirstCurrent(input.value);
        } else if (document.activeElement === second.current) {
            setSecondCurrent(input.value);
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
            ref={form}
            id="form"
            method="post"
            autoComplete="off"
            spellCheck="false"
            onSubmit={handleSubmit}>
            <h1>🌆First Home</h1>
            <input
                ref={first}
                id="first"
                className="input"
                type="text"
                value={firstCurrent}
                placeholder={sessionStorage.getItem("first")!}
                title={sessionStorage.getItem("first")!}
                onChange={handleChange}
                onClick={handleClick}></input>
            <h1>🌃Second Home</h1>
            <input
                ref={second}
                id="second"
                className="input"
                type="text"
                value={secondCurrent}
                placeholder={sessionStorage.getItem("second")!}
                title={sessionStorage.getItem("second")!}
                onChange={handleChange}
                onClick={handleClick}></input>
            <input type="submit" hidden />
        </form>
    );
}
