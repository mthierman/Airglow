import { defineConfig, UserConfig } from "vite";
import path from "path";
import react from "@vitejs/plugin-react-swc";

const defaults: UserConfig = {
    plugins: [react()],
    base: "./",
    root: "gui/apps",
    build: {
        outDir: "../build/gui",
        rollupOptions: {
            input: {
                test: path.resolve("test/index.html"),
                home: path.resolve("home/index.html"),
                settings: path.resolve("settings/index.html"),
                addressbar: path.resolve("addressbar/index.html"),
            },
        },
    },
    resolve: {
        alias: {
            "@components": path.resolve("gui/components"),
            "@css": path.resolve("gui/css"),
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
