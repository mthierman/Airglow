import "@css/index.css";
import { SyntheticEvent, useState, useRef, useEffect } from "react";
import { parseUrl } from "@libs/url";
import { initialize, getState, applyColors, applyFavicon } from "@libs/index";

export default () => {
    const first = useRef<HTMLInputElement | null>(null);
    const second = useRef<HTMLInputElement | null>(null);
    const [state, setState] = useState<App.State>(getState());

    useEffect(() => {
        initialize();
        applyFavicon();
    }, []);

    useEffect(() => {
        applyColors(state.colors);
    }, [state.colors]);

    const updateHome = (index: number, newValue: string) => {
        setState((prevState) => ({
            ...prevState,
            home: prevState.home.map((v, i) => (i === index ? newValue : v)) as Pair,
        }));
    };

    const getHome = (index: number) => {
        const store = getState();
        if (store) {
            return store.home[index];
        } else return state.home[index];
    };

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
                const store = getState();

                if (document.activeElement === first.current) {
                    updateHome(0, store.home[0]);
                } else if (document.activeElement === second.current) {
                    updateHome(1, store.home[1]);
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
            updateHome(0, target.value);
        } else if (document.activeElement === second.current) {
            updateHome(1, target.value);
        }
    };

    const parse = (index: number, element: HTMLInputElement | null) => {
        if (element) {
            const parsed = parseUrl(element.value).href;
            updateHome(index, parsed);
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
                        placeholder={getHome(0)}
                        title={getHome(0)}
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
                        placeholder={getHome(1)}
                        title={getHome(1)}
                        onChange={handleChange}
                        onClick={handleClick}></input>
                </label>
            </div>
            <input className="settings-submit" type="submit" value="Save" />
        </form>
    );
};
