import { defineConfig } from "vite";
import path from "path";
import react from "@vitejs/plugin-react-swc";

function settings(command: "serve" | "build") {
    if (command === "serve") {
        const cert = path.resolve("../../.cert/localhost.pfx");
        const passphrase = "localhost";
        const settings = {
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
        return settings;
    }
    if (command === "build") {
        const settings = {
            plugins: [react()],
        };
        return settings;
    }
}

export default defineConfig(({ command }) => {
    if (command === "serve") {
        return settings(command);
    }
    if (command === "build") {
        return settings(command);
    } else return;
});
