import { defineConfig, UserConfig } from "vite";
import path from "path";
import react from "@vitejs/plugin-react-swc";

const defaults: UserConfig = {
    plugins: [react()],
    base: "./",
    root: "gui",
    build: {
        outDir: "../build/gui",
        rollupOptions: {
            input: {
                index: path.resolve("gui/index.html"),
                settings: path.resolve("gui/settings.html"),
            },
        },
    },
};

const server: UserConfig["server"] = {
    port: 8000,
    https: {
        pfx: path.resolve("../.cert/localhost.pfx"),
        passphrase: "localhost",
    },
};

// https://vitejs.dev/config/
export default defineConfig(async ({ mode }) => {
    switch (mode) {
        case "development":
            return {
                ...defaults,
                server: { ...server },
                preview: { ...server },
            };
        default:
            return {
                ...defaults,
            };
    }
});
