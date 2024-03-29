import iconBlankDark from "assets/blankDark.svg?raw";
import iconBlankLight from "assets/blankLight.svg?raw";
import iconDebug from "data/debug.svg?raw";
import iconRelease from "data/release.svg?raw";
import { AppColors, AppLayout, AppState, FaviconProvider, Page } from "libs/types";
import { useEffect, useState } from "react";

const useInitializer = () => {
    useEffect(() => {
        window.chrome.webview.postMessage({ initialized: true });
    }, []);
};

const appFavicon = () => {
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

const defaultFavicon = () => {
    if (window.matchMedia("(prefers-color-scheme: dark)").matches) {
        return `data:image/svg+xml,${encodeURIComponent(iconBlankDark)}`;
    } else {
        return `data:image/svg+xml,${encodeURIComponent(iconBlankLight)}`;
    }
};

const getFavicon = (input: string, provider: FaviconProvider) => {
    let favicon = "";

    if (URL.canParse(input)) {
        const url = new URL(input);

        switch (provider) {
            case FaviconProvider.Default:
                favicon = `${url.origin}/favicon.ico`;
                break;
            case FaviconProvider.Google:
                favicon = `https://www.google.com/s2/favicons?domain=${url.origin}&sz=128`;
                break;
            case FaviconProvider.DuckDuckGo:
                favicon = `https://icons.duckduckgo.com/ip3/${url.hostname}.ico`;
                break;
            case FaviconProvider.FaviconKit:
                favicon = `https://api.faviconkit.com/${url.hostname}/144`;
                break;
        }
    }

    return favicon;
};

const useColors = (colors: AppColors) => {
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

const useScale = () => {
    const [scale, setScale] = useState(parseFloat(devicePixelRatio.toFixed(2)));

    useEffect(() => {
        const onResize = () => {
            setScale(parseFloat(devicePixelRatio.toFixed(2)));
        };

        window.addEventListener("resize", onResize);
        return () => {
            window.removeEventListener("resize", onResize);
        };
    }, []);

    return scale;
};

const defaultState = (): AppState => {
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

const defaultLayout = (): AppLayout => {
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

const defaultPage = (): Page => {
    return {
        favicon: "",
        source: "",
        title: "",
    };
};

const response = async (input: string) => {
    try {
        const response = await fetch(input);

        if (!response.ok) {
            throw new Error("Network response was not OK");
        }

        return true;
    } catch (error) {
        return false;
    }
};

export {
    appFavicon,
    defaultFavicon,
    defaultLayout,
    defaultPage,
    defaultState,
    getFavicon,
    response,
    useColors,
    useInitializer,
    useScale,
};
