import type { Config } from "tailwindcss";
import Flexoki from "./flexoki";

export default {
    plugins: [require("@tailwindcss/forms"), require("@tailwindcss/typography")],
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
                ...Flexoki,
            },
        },
    },
} satisfies Config;
