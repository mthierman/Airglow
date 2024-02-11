import blankDark from "@assets/blankDark.svg?raw";
import blankLight from "@assets/blankLight.svg?raw";
// import debug from "@assets/debug.svg?raw";
import release from "@assets/release.svg?raw";

export const initialize = () => {
    window.chrome.webview.postMessage({ initialized: true });
};

export const applyColors = (colors: App.Colors) => {
    document.documentElement.style.setProperty("--accent", colors.accent);
    document.documentElement.style.setProperty("--accentDark1", colors.accentDark1);
    document.documentElement.style.setProperty("--accentDark2", colors.accentDark2);
    document.documentElement.style.setProperty("--accentDark3", colors.accentDark3);
    document.documentElement.style.setProperty("--accentLight1", colors.accentLight1);
    document.documentElement.style.setProperty("--accentLight2", colors.accentLight2);
    document.documentElement.style.setProperty("--accentLight3", colors.accentLight3);
};

export const applyFavicon = () => {
    document.onreadystatechange = () => {
        if (document.readyState === "complete") {
            const favicon = document.createElement("link");
            favicon.type = "image/svg+xml";
            favicon.rel = "icon";
            favicon.href = `data:image/svg+xml,${encodeURIComponent(release)}`;
            document.head.appendChild(favicon);
        }
    };
};

export const defaultFavicon = () => {
    if (window.matchMedia("(prefers-color-scheme: dark)").matches) {
        return `data:image/svg+xml,${encodeURIComponent(blankDark)}`;
    } else {
        return `data:image/svg+xml,${encodeURIComponent(blankLight)}`;
    }
};

export const getState = () => {
    const state = sessionStorage.getItem("state");

    if (state) {
        return JSON.parse(state) as App.State;
    } else
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
        } as App.State;
};

export const getLayout = () => {
    const layout = sessionStorage.getItem("layout");

    if (layout) {
        return JSON.parse(layout) as App.Layout;
    } else {
        return {
            bar: 0,
            border: 0,
            focus: "first",
            init: false,
            split: false,
            swap: false,
            vertical: true,
        } as App.Layout;
    }
};
