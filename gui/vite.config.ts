import { defineConfig } from "vite";
import path from "path";
import react from "@vitejs/plugin-react-swc";

export default defineConfig(async ({ command, mode }) => {
    if (command === "serve") {
        const cert = path.resolve("../../.cert/localhost.pfx");
        const passphrase = "localhost";
        return {
            root: "./src",
            base: "./",
            plugins: [react()],
            server: {
                port: 8000,
                https: {
                    pfx: cert,
                    passphrase: passphrase,
                },
            },
            preview: {
                port: 8000,
                https: {
                    pfx: cert,
                    passphrase: passphrase,
                },
            },
        };
    } else {
        return {
            root: "./src",
            base: "./",
            plugins: [react()],
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
    }
});
