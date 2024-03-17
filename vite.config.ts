import react from "@vitejs/plugin-react-swc";
import path from "path";
import { CommonServerOptions, UserConfig, defineConfig } from "vite";

const userConfig: UserConfig = {
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
            data: path.resolve("data"),
            apps: path.resolve("gui/apps"),
            assets: path.resolve("gui/assets"),
            components: path.resolve("gui/components"),
            css: path.resolve("gui/css"),
            libs: path.resolve("gui/libs"),
        },
    },
};

const commonServerOptions: CommonServerOptions = {
    port: 8000,
    https: {
        pfx: path.resolve("../.cert/localhost.pfx"),
        passphrase: "localhost",
    },
};

// https://vitejs.dev/config/
export default defineConfig(({ command }) => {
    switch (command) {
        case "serve": {
            return {
                ...userConfig,
                server: { ...commonServerOptions },
                preview: { ...commonServerOptions },
            };
        }
        default: {
            return {
                ...userConfig,
            };
        }
    }
});
