import { SyntheticEvent, useState, useRef, useEffect } from "react";
import * as url from "@libs/url";
import { getSessionStorage, getSystemColorsStorage } from "@libs/storage";

export default function App() {
    const form = useRef<HTMLFormElement | null>(null);
    const firstInput = useRef<HTMLInputElement | null>(null);
    const secondInput = useRef<HTMLInputElement | null>(null);

    const [systemColors, setSystemColors] = useState<App.SystemColors>(getSystemColorsStorage());

    const [first, setFirst] = useState<App.URL>({
        current: "",
        loaded: getSessionStorage("first", ""),
    });
    const [second, setSecond] = useState<App.URL>({
        current: "",
        loaded: getSessionStorage("second", ""),
    });

    useEffect(() => {
        window.chrome.webview.postMessage({ initialized: true });
        document.documentElement.style.setProperty("--accent", systemColors.accent);
        document.documentElement.style.setProperty("--accentDark1", systemColors.accentDark1);
        document.documentElement.style.setProperty("--accentDark2", systemColors.accentDark2);
        document.documentElement.style.setProperty("--accentDark3", systemColors.accentDark3);
        document.documentElement.style.setProperty("--accentLight1", systemColors.accentLight1);
        document.documentElement.style.setProperty("--accentLight2", systemColors.accentLight2);
        document.documentElement.style.setProperty("--accentLight3", systemColors.accentLight3);
    }, []);

    useEffect(() => {
        const onMessage = (event: Event) => {
            const data = (event as MessageEvent).data;
            console.log(data);

            if (data.systemColors) {
                setSystemColors(data.systemColors);
                sessionStorage.setItem("systemColors", JSON.stringify(data.systemColors));
            }

            if (data.first) {
                setFirst({ loaded: data.first, current: data.first });
                sessionStorage.setItem("first", data.first);
            }

            if (data.second) {
                setSecond({ loaded: data.second, current: data.second });
                sessionStorage.setItem("second", data.second);
            }
        };

        const onFocusOut = () => {
            if (document.activeElement === firstInput.current) {
                firstInput.current!.blur();
            } else if (document.activeElement === secondInput.current) {
                secondInput.current!.blur();
            }
        };

        const onEscape = (event: KeyboardEvent) => {
            const key = event.key;
            if (key === "Escape") {
                if (document.activeElement === firstInput.current) {
                    firstInput.current!.value = first.loaded;
                } else if (document.activeElement === secondInput.current) {
                    secondInput.current!.value = second.loaded;
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
        let form = event.target as HTMLFormElement;

        if (document.activeElement === firstInput.current) {
            const parsed = url.parseUrl(firstInput.current?.value!).href;
            setFirst({ loaded: parsed, current: parsed });
            sessionStorage.setItem("first", parsed);
            window.chrome.webview.postMessage({ first: parsed });
        }

        if (document.activeElement === secondInput.current) {
            const parsed = url.parseUrl(secondInput.current?.value!).href;
            setSecond({ loaded: parsed, current: parsed });
            sessionStorage.setItem("second", parsed);
            window.chrome.webview.postMessage({ second: parsed });
        }

        form.reset();
    };

    const handleChange = (event: SyntheticEvent) => {
        let input = event.target as HTMLInputElement;

        if (input.id === "firstInput") {
            setFirst((prevState) => ({ ...prevState, current: input.value }));
        } else if (input.id === "secondInput") {
            setSecond((prevState) => ({ ...prevState, current: input.value }));
        }
    };

    const handleClick = async (event: SyntheticEvent) => {
        let input = event.target as HTMLInputElement;
        let nativeEvent = event.nativeEvent as MouseEvent;

        if (input.id === "firstInput") {
            if (nativeEvent.ctrlKey) await navigator.clipboard.writeText(first.loaded);
        } else if (input.id === "secondInput") {
            if (nativeEvent.ctrlKey) await navigator.clipboard.writeText(second.loaded);
        }
    };

    return (
        <form
            className="grid min-w-max grid-flow-row gap-2 p-2 text-center"
            id="form"
            method="post"
            autoComplete="off"
            spellCheck="false"
            ref={form}
            onSubmit={handleSubmit}>
            <h1 className="setting">
                <span>🌆</span>
                <span className="settingTitle">First Home</span>
            </h1>
            <input
                className="input"
                type="text"
                id="firstInput"
                ref={firstInput}
                value={first.current}
                placeholder={first.loaded}
                title={first.loaded}
                onChange={handleChange}
                onClick={handleClick}></input>
            <h1 className="setting">
                <span>🌃</span>
                <span className="settingTitle">Second Home</span>
            </h1>
            <input
                className="input"
                type="text"
                id="secondInput"
                ref={secondInput}
                value={second.current}
                placeholder={second.loaded}
                title={second.loaded}
                onChange={handleChange}
                onClick={handleClick}></input>
            <input type="submit" hidden />
        </form>
    );
}
