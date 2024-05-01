import react from "@vitejs/plugin-react-swc";
import { resolve } from "path";
import { CommonServerOptions, UserConfig, defineConfig } from "vite";

const userConfig: UserConfig = {
    plugins: [react()],
    base: "./",
    root: "gui/apps",
    build: {
        emptyOutDir: true,
        outDir: resolve("build/Release/gui"),
        rollupOptions: {
            output: {
                entryFileNames: `assets/[name].js`,
                chunkFileNames: `assets/[name].js`,
                assetFileNames: `assets/[name].[ext]`,
            },
            input: {
                home: resolve("gui/apps/home/index.html"),
                settings: resolve("gui/apps/settings/index.html"),
                url: resolve("gui/apps/url/index.html"),
            },
        },
    },
    resolve: {
        alias: {
            root: resolve("./"),
            data: resolve("data"),
            apps: resolve("gui/apps"),
            assets: resolve("gui/assets"),
            components: resolve("gui/components"),
            css: resolve("gui/css"),
            libs: resolve("gui/libs"),
            modules: resolve("modules"),
            scripts: resolve("scripts"),
            src: resolve("src"),
        },
    },
};

const commonServerOptions: CommonServerOptions = {
    port: 8000,
    https: {
        pfx: resolve("../.cert/localhost.pfx"),
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
