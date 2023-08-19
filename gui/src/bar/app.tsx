import { useEffect, useState } from "react";

export default function App() {
    const [mainCurrentPage, setMainCurrentPage] = useState("");
    const [sideCurrentPage, setSideCurrentPage] = useState("");
    const [webviewSplit, setWebviewSplit] = useState(false);
    const [webviewSwapped, setWebviewSwapped] = useState(false);

    if (window.chrome.webview) {
        window.chrome.webview.addEventListener("message", (arg: any) => {
            if (arg.data.settings) {
                setMainCurrentPage(arg.data.settings.mainCurrentPage);
                setSideCurrentPage(arg.data.settings.sideCurrentPage);
                setWebviewSplit(arg.data.settings.webviewSplit);
                setWebviewSwapped(arg.data.settings.webviewSwapped);
            }
        });
    }

    const handleForm = (e: any) => {
        e.preventDefault();

        const form = document.getElementsByName("url")[0] as HTMLFormElement;

        const rawData = new FormData(e.target);
        const data = Object.fromEntries(rawData.entries());

        if (data.mainCurrentPage.toString() != "") {
            window.chrome.webview.postMessage({
                mainCurrentPage: data.mainCurrentPage.toString().trim(),
            });
        }

        if (data.sideCurrentPage.toString() != "") {
            window.chrome.webview.postMessage({
                sideCurrentPage: data.sideCurrentPage.toString().trim(),
            });
        }

        form.reset();
    };

    let addressBarStyle = "flex min-w-0 flex-shrink";

    if (webviewSwapped)
        addressBarStyle = "flex min-w-0 flex-shrink flex-row-reverse";

    let addressStyle =
        "h-min w-full truncate pb-1 pl-2 pr-8 pt-2 text-end text-sm shadow-lg";

    let mainAddressStyle = addressStyle;
    let sideAddressStyle = addressStyle;

    if (!webviewSplit && webviewSwapped) mainAddressStyle = "hidden";
    if (!webviewSplit && !webviewSwapped) sideAddressStyle = "hidden";

    let inputStyle =
        "flex flex-1 flex-col bg-transparent pl-4 shadow-lg outline-none";

    let mainInputStyle = inputStyle;
    let sideInputStyle = inputStyle;

    if (!webviewSplit && webviewSwapped) mainInputStyle = "hidden";
    if (!webviewSplit && !webviewSwapped) sideInputStyle = "hidden";

    return (
        <div className="flex h-full flex-col">
            <form
                className="flex min-w-0 flex-shrink flex-grow flex-row text-lg"
                name="url"
                id="url"
                method="post"
                onSubmit={handleForm}
                autoComplete="off"
                spellCheck="false">
                <input
                    className={mainInputStyle}
                    type="text"
                    name="mainCurrentPage"
                    id="mainCurrentPage"
                    placeholder="📡"></input>

                <input
                    className={sideInputStyle}
                    type="text"
                    name="sideCurrentPage"
                    id="sideCurrentPage"
                    placeholder="📡"></input>

                <input type="submit" hidden />
            </form>

            <div className={addressBarStyle}>
                <address className={mainAddressStyle}>
                    {mainCurrentPage}
                </address>
                <address className={sideAddressStyle}>
                    {sideCurrentPage}
                </address>
            </div>
        </div>
    );
}
