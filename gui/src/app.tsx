import { useEffect, useState } from "react";
import iconRaw from "../../data/icons/icon.svg?raw";
import bannerRaw from "../../data/banners/icon_with_name.svg?raw";

document.onreadystatechange = () => {
    if (document.readyState === "complete") {
        const favicon = document.createElement("link");
        favicon.type = "image/svg+xml";
        favicon.rel = "icon";
        favicon.href = `data:image/svg+xml,${encodeURIComponent(iconRaw)}`;
        document.head.appendChild(favicon);
    }
};

export default function App() {
    const [homepageMain, setHomepageMain] = useState("");
    const [homepageSide, setHomepageSide] = useState("");
    const [theme, setTheme] = useState("");
    const [themeIcon, setThemeIcon] = useState("");

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
                setHomepageMain(arg.data.settings.homepageMain);
                setHomepageSide(arg.data.settings.homepageSide);
                setTheme(arg.data.settings.theme);
            }
        });
    }

    useEffect(() => {
        if (theme) {
            if (theme === "dark") {
                setThemeIcon("🌙");
            }
            if (theme === "light") {
                setThemeIcon("🔆");
            }
        }
    }, [theme]);

    return (
        <div className="pointer-events-none grid select-none">
            <img
                src={`data:image/svg+xml,${encodeURIComponent(bannerRaw)}`}
                className="max-w-screen max-h-screen select-none place-self-center"
            />
        </div>
    );
}
