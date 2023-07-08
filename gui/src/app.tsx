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
        <div>
            <form id="settings" method="post" onSubmit={handleForm}>
                <h1>Settings</h1>
                <div className="setting">
                    <label>Main URL</label>
                    <input
                        type="url"
                        name="mainUrl"
                        placeholder="https://"
                        pattern="https://.*"
                        required
                    />
                </div>

                <div className="setting">
                    <label>Side URL</label>
                    <input
                        type="url"
                        name="sideUrl"
                        placeholder="https://"
                        pattern="https://.*"
                        required
                    />
                </div>
                <button
                    className="rounded border-2 border-neutral-400 p-1 hover:border-blue-400"
                    type="submit">
                    💾 Save
                </button>
            </form>
        </div>
    );
}
