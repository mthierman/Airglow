import * as url from "@libs/url";
import { SyntheticEvent } from "react";
import { useState } from "react";

export default function App() {
    const [mainUrl, setMainUrl] = useState("");
    const [sideUrl, setSideUrl] = useState("");

    const handleMainChange = (event: SyntheticEvent) => {
        let input = event.target as HTMLInputElement;
        setMainUrl(input.value);
    };

    const handleMainSubmit = (event: SyntheticEvent) => {
        event.preventDefault();
        setMainUrl(url.parseUrl(mainUrl).href);
    };

    const handleSideChange = (event: SyntheticEvent) => {
        let input = event.target as HTMLInputElement;
        setSideUrl(input.value);
    };

    const handleSideSubmit = (event: SyntheticEvent) => {
        event.preventDefault();
        setSideUrl(url.parseUrl(sideUrl).href);
    };

    return (
        <div className="flex gap-2 bg-transparent p-2">
            <form
                className="flex-grow"
                name="mainForm"
                id="mainForm"
                method="post"
                onSubmit={handleMainSubmit}
                autoComplete="off"
                spellCheck="false">
                <input
                    className="w-full rounded-lg bg-flexoki-base-paper p-2 shadow-md shadow-flexoki-base-50 outline-none dark:bg-flexoki-base-950 dark:shadow-flexoki-base-black"
                    type="text"
                    name="mainUrl"
                    id="mainUrl"
                    value={mainUrl}
                    onChange={handleMainChange}></input>
            </form>

            <form
                className="flex-grow"
                name="sideForm"
                id="sideForm"
                method="post"
                onSubmit={handleSideSubmit}
                autoComplete="off"
                spellCheck="false">
                <input
                    className="w-full rounded-lg bg-flexoki-base-paper p-2 shadow-md shadow-flexoki-base-50 outline-none dark:bg-flexoki-base-950 dark:shadow-flexoki-base-black"
                    type="text"
                    name="sideUrl"
                    id="sideUrl"
                    value={sideUrl}
                    onChange={handleSideChange}></input>
            </form>
        </div>
    );
}
