import type { Config } from "tailwindcss";

export default {
    content: ["./src/**/*.{html,js,jsx,ts,tsx}"],
    plugins: [require("@tailwindcss/typography")],
    theme: {
        extend: {
            colors: {
                accent: "var(--colorAccent)",
                accentDark1: "var(--colorAccentDark1)",
                accentDark2: "var(--colorAccentDark2)",
                accentDark3: "var(--colorAccentDark3)",
                accentLight1: "var(--colorAccentLight1)",
                accentLight2: "var(--colorAccentLight2)",
                accentLight3: "var(--colorAccentLight3)",
            },
        },
        fontFamily: {
            sans: ["Segoe UI Variable Display"],
        },
    },
    corePlugins: {
        preflight: true,
    },
} satisfies Config;
