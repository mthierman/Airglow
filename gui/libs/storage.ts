export const getSessionStorage = (key: string, defaultValue: any) => {
    const value = sessionStorage.getItem(key);

    if (!value) {
        return defaultValue;
    } else {
        return value;
    }
};
