import { defineConfig } from "vite";
import path from "path";
import react from "@vitejs/plugin-react-swc";

export default defineConfig(async ({ command, mode, ssrBuild }) => {
    let options = {};
    if (command === "serve") {
        options = {
            port: 8000,
            https: {
                pfx: path.resolve("../../.cert/localhost.pfx"),
                passphrase: "localhost",
            },
        };
    }

    return {
        root: "./src",
        base: "./",
        plugins: [react()],
        server: options,
        preview: options,
        build: {
            outDir: "../dist",
            rollupOptions: {
                input: {
                    main: path.resolve(__dirname, "src/index.html"),
                    settings: path.resolve(
                        __dirname,
                        "src/settings/index.html",
                    ),
                    bar: path.resolve(__dirname, "src/bar/index.html"),
                },
            },
        },
    };
});
