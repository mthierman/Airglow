import "@css/index.css";
import { defaultState, initialize } from "@libs/index";
import { parseUrl } from "@libs/url";
import { SyntheticEvent, useEffect, useRef, useState } from "react";

export default function App() {
    const first = useRef<HTMLInputElement | null>(null);
    const second = useRef<HTMLInputElement | null>(null);

    const [home, setHome] = useState<PairObj>({ first: "", second: "" });
    const [homeValue, setHomeValue] = useState<PairObj>({
        first: "",
        second: "",
    });

    useEffect(() => {
        initialize();
    }, []);

    useEffect(() => {
        console.log(home);
    }, [home]);

    useEffect(() => {
        const onMessage = (event: Event) => {
            const data: App.Settings = (event as MessageEvent).data;
            // console.log(data);

            if (Object.hasOwn(data, "m_state")) {
                const home = data.m_state.home;
                setHome({ first: home[0], second: home[1] });
                setHomeValue({ first: home[0], second: home[1] });
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

    const submit = (element: HTMLInputElement) => {
        const parsed = parseUrl(element.value).href;
        setHome((prev) => ({ ...prev, [element.id]: parsed }));
        setHomeValue((prev) => ({ ...prev, [element.id]: parsed }));
        window.chrome.webview.postMessage({ [element.id]: parsed });
    };

    const handleSubmit = (event: SyntheticEvent) => {
        event.preventDefault();

        if (document.activeElement === first.current) {
            if (first.current) {
                submit(first.current);
            }
        } else if (document.activeElement === second.current) {
            if (second.current) {
                submit(second.current);
            }
        } else {
            if (first.current && second.current) {
                submit(first.current);
                submit(second.current);
            }
        }
    };

    // const handleClick = async (event: SyntheticEvent) => {
    //     let nativeEvent = event.nativeEvent as MouseEvent;
    //     if (document.activeElement === first.current) {
    //         if (nativeEvent.ctrlKey) await navigator.clipboard.writeText(state.home[0]);
    //     } else if (document.activeElement === second.current) {
    //         if (nativeEvent.ctrlKey) await navigator.clipboard.writeText(state.home[1]);
    //     }
    // };

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
                        value={homeValue.first}
                        onChange={(e) =>
                            setHomeValue((prev) => ({
                                ...prev,
                                first: e.target.value,
                            }))
                        }
                        placeholder={home.first}
                        title={home.first}
                        // onClick={handleClick}
                    ></input>
                </label>
                <label className="settings-spacer">
                    <h1 className="settings-title">🌃Second Home</h1>
                    <input
                        className="settings-input"
                        ref={second}
                        id="second"
                        type="text"
                        value={homeValue.second}
                        onChange={(e) =>
                            setHomeValue((prev) => ({
                                ...prev,
                                second: e.target.value,
                            }))
                        }
                        placeholder={home.second}
                        title={home.second}
                        // onClick={handleClick}
                    ></input>
                </label>
            </div>
            <input className="settings-submit" type="submit" value="Save" />
        </form>
    );
}
