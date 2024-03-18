export default (input: string) => {
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
