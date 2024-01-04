import * as url from "@libs/url";

export default function App() {
    const handleMain = (e: any) => {
        e.preventDefault();

        const form = document.getElementById("mainForm") as HTMLFormElement;

        const rawData = new FormData(e.target);
        const data = Object.fromEntries(rawData.entries());

        console.log(data.mainUrl);

        form.reset();
    };

    const handleSide = (e: any) => {
        // e.preventDefault();
        // const form = document.getElementsByName("sideForm")[0] as HTMLFormElement;
        // const rawData = new FormData(e.target);
        // const data = Object.fromEntries(rawData.entries());
        // if (data.sideUrl.toString() != "") {
        //     console.log(data.sideUrl);
        //     window.chrome.webview.postMessage({
        //         sideCurrentPage: data.sideCurrentPage.toString().trim(),
        //     });
        // }
        // form.reset();
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
                <input
                    className="w-full rounded-lg bg-flexoki-base-paper p-2 shadow-md shadow-flexoki-base-50 outline-none dark:bg-flexoki-base-950 dark:shadow-flexoki-base-black"
                    type="text"
                    name="mainUrl"
                    id="mainUrl"></input>
            </form>

            <form
                className="flex-grow"
                name="sideForm"
                id="sideForm"
                method="post"
                onSubmit={handleSide}
                autoComplete="off"
                spellCheck="false">
                <input
                    className="w-full rounded-lg bg-flexoki-base-paper p-2 shadow-md shadow-flexoki-base-50 outline-none dark:bg-flexoki-base-950 dark:shadow-flexoki-base-black"
                    type="text"
                    name="sideUrl"
                    id="sideUrl"></input>
            </form>
        </div>
    );
}
