import { SyntheticEvent, useState, useLayoutEffect, useRef, useEffect } from "react";
import * as url from "@libs/url";
import { getSessionStorage, getSystemColorsStorage } from "@libs/storage";

export default function App() {
    const [devicePixelRatio, setDevicePixelRatio] = useState(window.devicePixelRatio);
    const [offsetHeight, setOffsetHeight] = useState(0);
    const [offsetWidth, setOffsetWidth] = useState(0);
    const settingsForm = useRef<HTMLFormElement | null>(null);
    const firstInput = useRef<HTMLInputElement | null>(null);
    const secondInput = useRef<HTMLInputElement | null>(null);
    const [first, setFirst] = useState<App.URL>({
        current: "",
        loaded: getSessionStorage("first", ""),
    });
    const [second, setSecond] = useState<App.URL>({
        current: "",
        loaded: getSessionStorage("second", ""),
    });
    const [systemColors, setSystemColors] = useState<App.SystemColors>(getSystemColorsStorage());

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
    }, [systemColors]);

    useLayoutEffect(() => {
        const onResize = () => {
            setDevicePixelRatio(window.devicePixelRatio);
        };

        addEventListener("resize", onResize);

        return () => {
            removeEventListener("resize", onResize);
        };
    });

    useLayoutEffect(() => {
        setOffsetHeight(settingsForm.current?.offsetHeight!);
        setOffsetWidth(settingsForm.current?.offsetWidth!);
        window.chrome.webview.postMessage({
            devicePixelRatio: devicePixelRatio,
            offsetHeight: offsetHeight * devicePixelRatio,
            offsetWidth: offsetWidth * devicePixelRatio,
        });
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
                setFirst({ loaded: data.first, current: data.first });
                sessionStorage.setItem("first", data.first);
            }

            if (data.second) {
                setSecond({ loaded: data.second, current: data.second });
                sessionStorage.setItem("second", data.second);
            }
        };

        window.chrome.webview.addEventListener("message", onMessage);

        return () => {
            window.chrome.webview.removeEventListener("message", onMessage);
        };
    });

    useEffect(() => {
        const onEscape = (event: KeyboardEvent) => {
            const key = event.key;
            if (event.ctrlKey && key === "r") event.preventDefault();
            switch (key) {
                case "Escape":
                    if (document.activeElement === firstInput.current) {
                        firstInput.current!.value = first.loaded;
                        break;
                    }
                    if (document.activeElement === secondInput.current) {
                        secondInput.current!.value = second.loaded;
                        break;
                    }
            }
        };

        document.addEventListener("keydown", onEscape);

        return () => {
            document.removeEventListener("keydown", onEscape);
        };
    });

    useEffect(() => {
        const onFocusOut = () => {
            firstInput.current?.blur();
            secondInput.current?.blur();
        };

        if (settingsForm.current) {
            settingsForm.current.addEventListener("focusout", onFocusOut);
        }

        return () => {
            if (settingsForm.current) {
                settingsForm.current.removeEventListener("focusout", onFocusOut);
            }
        };
    });

    const handleSubmit = (event: SyntheticEvent) => {
        event.preventDefault();
        let form = event.target as HTMLFormElement;

        if (firstInput.current == document.activeElement) {
            const parsed = url.parseUrl(firstInput.current?.value!).href;
            setFirst({ loaded: parsed, current: parsed });
            sessionStorage.setItem("first", parsed);
            window.chrome.webview.postMessage({ first: parsed });
        }

        if (secondInput.current == document.activeElement) {
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
        }

        if (input.id === "secondInput") {
            setSecond((prevState) => ({ ...prevState, current: input.value }));
        }
    };

    return (
        <form
            className="grid grid-flow-row gap-2 p-2 text-center min-w-max"
            id="settingsForm"
            method="post"
            autoComplete="off"
            spellCheck="false"
            ref={settingsForm}
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
                onChange={handleChange}></input>
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
                onChange={handleChange}></input>
            <input type="submit" hidden />
        </form>
    );
}
