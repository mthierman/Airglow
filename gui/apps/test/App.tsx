import { SyntheticEvent, useState, useRef, useEffect } from "react";
import * as url from "@libs/url";
import { getSessionStorage } from "@libs/storage";
import { URL } from "@libs/types";

export default function App() {
    const container = useRef<HTMLDivElement | null>(null);
    const settingsForm = useRef<HTMLFormElement | null>(null);
    const firstInput = useRef<HTMLInputElement | null>(null);
    const secondInput = useRef<HTMLInputElement | null>(null);

    const [first, setFirst] = useState<URL>({
        current: "",
        loaded: getSessionStorage("first", ""),
    });

    const [second, setSecond] = useState<URL>({
        current: "",
        loaded: getSessionStorage("second", ""),
    });

    const handleSubmit = (event: SyntheticEvent) => {
        event.preventDefault();
        let form = event.target as HTMLFormElement;

        if (firstInput.current?.value !== "") {
            let parsed = url.parseUrl(firstInput.current?.value!).href;
            setFirst({ loaded: parsed, current: parsed });
            sessionStorage.setItem("first", parsed);
            firstInput.current?.setAttribute("value", first.loaded);
            // window.chrome.webview.postMessage({ first: parsed });
        }

        if (secondInput.current?.value === "") {
        }

        form.reset();
    };

    return (
        <div ref={container} id="container" className="p-4">
            <form
                className="grid grid-flow-row gap-2 text-center"
                id="settingsForm"
                method="post"
                onSubmit={handleSubmit}
                ref={settingsForm}
                autoComplete="off"
                spellCheck="false">
                <h1 className="setting">
                    <span>🌆</span>
                    <span className="settingTitle">First Home</span>
                </h1>
                <input ref={firstInput} className="input" type="text" id="firstInput"></input>
                <h1 className="setting">
                    <span>🌃</span>
                    <span className="settingTitle">Second Home</span>
                </h1>
                <input ref={secondInput} className="input" type="text" id="secondInput"></input>
                <input type="submit" hidden />
            </form>
        </div>
    );
}
