import blankLight from "@assets/blankLight.svg?raw";
import blankDark from "@assets/blankDark.svg?raw";

export const getSessionStorage = (key: string, defaultValue: any) => {
    const value = sessionStorage.getItem(key);

    if (!value) {
        return defaultValue;
    } else {
        return value;
    }
};

export const getLayoutStorage = () => {
    const value: App.Layout = JSON.parse(sessionStorage.getItem("layout")!);

    const defaultValue: App.Layout = {
        bar: 0,
        border: 0,
        horizontal: false,
        split: false,
        swapped: false,
    };

    if (!value) {
        return defaultValue;
    } else {
        return value;
    }
};

export const getColorStorage = () => {
    const value: App.Colors = JSON.parse(sessionStorage.getItem("colors")!);

    const defaultValue: App.Colors = {
        accent: "",
        accentDark1: "",
        accentDark2: "",
        accentDark3: "",
        accentLight1: "",
        accentLight2: "",
        accentLight3: "",
    };

    if (!value) {
        return defaultValue;
    } else {
        return value;
    }
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

export const initialize = () => {
    window.chrome.webview.postMessage({ initialized: true });
};

export const defaultFavicon = () => {
    if (window.matchMedia("(prefers-color-scheme: dark)").matches) {
        return `data:image/svg+xml,${encodeURIComponent(blankDark)}`;
    } else {
        return `data:image/svg+xml,${encodeURIComponent(blankLight)}`;
    }
};
