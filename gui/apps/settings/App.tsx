import "@css/index.css";
import { defaultState, initialize } from "@libs/index";
import { parseUrl } from "@libs/url";
import { SyntheticEvent, useEffect, useRef, useState } from "react";

export default function App() {
    const first = useRef<HTMLInputElement | null>(null);
    const second = useRef<HTMLInputElement | null>(null);
    const [state, setState] = useState(defaultState());

    useEffect(() => {
        initialize();
    }, []);

    useEffect(() => {
        const onMessage = (event: Event) => {
            const data: App.Settings = (event as MessageEvent).data;
            // console.log(data);

            if (Object.hasOwn(data, "m_state")) {
                setState(data.m_state);
            }
        };

        // const onEscape = (event: KeyboardEvent) => {
        //     if (event.key === "Escape") {
        //         const store = getState();

        //         if (document.activeElement === first.current) {
        //             updateHome(0, store.home[0]);
        //         } else if (document.activeElement === second.current) {
        //             updateHome(1, store.home[1]);
        //         }
        //     }
        // };

        window.chrome.webview.addEventListener("message", onMessage);
        // document.addEventListener("keydown", onEscape);

        return () => {
            window.chrome.webview.removeEventListener("message", onMessage);
            // document.removeEventListener("keydown", onEscape);
        };
    });

    // const handleClick = async (event: SyntheticEvent) => {
    //     let nativeEvent = event.nativeEvent as MouseEvent;
    //     if (document.activeElement === first.current) {
    //         if (nativeEvent.ctrlKey) await navigator.clipboard.writeText(state.home[0]);
    //     } else if (document.activeElement === second.current) {
    //         if (nativeEvent.ctrlKey) await navigator.clipboard.writeText(state.home[1]);
    //     }
    // };

    // const submit = (element: HTMLInputElement) => {
    //     const parsed = parseUrl(element.value).href;
    //     setHome((prev) => ({ ...prev, [element.id]: parsed }));
    //     setHomeValue((prev) => ({ ...prev, [element.id]: parsed }));
    //     window.chrome.webview.postMessage({ [element.id]: parsed });
    // };

    const handleSubmit = (event: SyntheticEvent) => {
        event.preventDefault();

        if (document.activeElement === first.current) {
        } else if (document.activeElement === second.current) {
        } else {
        }
    };

    const onChange = (event: SyntheticEvent<HTMLInputElement, Event>) => {
        const {
            currentTarget: { id, value },
        } = event;

        setState({
            ...state,
            home: state.home.map((v, i) => (i === Number(id) ? value : v)) as Pair,
        });
    };

    useEffect(() => {
        console.log(state.home);
    }, [state]);

    return (
        <form
            className="settings-form"
            id="form"
            method="post"
            autoComplete="off"
            spellCheck="false"
            // onSubmit={handleSubmit}
        >
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
                        // placeholder={home.first}
                        // title={home.first}
                        // onClick={handleClick}
                    ></input>
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
                        // placeholder={home.second}
                        // title={home.second}
                        // onClick={handleClick}
                    ></input>
                </label>
            </div>
            <input className="settings-submit" type="submit" value="Save" />
        </form>
    );
}
