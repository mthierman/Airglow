import { stdin, stdout } from "node:process";
import { createInterface } from "node:readline/promises";

import { actions } from "modules/actions";

export const tasks = {
    build: async () => {
        actions.build();
    },

    watch: async () => {
        const vite = await actions.watch();

        const cli = createInterface(stdin, stdout);

        cli.on("close", async () => {
            vite.close();
        });
    },

    dev: async () => {
        const vite = await actions.serve();

        const cli = createInterface(stdin, stdout);

        cli.on("close", async () => {
            vite.close();
        });
    },

    preview: async () => {
        const vite = await actions.preview();

        const cli = createInterface(stdin, stdout);

        cli.on("close", async () => {
            vite.close();
        });
    },
};
