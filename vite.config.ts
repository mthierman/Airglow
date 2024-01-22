import { defineConfig, UserConfig } from "vite";
import path from "path";
import react from "@vitejs/plugin-react-swc";

const defaults: UserConfig = {
    plugins: [react()],
    base: "./",
    root: "gui/apps",
    build: {
        emptyOutDir: true,
        outDir: "../../build/Release/gui",
        rollupOptions: {
            input: {
                home: path.resolve("gui/apps/home/index.html"),
                settings: path.resolve("gui/apps/settings/index.html"),
                url: path.resolve("gui/apps/url/index.html"),
            },
        },
    },
    resolve: {
        alias: {
            "@components": path.resolve("gui/components"),
            "@css": path.resolve("gui/css"),
            "@libs": path.resolve("gui/libs"),
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
