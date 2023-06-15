/** @type {import('prettier').Config} */
module.exports = {
    tabWidth: 4,
    bracketSameLine: true,
    htmlWhitespaceSensitivity: "ignore",
    plugins: [require("prettier-plugin-tailwindcss")],
};
