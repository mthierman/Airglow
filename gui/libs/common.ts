import iconBlankDark from "assets/blankDark.svg?raw";
import iconBlankLight from "assets/blankLight.svg?raw";
import iconDebug from "data/debug.svg?raw";
import iconRelease from "data/release.svg?raw";
import { Colors, Layout, Page, State } from "libs/types";
import { useEffect, useState } from "react";

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

export enum FaviconProvider {
    Default,
    Google,
    DuckDuckGo,
    FaviconKit,
}

export const getFavicon = (input: string, provider: FaviconProvider) => {
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

export const useColors = (colors: Colors) => {
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

export const useScale = () => {
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

export const defaultState = (): State => {
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

export const defaultLayout = (): Layout => {
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

export const defaultPage = (): Page => {
    return {
        favicon: "",
        source: "",
        title: "",
    };
};

export const url = (input: string) => {
    input = input.trim();

    if (input.length === 0) {
        return null;
    } else if (input.startsWith("http")) {
        if (URL.canParse(input)) {
            return new URL(input).href;
        }
    } else if (input.includes(".")) {
        if (URL.canParse(`https://${input}`)) {
            return new URL(`https://${input}`).href;
        }
    } else if (URL.canParse("https://www.google.com/")) {
        return new URL(`https://www.google.com/search?q=${encodeURIComponent(input)}`).href;
    } else return null;
};

export const response = async (input: string) => {
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
