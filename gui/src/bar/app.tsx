import { useEffect, useState } from "react";

export default function App() {
    const [mainUrl, setMainUrl] = useState("");
    const [sideUrl, setSideUrl] = useState("");
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
                setMainUrl(arg.data.settings.mainUrl);
                setSideUrl(arg.data.settings.sideUrl);
                setSplit(arg.data.settings.split);
                setSwapped(arg.data.settings.swapped);
            }
        });
    }

    useEffect(() => {
        if (mainUrl && sideUrl)
            window.chrome.webview.postMessage({
                mainUrl: mainUrl,
                sideUrl: sideUrl,
            });
    }, [mainUrl, sideUrl]);

    const handleForm = (e: any) => {
        e.preventDefault();

        const form = document.getElementsByName("url")[0] as HTMLFormElement;

        const rawData = new FormData(e.target);
        const data = Object.fromEntries(rawData.entries());

        if (data.mainUrl.toString() != "") {
            setMainUrl(data.mainUrl.toString());
        }

        if (data.sideUrl.toString() != "") {
            setSideUrl(data.sideUrl.toString());
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
                        name="mainUrl"
                        id="mainUrl"
                        placeholder={mainUrl}
                        pattern=".*[.].*"></input>

                    <input
                        type="text"
                        name="sideUrl"
                        id="sideUrl"
                        placeholder={sideUrl}
                        pattern=".*[.].*"></input>

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
                        name="sideUrl"
                        id="sideUrl"
                        placeholder={sideUrl}
                        pattern=".*[.].*"></input>

                    <input
                        type="text"
                        name="mainUrl"
                        id="mainUrl"
                        placeholder={mainUrl}
                        pattern=".*[.].*"></input>

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
                        name="mainUrl"
                        id="mainUrl"
                        placeholder={mainUrl}
                        pattern=".*[.].*"></input>

                    <input
                        type="text"
                        name="sideUrl"
                        id="sideUrl"
                        placeholder={sideUrl}
                        pattern=".*[.].*"
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
                        name="mainUrl"
                        id="mainUrl"
                        placeholder={mainUrl}
                        pattern=".*[.].*"
                        hidden></input>

                    <input
                        type="text"
                        name="sideUrl"
                        id="sideUrl"
                        placeholder={sideUrl}
                        pattern=".*[.].*"></input>

                    <input type="submit" hidden />
                </form>
            </div>
        );
    }
}
