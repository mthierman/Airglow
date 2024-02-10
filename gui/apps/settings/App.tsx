import "@css/index.css";
import { SyntheticEvent, useState, useRef, useEffect } from "react";
import { parseUrl } from "@libs/url";
import { initialize, applyColors } from "@libs/index";

export default () => {
    const form = useRef<HTMLFormElement | null>(null);
    const inputFirst = useRef<HTMLInputElement | null>(null);
    const inputSecond = useRef<HTMLInputElement | null>(null);
    const [state, setState] = useState<App.State>({
        args: ["about:blank", "about:blank"],
        colors: {
            accent: "",
            accentDark1: "",
            accentDark2: "",
            accentDark3: "",
            accentLight1: "",
            accentLight2: "",
            accentLight3: "",
        },
        home: ["about:blank", "about:blank"],
    });

    useEffect(() => {
        initialize();
    }, []);

    useEffect(() => {
        applyColors(state.colors);
    }, [state.colors]);

    useEffect(() => {
        const onMessage = (event: Event) => {
            const data: App.Settings = (event as MessageEvent).data;

            if (Object.hasOwn(data, "m_state")) {
                const state: App.State = data.m_state;
                setState(state);
            }
        };

        const onEscape = (event: KeyboardEvent) => {
            if (event.key === "Escape") {
                if (document.activeElement === inputFirst.current) {
                    setState((prevState) => ({
                        ...prevState,
                        home: [sessionStorage.getItem("first")!, prevState.home[1]],
                    }));
                } else if (document.activeElement === inputSecond.current) {
                    setState((prevState) => ({
                        ...prevState,
                        home: [prevState.home[0], sessionStorage.getItem("second")!],
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

    const handleChange = (e: SyntheticEvent) => {
        const target = e.target as HTMLInputElement;

        if (document.activeElement === inputFirst.current) {
            setState((prevState) => ({ ...prevState, home: [target.value, prevState.home[1]] }));
        } else if (document.activeElement === inputSecond.current) {
            setState((prevState) => ({ ...prevState, home: [prevState.home[0], target.value] }));
        }
    };

    const submitFirst = () => {
        const parsed = parseUrl(inputFirst.current?.value!).href;
        setState((prevState) => ({ ...prevState, home: [parsed, prevState.home[1]] }));
        sessionStorage.setItem("first", parsed);
        window.chrome.webview.postMessage({ first: parsed });
    };

    const submitSecond = () => {
        const parsed = parseUrl(inputSecond.current?.value!).href;
        setState((prevState) => ({ ...prevState, home: [prevState.home[0], parsed] }));
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
            if (nativeEvent.ctrlKey) await navigator.clipboard.writeText(state.home[0]);
        } else if (document.activeElement === inputSecond.current) {
            if (nativeEvent.ctrlKey) await navigator.clipboard.writeText(state.home[1]);
        }
    };

    return (
        <form
            className="settings-form"
            ref={form}
            id="form"
            method="post"
            autoComplete="off"
            spellCheck="false"
            onSubmit={handleSubmit}>
            <div className="settings-spacer">
                <label className="settings-spacer">
                    <h1 className="settings-title">🌆First Home</h1>
                    <input
                        className="settings-input"
                        ref={inputFirst}
                        id="first"
                        type="text"
                        value={state.home[0]}
                        placeholder={sessionStorage.getItem("first")!}
                        title={sessionStorage.getItem("first")!}
                        onChange={handleChange}
                        onClick={handleClick}></input>
                </label>
                <label className="settings-spacer">
                    <h1 className="settings-title">🌃Second Home</h1>
                    <input
                        className="settings-input"
                        ref={inputSecond}
                        id="second"
                        type="text"
                        value={state.home[1]}
                        placeholder={sessionStorage.getItem("second")!}
                        title={sessionStorage.getItem("second")!}
                        onChange={handleChange}
                        onClick={handleClick}></input>
                </label>
            </div>
            <input className="settings-submit" type="submit" value="Save" />
        </form>
    );
};
