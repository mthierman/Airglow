import iconBlankDark from "#assets/blankDark.svg?raw";
import iconBlankLight from "#assets/blankLight.svg?raw";
import iconDebug from "#data/debug.svg?raw";
import iconRelease from "#data/release.svg?raw";
import { useEffect } from "react";

export const useInitializer = () => {
    useEffect(() => {
        window.chrome.webview.postMessage({ initialized: true });
    }, []);
};

export const appFavicon = () => {
    document.onreadystatechange = () => {
        if (document.readyState === "complete") {
            const favicon = document.createElement("link");
            favicon.type = "image/svg+xml";
            favicon.rel = "icon";
            favicon.href = `data:image/svg+xml,${encodeURIComponent(import.meta.env.DEV ? iconDebug : iconRelease)}`;
            document.head.appendChild(favicon);
        }
    };
};

export const defaultFavicon = () => {
    if (window.matchMedia("(prefers-color-scheme: dark)").matches) {
        return `data:image/svg+xml,${encodeURIComponent(iconBlankDark)}`;
    } else {
        return `data:image/svg+xml,${encodeURIComponent(iconBlankLight)}`;
    }
};

export const useColors = (colors: App.Colors) => {
    useEffect(() => {
        document.documentElement.style.setProperty("--accent", colors.accent);
        document.documentElement.style.setProperty("--accentDark1", colors.accentDark1);
        document.documentElement.style.setProperty("--accentDark2", colors.accentDark2);
        document.documentElement.style.setProperty("--accentDark3", colors.accentDark3);
        document.documentElement.style.setProperty("--accentLight1", colors.accentLight1);
        document.documentElement.style.setProperty("--accentLight2", colors.accentLight2);
        document.documentElement.style.setProperty("--accentLight3", colors.accentLight3);
    }, [colors]);
};

export const defaultState = (): App.State => {
    return {
        args: ["", ""],
        colors: {
            accent: "",
            accentDark1: "",
            accentDark2: "",
            accentDark3: "",
            accentLight1: "",
            accentLight2: "",
            accentLight3: "",
        },
        home: ["", ""],
    };
};

export const defaultLayout = (): App.Layout => {
    return {
        bar: 0,
        border: 0,
        focus: "first",
        init: false,
        split: false,
        swap: false,
        vertical: true,
    };
};

export const defaultPage = (): App.Page => {
    return {
        favicon: "",
        source: "",
        title: "",
    };
};
