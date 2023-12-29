import type { Config } from "tailwindcss";

export default {
    plugins: [require("@tailwindcss/typography")],
    content: ["./src/**/*.{html,js,jsx,ts,tsx}"],
} satisfies Config;
