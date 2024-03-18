import "css/index.css";
import { defaultState, url, useColors, useInitializer } from "libs/common";
import { AppSettings, pair } from "libs/types";
import { SyntheticEvent, useEffect, useRef, useState } from "react";

export default function App() {
    const first = useRef<HTMLInputElement | null>(null);
    const second = useRef<HTMLInputElement | null>(null);
    const [state, setState] = useState(defaultState());

    useInitializer();

    useColors(state.colors);

    useEffect(() => {
        const onMessage = (event: Event) => {
            const data = (event as MessageEvent).data as AppSettings;

            if (Object.hasOwn(data, "m_state")) {
                setState(data.m_state);
            }
        };

        window.chrome.webview.addEventListener("message", onMessage);

        return () => {
            window.chrome.webview.removeEventListener("message", onMessage);
        };
    }, []);

    const onChange = (event: SyntheticEvent<HTMLInputElement, Event>) => {
        const { id, value } = event.currentTarget;

        setState({
            ...state,
            home: state.home.map((v, i) => (i === Number(id) ? value : v)) as pair,
        });
    };

    const parseInput = (element: HTMLInputElement) => {
        const { id, value } = element;

        return {
            ...state,
            home: state.home.map((v, i) => (i === Number(id) ? url(value) : v)) as pair,
        };
    };

    const handleSubmit = (event: SyntheticEvent<HTMLFormElement, SubmitEvent>) => {
        event.preventDefault();

        let newState = defaultState();

        if (document.activeElement === first.current) {
            if (first.current?.value !== "") {
                newState = parseInput(first.current!);
            }
        } else if (document.activeElement === second.current) {
            if (second.current?.value !== "") {
                newState = parseInput(second.current!);
            }
        } else {
            newState = {
                ...state,
                home: [url(state.home[0])!, url(state.home[1])!],
            };
        }

        window.chrome.webview.postMessage({ m_state: newState });
    };

    const handleClick = async (event: SyntheticEvent) => {
        const nativeEvent = event.nativeEvent as MouseEvent;

        if (document.activeElement === first.current) {
            if (nativeEvent.ctrlKey) {
                await navigator.clipboard.writeText(state.home[0]);
            }
        } else if (document.activeElement === second.current) {
            if (nativeEvent.ctrlKey) {
                await navigator.clipboard.writeText(state.home[1]);
            }
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
