export default function App() {
    function handleButton() {
        alert("TEST");
        window.chrome.webview.postMessage("TEST");
    }

    return (
        <div id="settings">
            <h1>Settings</h1>
            <div className="setting">
                <label>Main URL</label>
                <input type="url" name="mainUrl" defaultValue="google.com" />
            </div>
            <div className="setting">
                <label>Side URL</label>
                <input type="url" name="sideUrl" defaultValue="bing.com" />
            </div>
            <button onClick={handleButton} id="TestButton">
                Test
            </button>
        </div>
    );
}
