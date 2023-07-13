import type { Config } from "tailwindcss";

export default {
    content: ["./src/**/*.{html,js,jsx,ts,tsx}"],
    // darkMode: ["class", '[data-theme="dark"]'],
    theme: {
        extend: {
            colors: {
                accent: "var(--accentColor)",
            },
        },
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
