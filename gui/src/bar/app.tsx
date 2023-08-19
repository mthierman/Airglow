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

    let mainUrl = mainCurrentPage;
    let sideUrl = sideCurrentPage;

    if (
        mainCurrentPage.includes("Airglow/gui") ||
        mainCurrentPage === "https://localhost:8000/"
    )
        mainUrl = `\uFEFF`;

    if (
        sideCurrentPage.includes("Airglow/gui") ||
        sideCurrentPage === "https://localhost:8000/"
    )
        sideUrl = `\uFEFF`;

    let formStyle = `flex flex-grow`;
    if (webviewSwapped) formStyle = `flex flex-grow flex-row-reverse`;

    let inputStyle = `flex flex-1 outline-none text-center bg-transparent placeholder:text-left`;
    let mainInputStyle = inputStyle;
    let sideInputStyle = inputStyle;
    if (!webviewSplit && webviewSwapped) mainInputStyle = `hidden`;
    if (!webviewSplit && !webviewSwapped) sideInputStyle = `hidden`;

    let addresses = `flex`;
    if (webviewSwapped) addresses = `flex flex-row-reverse`;

    let addressStyle = `flex-1 flex-shrink min-w-0 truncate text-sm not-italic`;
    let mainAddress = addressStyle;
    let sideAddress = addressStyle;
    if (!webviewSplit && webviewSwapped) mainAddress = `hidden`;
    if (!webviewSplit && !webviewSwapped) sideAddress = `hidden`;

    return (
        <div className="flex h-full flex-col p-1">
            <form
                className={formStyle}
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
                    placeholder="🔍︎"></input>

                <input
                    className={sideInputStyle}
                    type="text"
                    name="sideCurrentPage"
                    id="sideCurrentPage"
                    placeholder="🔍︎"></input>

                <input type="submit" hidden />
            </form>

            <div className={addresses}>
                <address className={mainAddress}>{mainUrl}</address>
                <address className={sideAddress}>{sideUrl}</address>
            </div>
        </div>
    );
}
