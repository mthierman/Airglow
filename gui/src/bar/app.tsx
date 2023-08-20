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

    const handleMainDevtools = () => {
        window.chrome.webview.postMessage({
            mainDevtools: true,
        });
    };

    const handleSideDevtools = () => {
        window.chrome.webview.postMessage({
            sideDevtools: true,
        });
    };

    const mainBlank = () => {
        return mainCurrentPage.includes("Airglow/gui") ||
            mainCurrentPage === "https://localhost:8000/"
            ? true
            : false;
    };

    const sideBlank = () => {
        return sideCurrentPage.includes("Airglow/gui") ||
            sideCurrentPage === "https://localhost:8000/"
            ? true
            : false;
    };

    return (
        <div className="flex h-full flex-col overflow-clip">
            <form
                className={
                    "flex flex-grow p-1 font-semibold shadow-lg shadow-neutral-300 dark:shadow-neutral-950 " +
                    (webviewSwapped ? "flex-row-reverse" : "")
                }
                name="url"
                id="url"
                method="post"
                onSubmit={handleForm}
                autoComplete="off"
                spellCheck="false">
                <label
                    className={
                        "flex min-w-0 flex-1 select-none text-accentDark2 dark:text-accent " +
                        (!webviewSplit && webviewSwapped ? "hidden" : "")
                    }>
                    🔍︎
                    <input
                        className="flex min-w-0 flex-1 bg-transparent px-2 text-center text-accentDark2 outline-none placeholder:text-left dark:text-accent"
                        type="text"
                        name="mainCurrentPage"
                        id="mainCurrentPage"></input>
                </label>

                <label
                    className={
                        "flex min-w-0 flex-1 select-none text-accentDark2 dark:text-accent " +
                        (!webviewSplit && !webviewSwapped ? "hidden" : "")
                    }>
                    🔍︎
                    <input
                        className="flex min-w-0 flex-1 bg-transparent px-2 text-center text-accentDark2 outline-none placeholder:text-left dark:text-accent"
                        type="text"
                        name="sideCurrentPage"
                        id="sideCurrentPage"></input>
                </label>

                <input type="submit" hidden />
            </form>

            <div
                className={
                    "flex p-2 text-sm text-neutral-500 dark:text-neutral-400 " +
                    (webviewSwapped ? "flex-row-reverse" : "")
                }>
                <address
                    className={
                        "flex min-w-0 flex-1 flex-shrink items-center not-italic " +
                        (!webviewSplit && webviewSwapped ? "hidden" : "")
                    }>
                    <span className="flex-grow truncate px-4 text-center">
                        <a
                            className="select-none pr-2 hover:cursor-pointer"
                            onClick={handleMainDevtools}>
                            {mainBlank() ? `` : `🔧︎`}
                        </a>
                        {mainBlank() ? `` : mainCurrentPage}
                    </span>
                </address>
                <address
                    className={
                        "flex min-w-0 flex-1 flex-shrink items-center not-italic " +
                        (!webviewSplit && !webviewSwapped ? "hidden" : "")
                    }>
                    <span className="flex-grow truncate px-4 text-center">
                        <a
                            className="select-none pr-2 hover:cursor-pointer"
                            onClick={handleSideDevtools}>
                            {sideBlank() ? `` : `🔧︎`}
                        </a>
                        {sideBlank() ? `` : sideCurrentPage}
                    </span>
                </address>
            </div>
        </div>
    );
}
