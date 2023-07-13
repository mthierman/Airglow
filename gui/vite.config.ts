import { defineConfig } from "vite";
import path from "path";
import react from "@vitejs/plugin-react-swc";

export default defineConfig(async ({ command, mode }) => {
    if (command === "serve") {
        const cert = path.resolve("../../.cert/localhost.pfx");
        const passphrase = "localhost";
        return {
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
            build: {
                outDir: "../build/settings",
                assetsDir: "./",
            },
            plugins: [react()],
        };
    }
});
