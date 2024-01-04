// https://medium.com/expedia-group-tech/what-happens-when-i-type-something-in-the-browsers-address-bar-89a1243f0fff

export const tests = {
    error1: "https://asdasdasdasdasdfkjlwejjklsrghsjkghsjgnsegfnbsgk.com/",
    error2: "http://www.apple",
    http: "http://www.apple.com/",
    https: "https://www.apple.com/",
    www: "www.apple.com",
    com: "google.com",
    searchTerm: "apples",
    spaces: "   apples asdasd",
};
export const values = Object.values(tests);
export const searchEngine = "https://www.google.com/search?q=";

export const getTrimmed = (input: string) => {
    let trim = input.trim();

    if (trim.startsWith("http://") || trim.startsWith("https://")) {
        return trim;
    }
    if (trim.includes(".")) {
        return `https://${trim}`;
    } else {
        return `${trim}`;
    }
};

export const parseUrl = (input: string): URL => {
    let trim = input.trim();
    let url: URL;

    if (URL.canParse(trim)) {
        url = new URL(trim);
    } else {
        url = parseUrl(`${searchEngine}${encodeURIComponent(trim)}`);
    }

    return url;
};

export const getResponse = async (input: string) => {
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
