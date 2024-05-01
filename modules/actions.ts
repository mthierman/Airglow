import { copyFile, mkdir, writeFile } from "node:fs/promises";
import type { RollupWatcher } from "rollup";
import { build, createServer, preview } from "vite";

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

    // manifest: () => {
    //     mkdir("dist", { recursive: true })
    //         .then(() => {
    //             writeFile("dist/package.json", JSON.stringify(manifest(), null, 4));
    //         })
    //         .catch(() => process.exit(1));
    // },

    // icon: () => {
    //     copyFile("data/icon.ico", "dist/icon.ico").catch(() =>
    //         console.error("Failed to copy icon"),
    //     );
    // },
};
