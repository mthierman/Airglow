import { execSync } from "node:child_process";
import { rm } from "node:fs";
import { mkdir, writeFile } from "node:fs/promises";
import { resolve } from "node:path";
import manifest from "../package.json" with { type: "json" };

const outdir = resolve(import.meta.dirname, "..", "build");

mkdir(outdir, { recursive: true })
    .then(async () => {
        await writeFile(
            resolve(outdir, `manifest.json`),
            JSON.stringify(
                {
                    name: `${manifest.fullname}`,
                    description: `${manifest.description}`,
                    version: `${manifest.version}`,
                    date: `${new Date().toLocaleDateString("en-GB", { day: "2-digit", month: "2-digit", year: "2-digit" })} ${new Date().toLocaleTimeString("en-GB", { hour12: false })}`,
                    hash: execSync("git rev-parse HEAD").toString().trim(),
                    symbol: "🔵",
                    github: "https://github.com/mthierman/Airglow",
                    download: `https://github.com/mthierman/Airglow/releases/download/v${manifest.version}/Airglow.exe`,
                },
                null,
                4,
            ),
        );
    })
    .catch(() => process.exit(1))
    .finally(() => {
        rm(resolve(import.meta.dirname, "manifest.js"), () => {});
    });
