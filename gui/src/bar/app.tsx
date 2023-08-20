import { useEffect, useState } from "react";

export default function App() {
    const [mainCurrentPage, setMainCurrentPage] = useState("");
    const [sideCurrentPage, setSideCurrentPage] = useState("");
    const [webviewSplit, setWebviewSplit] = useState(false);
    const [webviewSwapped, setWebviewSwapped] = useState(false);

    if (window.chrome.webview) {
        window.chrome.webview.addEventListener("message", (arg: any) => {
            if (arg.data.colors) {
                document.documentElement.style.setProperty(
                    "--colorAccent",
                    arg.data.colors.colorAccent,
                );
                document.documentElement.style.setProperty(
                    "--colorAccentDark1",
                    arg.data.colors.colorAccentDark1,
                );
                document.documentElement.style.setProperty(
                    "--colorAccentDark2",
                    arg.data.colors.colorAccentDark2,
                );
                document.documentElement.style.setProperty(
                    "--colorAccentDark3",
                    arg.data.colors.colorAccentDark3,
                );
                document.documentElement.style.setProperty(
                    "--colorAccentLight1",
                    arg.data.colors.colorAccentLight1,
                );
                document.documentElement.style.setProperty(
                    "--colorAccentLight2",
                    arg.data.colors.colorAccentLight2,
                );
                document.documentElement.style.setProperty(
                    "--colorAccentLight3",
                    arg.data.colors.colorAccentLight3,
                );
            }
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

    return (
        <div className="flex h-full flex-col">
            <form
                className={
                    "flex flex-grow shadow-md shadow-neutral-400 dark:shadow-black " +
                    (webviewSwapped ? "flex-row-reverse" : "")
                }
                name="url"
                id="url"
                method="post"
                onSubmit={handleForm}
                autoComplete="off"
                spellCheck="false">
                <input
                    className={
                        "flex flex-1 bg-transparent px-4 text-center outline-none placeholder:text-left " +
                        (!webviewSplit && webviewSwapped ? "hidden" : "")
                    }
                    type="text"
                    name="mainCurrentPage"
                    id="mainCurrentPage"
                    placeholder="🔍︎"></input>

                <input
                    className={
                        "flex flex-1 bg-transparent px-4 text-center outline-none placeholder:text-left " +
                        (!webviewSplit && !webviewSwapped ? "hidden" : "")
                    }
                    type="text"
                    name="sideCurrentPage"
                    id="sideCurrentPage"
                    placeholder="🔍︎"></input>

                <input type="submit" hidden />
            </form>

            <div
                className={
                    "flex " + (webviewSwapped ? "flex-row-reverse" : "")
                }>
                <address
                    className={
                        "min-w-0 flex-1 flex-shrink truncate px-4 py-1 text-center text-sm not-italic text-accent dark:text-accentLight1 " +
                        (!webviewSplit && webviewSwapped ? "hidden" : "")
                    }>
                    {mainCurrentPage.includes("Airglow/gui") ||
                    mainCurrentPage === "https://localhost:8000/"
                        ? `\uFEFF`
                        : mainCurrentPage}
                    <a href="#">🛠️</a>
                </address>
                <address
                    className={
                        "min-w-0 flex-1 flex-shrink truncate px-4 py-1 text-center text-sm not-italic text-accent dark:text-accentLight1 " +
                        (!webviewSplit && !webviewSwapped ? "hidden" : "")
                    }>
                    {sideCurrentPage.includes("Airglow/gui") ||
                    sideCurrentPage === "https://localhost:8000/"
                        ? `\uFEFF`
                        : sideCurrentPage}
                </address>
            </div>
        </div>
    );
}
