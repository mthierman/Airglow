import { useState } from "react";

export default function App() {
    const [colors, setColors] = useState({
        accent: "",
        accentDark1: "",
        accentDark2: "",
        accentDark3: "",
        accentLight1: "",
        accentLight2: "",
        accentLight3: "",
    });
    const [mainCurrentPage, setMainCurrentPage] = useState("");
    const [sideCurrentPage, setSideCurrentPage] = useState("");
    const [webviewSplit, setWebviewSplit] = useState(false);
    const [webviewSwapped, setWebviewSwapped] = useState(false);

    if (window.chrome.webview) {
        window.chrome.webview.addEventListener("message", (arg: any) => {
            if (arg.data.colors) {
                setColors(arg.data.colors);

                document.documentElement.style.setProperty(
                    "--accent",
                    colors.accent,
                );

                document.documentElement.style.setProperty(
                    "--accentDark1",
                    colors.accentDark1,
                );

                document.documentElement.style.setProperty(
                    "--accentDark2",
                    colors.accentDark2,
                );

                document.documentElement.style.setProperty(
                    "--accentDark3",
                    colors.accentDark3,
                );

                document.documentElement.style.setProperty(
                    "--accentLight1",
                    colors.accentLight1,
                );

                document.documentElement.style.setProperty(
                    "--accentLight2",
                    colors.accentLight2,
                );

                document.documentElement.style.setProperty(
                    "--accentLight3",
                    colors.accentLight3,
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

    const handleDevtools = (webview: string) => {
        switch (webview) {
            case "main":
                window.chrome.webview.postMessage({
                    mainDevtools: true,
                });
            case "side":
                window.chrome.webview.postMessage({
                    sideDevtools: true,
                });
        }
    };

    const handleClipboard = async (page: string) => {
        await navigator.clipboard.writeText(page);
    };

    const isBlank = (page: string) => {
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
                        "flex min-w-0 flex-1 select-none px-1 " +
                        (!webviewSplit && webviewSwapped ? "hidden" : "")
                    }>
                    🔍︎
                    <input
                        className="flex min-w-0 flex-1 bg-transparent px-2 text-center text-accentDark2 outline-none dark:text-accent"
                        type="text"
                        name="mainCurrentPage"
                        id="mainCurrentPage"></input>
                </label>

                <label
                    className={
                        "flex min-w-0 flex-1 select-none px-1 " +
                        (!webviewSplit && !webviewSwapped ? "hidden" : "")
                    }>
                    🔍︎
                    <input
                        className="flex min-w-0 flex-1 bg-transparent px-2 text-center text-accentDark2 outline-none dark:text-accent"
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
                            onClick={() => {
                                handleDevtools("main");
                            }}>
                            {isBlank(mainCurrentPage) ? `` : `🔧︎`}
                        </a>
                        {isBlank(mainCurrentPage) ? (
                            ``
                        ) : (
                            <a
                                className="hover:cursor-copy"
                                onClick={() => {
                                    handleClipboard(mainCurrentPage);
                                }}>
                                {mainCurrentPage}
                            </a>
                        )}
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
                            onClick={() => {
                                handleDevtools("side");
                            }}>
                            {isBlank(sideCurrentPage) ? `` : `🔧︎`}
                        </a>
                        {isBlank(sideCurrentPage) ? (
                            ``
                        ) : (
                            <a
                                className="hover:cursor-copy"
                                onClick={() => {
                                    handleClipboard(sideCurrentPage);
                                }}>
                                {sideCurrentPage}
                            </a>
                        )}
                    </span>
                </address>
            </div>
        </div>
    );
}
