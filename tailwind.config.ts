import type { Config } from "tailwindcss";

export default {
    content: ["./gui/**/*.{html,js,jsx,ts,tsx}"],
    theme: {
        fontFamily: {
            sans: ["Segoe UI"],
            serif: ["Cambria"],
            mono: ["Cascadia Code"],
        },
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
    },
} satisfies Config;
