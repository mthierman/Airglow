import react from "@vitejs/plugin-react-swc";
import path from "path";
import { defineConfig, UserConfig } from "vite";

const defaults: UserConfig = {
    plugins: [react()],
    base: "./",
    root: "gui/apps",
    build: {
        emptyOutDir: true,
        outDir: path.resolve("build/Release/gui"),
        rollupOptions: {
            output: {
                entryFileNames: `assets/[name].js`,
                chunkFileNames: `assets/[name].js`,
                assetFileNames: `assets/[name].[ext]`,
            },
            input: {
                home: path.resolve("gui/apps/home/index.html"),
                settings: path.resolve("gui/apps/settings/index.html"),
                url: path.resolve("gui/apps/url/index.html"),
            },
        },
    },
    resolve: {
        alias: {
            "#assets": path.resolve("gui/assets"),
            "#components": path.resolve("gui/components"),
            "#css": path.resolve("gui/css"),
            "#libs": path.resolve("gui/libs"),
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
export default defineConfig(async ({ command, mode, isPreview }) => {
    switch (command) {
        case "serve": {
            return {
                ...defaults,
                server: { ...server },
                preview: { ...server },
            };
        }
        default: {
            return {
                ...defaults,
            };
        }
    }
});
