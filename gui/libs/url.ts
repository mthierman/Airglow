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

export const trimInput = (input: string) => {
    const trim = input.trim();

    if (trim.startsWith("http://") || trim.startsWith("https://")) {
        return trim;
    }
    if (trim.includes(".")) {
        return `https://${trim}`;
    } else {
        return `${trim}`;
    }
};

export const parseInput = (input: string): URL => {
    const trim = input.trim();
    let url: URL;

    if (URL.canParse(trim)) {
        url = new URL(trim);
    } else {
        url = parseInput(`${searchEngine}${encodeURIComponent(trim)}`);
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

export const parseUrl = (input: string) => {
    return parseInput(trimInput(input));
};
