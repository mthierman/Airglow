import * as url from "@libs/url";

export default function App() {
    const handleMain = (e: any) => {
        e.preventDefault();

        const form = document.getElementsByName("mainForm")[0] as HTMLFormElement;

        const rawData = new FormData(e.target);
        const data = Object.fromEntries(rawData.entries());

        console.log(data.mainUrl);

        if (data.mainUrl != "") {
            console.log(data.mainUrl);
            // window.chrome.webview.postMessage({
            //     mainCurrentPage: data.mainCurrentPage.toString().trim(),
            // });
        }

        form.reset();
    };

    const handleSide = (e: any) => {
        e.preventDefault();

        const form = document.getElementsByName("sideForm")[0] as HTMLFormElement;

        const rawData = new FormData(e.target);
        const data = Object.fromEntries(rawData.entries());

        if (data.sideUrl.toString() != "") {
            console.log(data.sideUrl);
            // window.chrome.webview.postMessage({
            //     sideCurrentPage: data.sideCurrentPage.toString().trim(),
            // });
        }

        form.reset();
    };

    return (
        <div className="flex gap-2 bg-transparent p-2">
            <form
                className="flex-grow"
                name="mainForm"
                id="mainForm"
                method="post"
                onSubmit={handleMain}
                autoComplete="off"
                spellCheck="false">
                <label className="flex flex-grow">
                    <input
                        className="flex-grow rounded-lg bg-flexoki-base-paper p-2 shadow-md shadow-flexoki-base-50 outline-none dark:bg-flexoki-base-950 dark:shadow-flexoki-base-black"
                        type="text"
                        name="mainUrl"
                        id="mainUrl"></input>
                </label>
                <input type="submit" hidden />
            </form>

            <form
                className="flex-grow"
                name="sideForm"
                id="sideForm"
                method="post"
                onSubmit={handleSide}
                autoComplete="off"
                spellCheck="false">
                <label className="flex flex-grow">
                    <input
                        className="w-full rounded-lg bg-flexoki-base-paper p-2 shadow-md shadow-flexoki-base-50 outline-none dark:bg-flexoki-base-950 dark:shadow-flexoki-base-black"
                        type="text"
                        name="sideUrl"
                        id="sideUrl"></input>
                </label>
                <input type="submit" hidden />
            </form>
        </div>
    );
}
