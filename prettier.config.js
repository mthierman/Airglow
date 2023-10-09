/** @type {import('prettier').Config} */

module.exports = {
    tabWidth: 4,
    bracketSameLine: true,
    overrides: [
        {
            files: ["*.yml", "*.yaml", ".clang-*"],
            options: {
                tabWidth: 2,
            },
        },
    ],
};
