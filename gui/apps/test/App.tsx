import "./test.css";

export default function App() {
    const handleForm = (e: any) => {
        e.preventDefault();

        const form = document.getElementsByName("url")[0] as HTMLFormElement;

        const rawData = new FormData(e.target);
        const data = Object.fromEntries(rawData.entries());

        if (data.mainUrl.toString() != "") {
            console.log(data.mainUrl);
            // window.chrome.webview.postMessage({
            //     mainCurrentPage: data.mainCurrentPage.toString().trim(),
            // });
        }

        if (data.sideUrl.toString() != "") {
            console.log(data.sideUrl);
            // window.chrome.webview.postMessage({
            //     sideCurrentPage: data.sideCurrentPage.toString().trim(),
            // });
        }

        form.reset();
    };

    return (
        <div>
            <form
                name="url"
                id="url"
                method="post"
                onSubmit={handleForm}
                autoComplete="off"
                spellCheck="false">
                <label>
                    <input type="text" name="mainUrl" id="mainUrl"></input>
                </label>

                <label>
                    <input type="text" name="sideUrl" id="sideUrl"></input>
                </label>

                <input type="submit" hidden />
            </form>
        </div>
    );
}
