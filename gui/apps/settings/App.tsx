import "@css/index.css";
import { SyntheticEvent, useState, useRef, useEffect } from "react";
import { parseUrl } from "@libs/url";
import { initialize, getState, applyColors } from "@libs/index";

export default () => {
    const first = useRef<HTMLInputElement | null>(null);
    const second = useRef<HTMLInputElement | null>(null);
    const [state, setState] = useState<App.State>(getState());

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
                sessionStorage.setItem("state", JSON.stringify(data.m_state));
                setState(data.m_state);
            }
        };

        const onEscape = (event: KeyboardEvent) => {
            if (event.key === "Escape") {
                const obj: App.State = JSON.parse(sessionStorage.getItem("state")!);
                if (document.activeElement === first.current) {
                    setState((prevState) => ({
                        ...prevState,
                        home: [obj.home[0], prevState.home[1]],
                    }));
                } else if (document.activeElement === second.current) {
                    setState((prevState) => ({
                        ...prevState,
                        home: [prevState.home[0], obj.home[1]],
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

        if (document.activeElement === first.current) {
            setState((prevState) => ({ ...prevState, home: [target.value, prevState.home[1]] }));
        } else if (document.activeElement === second.current) {
            setState((prevState) => ({ ...prevState, home: [prevState.home[0], target.value] }));
        }
    };

    const parse = (index: number, element: HTMLInputElement | null) => {
        if (element) {
            const parsed = parseUrl(element.value).href;
            setState((prevState) => ({
                ...prevState,
                home: prevState.home.map((item, i) => (i === index ? parsed : item)) as Pair,
            }));
            return parsed;
        }
    };

    const handleSubmit = (event: SyntheticEvent) => {
        event.preventDefault();

        let post: App.State = state;

        if (document.activeElement === first.current) {
            post.home[0] = parse(0, first.current)!;
        } else if (document.activeElement === second.current) {
            post.home[1] = parse(1, second.current)!;
        } else {
            post.home[0] = parse(0, first.current)!;
            post.home[1] = parse(1, second.current)!;
        }

        sessionStorage.setItem("state", JSON.stringify(post));
        window.chrome.webview.postMessage({ m_state: post } as App.Settings);
    };

    const handleClick = async (event: SyntheticEvent) => {
        let nativeEvent = event.nativeEvent as MouseEvent;
        if (document.activeElement === first.current) {
            if (nativeEvent.ctrlKey) await navigator.clipboard.writeText(state.home[0]);
        } else if (document.activeElement === second.current) {
            if (nativeEvent.ctrlKey) await navigator.clipboard.writeText(state.home[1]);
        }
    };

    return (
        <form
            className="settings-form"
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
                        ref={first}
                        id="first"
                        type="text"
                        value={state.home[0]}
                        placeholder={(() => {
                            const obj: App.State = JSON.parse(sessionStorage.getItem("state")!);
                            if (obj) {
                                return obj.home[0];
                            } else return state.home[0];
                        })()}
                        title={(() => {
                            const obj: App.State = JSON.parse(sessionStorage.getItem("state")!);
                            if (obj) {
                                return obj.home[0];
                            } else return state.home[0];
                        })()}
                        onChange={handleChange}
                        onClick={handleClick}></input>
                </label>
                <label className="settings-spacer">
                    <h1 className="settings-title">🌃Second Home</h1>
                    <input
                        className="settings-input"
                        ref={second}
                        id="second"
                        type="text"
                        value={state.home[1]}
                        placeholder={(() => {
                            const obj: App.State = JSON.parse(sessionStorage.getItem("state")!);
                            if (obj) {
                                return obj.home[1];
                            } else return state.home[1];
                        })()}
                        title={(() => {
                            const obj: App.State = JSON.parse(sessionStorage.getItem("state")!);
                            if (obj) {
                                return obj.home[1];
                            } else return state.home[1];
                        })()}
                        onChange={handleChange}
                        onClick={handleClick}></input>
                </label>
            </div>
            <input className="settings-submit" type="submit" value="Save" />
        </form>
    );
};
