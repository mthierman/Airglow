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
    let mainUrl;
    let sideUrl;

    // window.chrome.webview.addEventListener("message", (arg: any) => {
    //     if (arg.data.includes("mainUrl")) {
    //         mainUrl = arg.data.slice(8);
    //     }
    //     if (arg.data.includes("sideUrl")) {
    //         sideUrl = arg.data.slice(8);
    //     }
    // });

    const styles = useStyles();
    function handleForm(e: any) {
        e.preventDefault();
        const data = new FormData(e.target);
        mainUrl = Object.fromEntries(data.entries()).mainUrl;
        sideUrl = Object.fromEntries(data.entries()).sideUrl;
        window.chrome.webview.postMessage(`mainUrl ${mainUrl}`);
        window.chrome.webview.postMessage(`sideUrl ${sideUrl}`);
    }

    const [dataMainUrl, setMainUrl] = useState("");
    const [dataSideUrl, setSideUrl] = useState("");

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
                            <span className="select-none">🏠 </span>
                            {dataMainUrl}
                        </h1>
                        <Input
                            type="url"
                            name="mainUrl"
                            placeholder={dataMainUrl}
                            pattern="https://.*"
                            required></Input>
                    </Label>

                    <Label>
                        <h1>
                            <span className="select-none">🔧 </span>
                            {dataSideUrl}
                        </h1>
                        <Input
                            type="url"
                            name="sideUrl"
                            placeholder={dataSideUrl}
                            pattern="https://.*"
                            required></Input>
                    </Label>
                </div>

                <Button appearance="transparent" type="submit">
                    Save
                </Button>
            </form>
        </FluentProvider>
    );
}
