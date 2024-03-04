import "#css/index.css";
import { useColors, useInitializer, defaultState } from "#libs/common";
import { parseUrl } from "#libs/url";
import { SyntheticEvent, useEffect, useRef, useState } from "react";

export default function App() {
    const first = useRef<HTMLInputElement | null>(null);
    const second = useRef<HTMLInputElement | null>(null);
    const [state, setState] = useState(defaultState());

    useInitializer();
    useColors(state.colors);

    useEffect(() => {
        const onMessage = (event: Event) => {
            const data = (event as MessageEvent).data as App.Settings;

            if (Object.hasOwn(data, "m_state")) {
                setState(data.m_state);
            }
        };

        window.chrome.webview.addEventListener("message", onMessage);

        return () => {
            window.chrome.webview.removeEventListener("message", onMessage);
        };
    });

    const mapHome = (value: string, id: string): App.State => {
        return {
            ...state,
            home: state.home.map((v, i) => (i === Number(id) ? value : v)) as Pair,
        };
    };

    const onChange = (event: SyntheticEvent<HTMLInputElement, Event>) => {
        const {
            currentTarget: { id, value },
        } = event;

        setState(mapHome(value, id));
    };

    const parseInput = (element: HTMLInputElement) => {
        const { id, value } = element;
        const parsed = parseUrl(value).href;

        return mapHome(parsed, id);
    };

    const postState = (newState: App.State) => {
        window.chrome.webview.postMessage({ m_state: newState });
    };

    const handleSubmit = (event: SyntheticEvent<HTMLFormElement, SubmitEvent>) => {
        event.preventDefault();

        if (document.activeElement === first.current) {
            if (first.current?.value !== "") postState(parseInput(first.current!));
        } else if (document.activeElement === second.current) {
            if (second.current?.value !== "") postState(parseInput(second.current!));
        } else {
            postState({
                ...state,
                home: [parseUrl(state.home[0]).href, parseUrl(state.home[1]).href],
            });
        }
    };

    const handleClick = async (event: SyntheticEvent) => {
        const nativeEvent = event.nativeEvent as MouseEvent;

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
                        id="0"
                        type="text"
                        value={state.home[0]}
                        onChange={onChange}
                        onClick={(e) => {
                            void handleClick(e);
                        }}
                    />
                </label>
                <label className="settings-spacer">
                    <h1 className="settings-title">🌃Second Home</h1>
                    <input
                        className="settings-input"
                        ref={second}
                        id="1"
                        type="text"
                        value={state.home[1]}
                        onChange={onChange}
                        onClick={(e) => {
                            void handleClick(e);
                        }}
                    />
                </label>
            </div>
            <input className="settings-submit" type="submit" value="Save" />
        </form>
    );
}
