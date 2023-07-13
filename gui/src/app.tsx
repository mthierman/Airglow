import {
    BrandVariants,
    Theme,
    FluentProvider,
    createLightTheme,
    createDarkTheme,
    webLightTheme,
    webDarkTheme,
    makeStyles,
    tokens,
    Button,
    Input,
    Label,
    Link,
} from "@fluentui/react-components";
import { useEffect, useState } from "react";

export const customTheme: Theme = {
    ...webDarkTheme,
    colorNeutralForeground2BrandPressed: "#FF0000",
    colorNeutralForeground3BrandPressed: "#FF0000",
    // colorNeutralForeground1: "#FF0000",
    // colorNeutralForeground2: "#FF0000",
    // colorBrandForeground1: "#FF0000",
    // colorBrandForeground2: "#FF0000",
    // colorBrandForegroundLinkPressed: "#FF0000",
    // colorBrandForegroundInvertedPressed: "#FF0000",
    // colorBrandForeground2Pressed: "#FF0000",
};

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
                const response = await fetch(`https://airglow/Config.json`);
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
        <FluentProvider className={styles.provider} theme={customTheme}>
            <form
                autoComplete="on"
                id="settings"
                method="post"
                onSubmit={handleForm}>
                <div id="url">
                    <Label>
                        <h1>
                            <span className="select-none">🏠 https://</span>
                            <Link appearance="subtle" href="#">
                                {mainUrl}
                            </Link>
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
                            <Link appearance="subtle" href="#">
                                {sideUrl}
                            </Link>
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

                <Button appearance="primary" type="submit">
                    Save
                </Button>
            </form>
        </FluentProvider>
    );
}
