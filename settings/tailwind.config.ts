import type { Config } from "tailwindcss";

export default {
    content: ["./src/**/*.{html,js,jsx,ts,tsx}"],
    plugins: [require("@tailwindcss/typography")],
    // darkMode: ["class", '[data-theme="dark"]'],
    theme: {
        extend: {
            colors: {
                accent: "var(--accentColor)",
            },
        },
        // fontFamily: {
        //     sans: [
        //         "IBM Plex Sans",
        //         "Segoe UI Variable Text",
        //         "Segoe UI",
        //         "Arial",
        //         "Noto Emoji",
        //     ],
        //     serif: ["Constantia", "Georgia", "Times New Roman", "Noto Emoji"],
        //     mono: ["Cascadia Mono", "Consolas", "Courier New", "Noto Emoji"],
        // },
        fontFamily: {
            sans: ["Segoe UI Variable Text", "Segoe UI", "Arial", "Noto Emoji"],
            serif: ["Constantia", "Georgia", "Times New Roman", "Noto Emoji"],
            mono: ["Cascadia Mono", "Consolas", "Courier New", "Noto Emoji"],
        },
    },
    corePlugins: {
        preflight: true,
    },
} satisfies Config;
