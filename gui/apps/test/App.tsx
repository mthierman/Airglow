import { SyntheticEvent, useState, useRef, useEffect } from "react";
import * as url from "@libs/url";

interface Position {
    bar: number;
    border: number;
    horizontal: boolean;
    split: boolean;
    swapped: boolean;
}

interface URL {
    current: string;
    loaded: string;
}

export default function App() {
    const container = useRef<HTMLDivElement | null>(null);
    const firstForm = useRef<HTMLFormElement | null>(null);
    const secondForm = useRef<HTMLFormElement | null>(null);
    const firstInput = useRef<HTMLInputElement | null>(null);
    const secondInput = useRef<HTMLInputElement | null>(null);
    const [first, setFirst] = useState<URL>({ current: "", loaded: "" });
    const [second, setSecond] = useState<URL>({ current: "", loaded: "" });

    const [position, setPosition] = useState<Position>({
        bar: 0,
        border: 0,
        horizontal: false,
        split: false,
        swapped: false,
    });

    useEffect(() => {
        setPosition((prevState) => ({ ...prevState, bar: container.current!.offsetHeight }));
        window.chrome.webview.postMessage({ height: position.bar });
    }, [position.bar]);

    useEffect(() => {
        const onMessage = (event: Event) => {
            const data = (event as MessageEvent).data;
            console.log(data);

            if (data.layout) {
                setPosition(data.layout);
            }

            if (data.first) {
                // setFirst((prevState) => ({ ...prevState, current: data.first }));
                setFirst({ loaded: data.first, current: data.first });
            }

            if (data.second) {
                // setSecond((prevState) => ({ ...prevState, current: data.second }));
                setSecond({ loaded: data.second, current: data.second });
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
        const form = firstForm.current;
        const onFocusOut = () => {
            firstInput.current?.blur();
        };

        if (form) {
            form.addEventListener("focusout", onFocusOut);
        }

        return () => {
            if (form) {
                form.removeEventListener("focusout", onFocusOut);
            }
        };
    });

    useEffect(() => {
        const form = secondForm.current;
        const onFocusOut = () => {
            secondInput.current?.blur();
        };

        if (form) {
            form.addEventListener("focusout", onFocusOut);
        }

        return () => {
            if (form) {
                form.removeEventListener("focusout", onFocusOut);
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

    useEffect(() => {
        // const form = firstForm.current;
        // if (firstForm.current) {
        //     firstForm.current.addEventListener("focusout", () => {
        //         firstInput.current?.blur();
        //     });
        // }
    });

    useEffect(() => {
        // const form = secondForm.current;
        // if (form) {
        //     form.addEventListener("focusout", () => {
        //         secondInput.current?.blur();
        //     });
        // }
    });

    return (
        <div
            ref={container}
            id="container"
            className={`flex bg-transparent ${position.swapped ? "flex-row-reverse" : "flex-row"}`}>
            <form
                className="flex flex-grow"
                id="firstForm"
                method="post"
                onSubmit={handleSubmit}
                ref={firstForm}
                autoComplete="off"
                spellCheck="false">
                <input
                    className="flex-grow text-ellipsis bg-transparent p-2 text-center outline-none"
                    type="text"
                    id="firstInput"
                    value={first.current}
                    // defaultValue={first.loaded}
                    placeholder={first.loaded}
                    title={first.loaded}
                    ref={firstInput}
                    onChange={handleChange}></input>
            </form>

            <form
                className="flex flex-grow"
                id="secondForm"
                method="post"
                onSubmit={handleSubmit}
                ref={secondForm}
                autoComplete="off"
                spellCheck="false">
                <input
                    className="flex-grow text-ellipsis bg-transparent p-2 text-center outline-none"
                    type="text"
                    id="secondInput"
                    value={second.current}
                    // defaultValue={second.loaded}
                    placeholder={second.loaded}
                    title={second.loaded}
                    ref={secondInput}
                    onChange={handleChange}></input>
            </form>
        </div>
    );
}
