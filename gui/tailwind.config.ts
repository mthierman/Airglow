import type { Config } from "tailwindcss";

export default {
    content: ["./src/**/*.{html,js,jsx,ts,tsx}"],
    plugins: [require("@tailwindcss/typography")],
    theme: {
        extend: {
            colors: {
                accent: "var(--accent)",
                accentDark1: "var(--accentDark1)",
                accentDark2: "var(--accentDark2)",
                accentDark3: "var(--accentDark3)",
                accentLight1: "var(--accentLight1)",
                accentLight2: "var(--accentLight2)",
                accentLight3: "var(--accentLight3)",
            },
        },
        fontFamily: {
            sans: ["Segoe UI Variable Text", "Segoe UI", "Noto Emoji"],
            serif: ["Cambria", "Noto Emoji"],
            mono: ["Cascadia Code", "Noto Emoji"],
        },
    },
    corePlugins: {
        preflight: true,
    },
} satisfies Config;
