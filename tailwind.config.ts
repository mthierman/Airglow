import type { Config } from "tailwindcss";

export default {
    plugins: [
        require("@tailwindcss/aspect-ratio"),
        require("@tailwindcss/container-queries"),
        require("@tailwindcss/forms"),
        require("@tailwindcss/typography"),
    ],
    content: ["./gui/**/*.{html,js,jsx,ts,tsx}"],
    theme: {
        fontFamily: {
            body: [
                "Roboto Flex",
                "system-ui",
                "ui-sans-serif",
                "sans-serif",
                "Noto Emoji",
                "emoji",
            ],
            slab: [
                "Roboto Slab",
                "system-ui",
                "ui-sans-serif",
                "sans-serif",
                "Noto Emoji",
                "emoji",
            ],
            mono: ["Cascadia Code PL", "ui-monospace", "monospace", "Noto Emoji", "emoji"],
        },
        extend: {
            colors: {
                accent: "#106BB2",
            },
        },
    },
} satisfies Config;
