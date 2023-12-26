import { defineConfig } from "vite";
import path from "path";
import react from "@vitejs/plugin-react-swc";

const defaults = {
    plugins: [react()],
    base: "./",
    root: "src",
    build: {
        outDir: "../dist",
        rollupOptions: {
            input: {
                main: path.resolve(__dirname, "src/index.html"),
                settings: path.resolve(__dirname, "src/settings/index.html"),
                bar: path.resolve(__dirname, "src/bar/index.html"),
            },
        },
    },
};

const server = {
    port: 8000,
    https: {
        pfx: path.resolve("../../../.cert/localhost.pfx"),
        passphrase: "localhost",
    },
};

export default defineConfig(async ({ mode }) => {
    switch (mode) {
        case "development":
            return {
                ...defaults,
                server: {
                    ...server,
                },
                preview: {
                    ...server,
                },
            };
        default:
            return {
                ...defaults,
            };
    }
});
