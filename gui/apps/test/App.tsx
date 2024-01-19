import { SyntheticEvent, useState, useRef, useEffect } from "react";
// import { Button } from "@components/Button";

export default function App() {
    const container = useRef<HTMLDivElement | null>(null);
    const settingsForm = useRef<HTMLFormElement | null>(null);

    const handleSubmit = (event: SyntheticEvent) => {
        event.preventDefault();
        let form = event.target as HTMLFormElement;
    };

    return (
        <div ref={container} id="container">
            <form
                className="grid grid-flow-row gap-4 p-4"
                id="settingsForm"
                method="post"
                onSubmit={handleSubmit}
                ref={settingsForm}
                autoComplete="off"
                spellCheck="false">
                <input className="input" type="text" id="firstInput"></input>
                <input className="input" type="text" id="firstInput"></input>
            </form>
        </div>
    );
}
