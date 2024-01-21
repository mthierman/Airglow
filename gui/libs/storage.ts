export const getSessionStorage = (key: string, defaultValue: any) => {
    const value = sessionStorage.getItem(key);

    if (!value) {
        return defaultValue;
    } else {
        return value;
    }
};

export const getLayoutStorage = () => {
    const value: App.Layout = JSON.parse(sessionStorage.getItem("position")!);

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

export const getSystemColorsStorage = () => {
    const value: App.SystemColors = JSON.parse(sessionStorage.getItem("systemColors")!);

    const defaultValue: App.SystemColors = {
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
