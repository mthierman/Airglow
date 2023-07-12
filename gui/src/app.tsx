import {
    makeStyles,
    FluentProvider,
    webLightTheme,
    webDarkTheme,
    Button,
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
                    <label>
                        Main URL
                        <input
                            type="url"
                            name="mainUrl"
                            placeholder="https://"
                            pattern="https://.*"
                            required
                        />
                    </label>

                    <label>
                        Side URL
                        <input
                            type="url"
                            name="sideUrl"
                            placeholder="https://"
                            pattern="https://.*"
                            required
                        />
                    </label>
                </div>

                <Button appearance="secondary" type="submit">
                    💾 Save
                </Button>
            </form>
        </FluentProvider>
    );
}
