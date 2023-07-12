import {
    makeStyles,
    FluentProvider,
    webLightTheme,
    webDarkTheme,
    Button,
    Input,
    Label,
} from "@fluentui/react-components";
import React, { useEffect, useState } from "react";

const useStyles = makeStyles({
    provider: {
        backgroundColor: "transparent",
        height: "100%",
    },
});

export default function App() {
    const styles = useStyles();

    const [mainUrl, setMainUrl] = useState("");
    const [sideUrl, setSideUrl] = useState("");

    const handleForm = (e: any) => {
        e.preventDefault();
        const rawData = new FormData(e.target);
        const data = Object.fromEntries(rawData.entries());
        if (data.mainUrl.toString() != "") {
            setMainUrl(data.mainUrl.toString());
            window.chrome.webview.postMessage(
                `mainUrl ${data.mainUrl.toString()}`,
            );
        }
        if (data.sideUrl.toString() != "") {
            setSideUrl(data.sideUrl.toString());
            window.chrome.webview.postMessage(
                `sideUrl ${data.sideUrl.toString()}`,
            );
        }
    };

    useEffect(() => {
        const getData = async () => {
            try {
                const response = await fetch(`https://airglow/state.json`);
                let data = await response.json();
                setMainUrl(data.mainUrl);
                setSideUrl(data.sideUrl);
            } catch (error) {
                console.error("error", error);
            }
        };
        getData();
    }, []);

    return (
        <FluentProvider className={styles.provider} theme={webDarkTheme}>
            <form
                autoComplete="on"
                id="settings"
                method="post"
                onSubmit={handleForm}>
                <div id="url">
                    <Label>
                        <h1>
                            <span className="select-none">🏠 https://</span>
                            {mainUrl}
                        </h1>
                        <Input
                            type="text"
                            name="mainUrl"
                            id="mainUrl"
                            // placeholder={mainUrl}
                            contentBefore="https://"
                            pattern=".*[.].*"></Input>
                    </Label>

                    <Label>
                        <h1>
                            <span className="select-none">🔧 https://</span>
                            {sideUrl}
                        </h1>
                        <Input
                            type="text"
                            name="sideUrl"
                            id="sideUrl"
                            // placeholder={sideUrl}
                            contentBefore="https://"
                            pattern=".*[.].*"></Input>
                    </Label>
                </div>

                <Button appearance="transparent" type="submit">
                    Save
                </Button>
            </form>
        </FluentProvider>
    );
}
