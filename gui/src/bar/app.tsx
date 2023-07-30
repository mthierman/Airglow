import { useEffect, useState } from "react";

export default function App() {
    const [currentPageMain, setCurrentPageMain] = useState("");
    const [currentPageSide, setCurrentPageSide] = useState("");
    const [split, setSplit] = useState(false);
    const [swapped, setSwapped] = useState(false);

    if (window.chrome.webview) {
        window.chrome.webview.addEventListener("message", (arg: any) => {
            if (arg.data.colors) {
                document.documentElement.style.setProperty(
                    "--accent",
                    arg.data.colors.accent,
                );
                document.documentElement.style.setProperty(
                    "--accentDark1",
                    arg.data.colors.accentDark1,
                );
                document.documentElement.style.setProperty(
                    "--accentDark2",
                    arg.data.colors.accentDark2,
                );
                document.documentElement.style.setProperty(
                    "--accentDark3",
                    arg.data.colors.accentDark2,
                );
                document.documentElement.style.setProperty(
                    "--accentLight1",
                    arg.data.colors.accentLight1,
                );
                document.documentElement.style.setProperty(
                    "--accentLight2",
                    arg.data.colors.accentLight2,
                );
                document.documentElement.style.setProperty(
                    "--accentLight3",
                    arg.data.colors.accentLight3,
                );
            }
            if (arg.data.settings) {
                setCurrentPageMain(arg.data.settings.currentPageMain);
                setCurrentPageSide(arg.data.settings.currentPageSide);
                setSplit(arg.data.settings.split);
                setSwapped(arg.data.settings.swapped);
            }
        });
    }

    const handleForm = (e: any) => {
        e.preventDefault();

        const form = document.getElementsByName("url")[0] as HTMLFormElement;

        const rawData = new FormData(e.target);
        const data = Object.fromEntries(rawData.entries());

        if (data.currentPageMain.toString() != "") {
            window.chrome.webview.postMessage({
                currentPageMain: data.currentPageMain,
            });
        }

        if (data.currentPageSide.toString() != "") {
            window.chrome.webview.postMessage({
                currentPageSide: data.currentPageSide,
            });
        }

        form.reset();
    };

    let formStyle = "flex";
    let mainLabelStyle = "";
    let sideLabelStyle = "";

    if (swapped) formStyle = "flex flex-row-reverse";

    if (!split && !swapped) {
        sideLabelStyle = "hidden";
    }
    if (!split && swapped) {
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
                        name="currentPageMain"
                        id="currentPageMain"
                        placeholder="🔭"></input>
                    <address>{currentPageMain}</address>
                </label>

                <label className={sideLabelStyle}>
                    <input
                        type="text"
                        name="currentPageSide"
                        id="currentPageSide"
                        placeholder="🔭"></input>
                    <address>{currentPageSide}</address>
                </label>

                <input type="submit" hidden />
            </form>
        </div>
    );
}
