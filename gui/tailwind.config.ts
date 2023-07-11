import type { Config } from "tailwindcss";

export default {
    plugins: [require("@tailwindcss/typography")],
    content: ["./src/**/*.{html,js,jsx,ts,tsx}"],
    // darkMode: ["class", '[data-theme="dark"]'],
    theme: {
        extend: {},
        fontFamily: {
            sans: ["Segoe UI Variable Text", "Segoe UI", "Arial"],
            serif: ["Constantia", "Georgia", "Times New Roman"],
            mono: ["Cascadia Mono", "Consolas", "Courier New"],
        },
    },
    corePlugins: {
        preflight: true,
    },
} satisfies Config;
