/** @type {import('prettier').Config} */

module.exports = {
    tabWidth: 4,
    bracketSameLine: true,
    quoteProps: "consistent",
    overrides: [
        {
            files: ["*.yml", "*.yaml"],
            options: {
                tabWidth: 2,
            },
        },
    ],
    plugins: ["prettier-plugin-tailwindcss"],
};
