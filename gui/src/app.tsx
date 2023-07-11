export default function App() {
    function handleForm(e: any) {
        e.preventDefault();
        const data = new FormData(e.target);
        const mainUrl = Object.fromEntries(data.entries()).mainUrl;
        const sideUrl = Object.fromEntries(data.entries()).sideUrl;
        // console.log(main);
        // console.log(side);
        window.chrome.webview.postMessage(`mainUrl ${mainUrl}`);
        window.chrome.webview.postMessage(`sideUrl ${sideUrl}`);
    }

    return (
        <form autoComplete="off" id="settings" method="post" onSubmit={handleForm}>
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

            <button type="submit">💾 Save</button>
        </form>
    );
}
