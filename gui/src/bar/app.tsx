import { useEffect, useState } from "react";

export default function App() {
    // const [homepageMain, setHomepageMain] = useState("");
    // const [homepageSide, setHomepageSide] = useState("");
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
                // setHomepageMain(arg.data.settings.homepageMain);
                // setHomepageSide(arg.data.settings.homepageSide);
                setCurrentPageMain(arg.data.settings.currentPageMain);
                setCurrentPageSide(arg.data.settings.currentPageSide);
                setSplit(arg.data.settings.split);
                setSwapped(arg.data.settings.swapped);
            }
        });
    }

    useEffect(() => {
        window.chrome.webview.postMessage({
            currentPageMain: currentPageMain,
        });
    }, [currentPageMain]);

    useEffect(() => {
        window.chrome.webview.postMessage({
            currentPageSide: currentPageSide,
        });
    }, [currentPageSide]);

    const handleForm = (e: any) => {
        e.preventDefault();

        const form = document.getElementsByName("url")[0] as HTMLFormElement;

        const rawData = new FormData(e.target);
        const data = Object.fromEntries(rawData.entries());

        if (data.currentPageMain.toString() != "") {
            setCurrentPageMain(data.currentPageMain.toString());
        }

        if (data.currentPageSide.toString() != "") {
            setCurrentPageSide(data.currentPageSide.toString());
        }

        form.reset();
    };

    if (split && !swapped) {
        return (
            <div>
                <form
                    name="url"
                    id="url"
                    method="post"
                    onSubmit={handleForm}
                    autoComplete="off"
                    spellCheck="false">
                    <input
                        type="text"
                        name="currentPageMain"
                        id="currentPageMain"
                        placeholder={currentPageMain}></input>

                    <input
                        type="text"
                        name="currentPageSide"
                        id="currentPageSide"
                        placeholder={currentPageSide}></input>

                    <input type="submit" hidden />
                </form>
            </div>
        );
    }

    if (split && swapped) {
        return (
            <div>
                <form
                    name="url"
                    id="url"
                    method="post"
                    onSubmit={handleForm}
                    autoComplete="off"
                    spellCheck="false">
                    <input
                        type="text"
                        name="currentPageSide"
                        id="currentPageSide"
                        placeholder={currentPageSide}></input>

                    <input
                        type="text"
                        name="currentPageMain"
                        id="currentPageMain"
                        placeholder={currentPageMain}></input>

                    <input type="submit" hidden />
                </form>
            </div>
        );
    }

    if (!split && !swapped) {
        return (
            <div>
                <form
                    name="url"
                    id="url"
                    method="post"
                    onSubmit={handleForm}
                    autoComplete="off"
                    spellCheck="false">
                    <input
                        type="text"
                        name="currentPageMain"
                        id="currentPageMain"
                        placeholder={currentPageMain}></input>

                    <input
                        type="text"
                        name="currentPageSide"
                        id="currentPageSide"
                        placeholder={currentPageSide}
                        hidden></input>

                    <input type="submit" hidden />
                </form>
            </div>
        );
    }

    if (!split && swapped) {
        return (
            <div>
                <form
                    name="url"
                    id="url"
                    method="post"
                    onSubmit={handleForm}
                    autoComplete="off"
                    spellCheck="false">
                    <input
                        type="text"
                        name="currentPageSide"
                        id="currentPageSide"
                        placeholder={currentPageSide}></input>

                    <input
                        type="text"
                        name="currentPageMain"
                        id="currentPageMain"
                        placeholder={currentPageMain}
                        hidden></input>

                    <input type="submit" hidden />
                </form>
            </div>
        );
    }
}
