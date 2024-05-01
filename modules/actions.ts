import { execSync } from "node:child_process";
import type { RollupWatcher } from "rollup";
import { build, createServer, preview } from "vite";

import manifest from "root/package.json" with { type: "json" };

export const actions = {
    build: async () => {
        return await build();
    },

    watch: async () => {
        return (await build({
            build: { emptyOutDir: false, watch: { chokidar: { usePolling: false } } },
        })) as RollupWatcher;
    },

    serve: async (host = false) => {
        const server = await createServer({ server: { host: host } });

        await server.listen();

        server.printUrls();
        server.bindCLIShortcuts({ print: true });

        return server;
    },

    preview: async (host = false) => {
        const server = await preview({ server: { host: host } });

        server.printUrls();
        server.bindCLIShortcuts({ print: true });

        return server;
    },

    manifest: () => {
        return {
            name: `${manifest.fullname}`,
            description: `${manifest.description}`,
            version: `${manifest.version}`,
            date: `${new Date().toLocaleDateString("en-GB", { day: "2-digit", month: "2-digit", year: "2-digit" })} ${new Date().toLocaleTimeString("en-GB", { hour12: false })}`,
            hash: execSync("git rev-parse HEAD").toString().trim(),
            symbol: "🔵",
            github: "https://github.com/mthierman/Airglow",
            download: `https://github.com/mthierman/Airglow/releases/download/v${manifest.version}/Airglow.exe`,
        };
    },
};
