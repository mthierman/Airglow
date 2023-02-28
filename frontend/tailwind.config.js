/** @type {import('tailwindcss').Config} */
module.exports = {
    content: ["./src/**/*.{html,js,jsx,ts,tsx}"],
    theme: {
        extend: {},
    },
    corePlugins: {
        preflight: true,
    },
    plugins: [require("@tailwindcss/typography")],
};
