import { SyntheticEvent, useState, useRef, useEffect } from "react";
import * as url from "@libs/url";
import { getSessionStorage } from "@libs/storage";

const getPositionStorage = () => {
    const value: App.Position = JSON.parse(sessionStorage.getItem("position")!);

    const defaultValue: App.Position = {
        bar: 0,
        border: 0,
        horizontal: false,
        split: false,
        swapped: false,
    };

    if (!value) {
        return defaultValue;
    } else {
        return value;
    }
};

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
    const [position, setPosition] = useState<App.Position>(getPositionStorage());

    useEffect(() => {
        setPosition((prevState) => ({ ...prevState, bar: container.current!.offsetHeight }));
        window.chrome.webview.postMessage({ height: position.bar });
        sessionStorage.setItem("position", JSON.stringify(position));
    }, [position.bar]);

    useEffect(() => {
        const onMessage = (event: Event) => {
            const data = (event as MessageEvent).data;

            if (data.layout) {
                setPosition(data.layout);
                sessionStorage.setItem("position", JSON.stringify(data.layout));
                setPosition((prevState) => ({
                    ...prevState,
                    bar: container.current!.offsetHeight,
                }));
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
        <div
            ref={container}
            id="container"
            className={`flex gap-2 bg-transparent p-2 ${
                position.swapped ? "flex-row-reverse" : "flex-row"
            }`}>
            <form
                className={`flex flex-grow ${!position.split && position.swapped ? "hidden" : ""}`}
                id="firstForm"
                method="post"
                onSubmit={handleSubmit}
                ref={firstForm}
                autoComplete="off"
                spellCheck="false">
                <input
                    className="input flex-grow"
                    type="text"
                    id="firstInput"
                    value={first.current}
                    placeholder={first.loaded}
                    title={first.loaded}
                    ref={firstInput}
                    onChange={handleChange}
                    onClick={handleClick}></input>
            </form>

            <form
                className={`flex flex-grow ${!position.split && !position.swapped ? "hidden" : ""}`}
                id="secondForm"
                method="post"
                onSubmit={handleSubmit}
                ref={secondForm}
                autoComplete="off"
                spellCheck="false">
                <input
                    className="input flex-grow"
                    type="text"
                    id="secondInput"
                    value={second.current}
                    placeholder={second.loaded}
                    title={second.loaded}
                    ref={secondInput}
                    onChange={handleChange}
                    onClick={handleClick}></input>
            </form>
        </div>
    );
}
