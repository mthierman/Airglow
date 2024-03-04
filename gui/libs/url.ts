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

const parse = (input: string): URL => {
    let url: URL;

    if (URL.canParse(input)) {
        url = new URL(input);
    } else {
        url = parse(`${searchEngine}${encodeURIComponent(input)}`);
    }

    return url;
};

export default (input: string) => {
    input = input.trim();

    if (input.length === 0) {
        return input;
    }

    if (input.startsWith("http://") || input.startsWith("https://")) {
        return parse(input).href;
    } else if (input.includes(".")) {
        return parse(`https://${input}`).href;
    } else {
        return parse(input).href;
    }
};
