import {
    makeStyles,
    FluentProvider,
    webLightTheme,
    webDarkTheme,
    Button,
    Input,
    Label,
} from "@fluentui/react-components";

const useStyles = makeStyles({
    provider: {
        backgroundColor: "transparent",
        height: "100%",
    },
});

export default function App() {
    function handleForm(e: any) {
        e.preventDefault();
        const data = new FormData(e.target);
        const mainUrl = Object.fromEntries(data.entries()).mainUrl;
        const sideUrl = Object.fromEntries(data.entries()).sideUrl;
        window.chrome.webview.postMessage(`mainUrl ${mainUrl}`);
        window.chrome.webview.postMessage(`sideUrl ${sideUrl}`);
    }

    const styles = useStyles();

    return (
        <FluentProvider className={styles.provider} theme={webDarkTheme}>
            <form
                autoComplete="off"
                id="settings"
                method="post"
                onSubmit={handleForm}>
                <div id="url">
                    <Label>
                        Test
                        <Input
                            type="url"
                            name="mainUrl"
                            placeholder="https://"
                            pattern="https://.*"
                            required></Input>
                    </Label>

                    <Label>
                        Test
                        <Input
                            type="url"
                            name="sideUrl"
                            placeholder="https://"
                            pattern="https://.*"
                            required></Input>
                    </Label>
                </div>

                <Button appearance="primary" type="submit">
                    Save
                </Button>
            </form>
        </FluentProvider>
    );
}
