import { SyntheticEvent, useState, useRef, useEffect } from "react";
import * as url from "@libs/url";
import { getSessionStorage, getPositionStorage, getSystemColorsStorage } from "@libs/storage";

export default function App() {
    const container = useRef<HTMLDivElement | null>(null);
    const firstForm = useRef<HTMLFormElement | null>(null);
    const secondForm = useRef<HTMLFormElement | null>(null);
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
    const [position, setPosition] = useState<App.Layout>(getPositionStorage());
    const [systemColors, setSystemColors] = useState<App.SystemColors>(getSystemColorsStorage());

    useEffect(() => {
        window.chrome.webview.postMessage({ initialized: true });
    }, []);

    useEffect(() => {
        setPosition((prevState) => ({ ...prevState, bar: container.current!.offsetHeight }));
        window.chrome.webview.postMessage({ height: position.bar });
        sessionStorage.setItem("position", JSON.stringify(position));
    }, [position.bar]);

    useEffect(() => {
        document.documentElement.style.setProperty("--accent", systemColors.accent);
        document.documentElement.style.setProperty("--accentDark1", systemColors.accentDark1);
        document.documentElement.style.setProperty("--accentDark2", systemColors.accentDark2);
        document.documentElement.style.setProperty("--accentDark3", systemColors.accentDark3);
        document.documentElement.style.setProperty("--accentLight1", systemColors.accentLight1);
        document.documentElement.style.setProperty("--accentLight2", systemColors.accentLight2);
        document.documentElement.style.setProperty("--accentLight3", systemColors.accentLight3);
    }, [systemColors]);

    useEffect(() => {
        const onMessage = (event: Event) => {
            const data = (event as MessageEvent).data;
            console.log(data);

            if (data.layout) {
                setPosition(data.layout);
                sessionStorage.setItem("position", JSON.stringify(data.layout));
                setPosition((prevState) => ({
                    ...prevState,
                    bar: container.current!.offsetHeight,
                }));
            }

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

        if (firstForm.current) {
            firstForm.current.addEventListener("focusout", onFocusOut);
        }

        if (secondForm.current) {
            secondForm.current.addEventListener("focusout", onFocusOut);
        }

        return () => {
            if (firstForm.current) {
                firstForm.current.removeEventListener("focusout", onFocusOut);
            }

            if (secondForm.current) {
                secondForm.current.removeEventListener("focusout", onFocusOut);
            }
        };
    });

    const handleSubmit = (event: SyntheticEvent) => {
        event.preventDefault();
        let form = event.target as HTMLFormElement;

        if (form.id === "firstForm") {
            if (firstInput.current?.value !== "") {
                let parsed = url.parseUrl(firstInput.current?.value!).href;
                window.chrome.webview.postMessage({ first: parsed });
            }
        }

        if (form.id === "secondForm") {
            if (secondInput.current?.value !== "") {
                let parsed = url.parseUrl(secondInput.current?.value!).href;
                window.chrome.webview.postMessage({ second: parsed });
            }
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

    const handleClick = async (event: SyntheticEvent) => {
        let input = event.target as HTMLInputElement;
        let nativeEvent = event.nativeEvent as MouseEvent;

        if (input.id === "firstInput") {
            if (nativeEvent.ctrlKey) await navigator.clipboard.writeText(first.loaded);
        }

        if (input.id === "secondInput") {
            if (nativeEvent.ctrlKey) await navigator.clipboard.writeText(second.loaded);
        }
    };

    return (
        <div ref={container} id="container" className="grid grid-flow-col">
            <form
                className={`${position.swapped ? "order-1" : "order-0"} ${
                    !position.split && position.swapped ? "hidden" : ""
                }`}
                id="firstForm"
                method="post"
                autoComplete="off"
                spellCheck="false"
                ref={firstForm}
                onSubmit={handleSubmit}>
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
            </form>

            <form
                className={`${position.swapped ? "order-0" : "order-1"} ${
                    !position.split && !position.swapped ? "hidden" : ""
                }`}
                id="secondForm"
                method="post"
                onSubmit={handleSubmit}
                ref={secondForm}
                autoComplete="off"
                spellCheck="false">
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
            </form>
        </div>
    );
}
