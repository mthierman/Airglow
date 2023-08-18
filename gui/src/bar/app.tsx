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
                mainCurrentPage: data.mainCurrentPage,
            });
        }

        if (data.sideCurrentPage.toString() != "") {
            window.chrome.webview.postMessage({
                sideCurrentPage: data.sideCurrentPage,
            });
        }

        form.reset();
    };

    let formStyle = "flex";
    let mainLabelStyle = "";
    let sideLabelStyle = "";

    if (webviewSwapped) formStyle = "flex flex-row-reverse";

    if (!webviewSplit && !webviewSwapped) {
        sideLabelStyle = "hidden";
    }
    if (!webviewSplit && webviewSwapped) {
        mainLabelStyle = "hidden";
    }

    return (
        <div>
            <form
                className={formStyle}
                name="url"
                id="url"
                method="post"
                onSubmit={handleForm}
                autoComplete="off"
                spellCheck="false">
                <label className={mainLabelStyle}>
                    <input
                        type="text"
                        name="mainCurrentPage"
                        id="mainCurrentPage"></input>
                    <address>{mainCurrentPage}</address>
                </label>

                <label className={sideLabelStyle}>
                    <input
                        type="text"
                        name="sideCurrentPage"
                        id="sideCurrentPage"></input>
                    <address>{sideCurrentPage}</address>
                </label>

                <input type="submit" hidden />
            </form>
        </div>
    );
}
