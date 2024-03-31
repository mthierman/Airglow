import { execSync } from "node:child_process";
import { mkdir, writeFile } from "node:fs/promises";
import { dirname, resolve } from "node:path";
import manifest from "../../package.json" with { type: "json" };

const airglow = () => {
    return {
        name: `${manifest.fullname}`,
        description: `${manifest.description}`,
        version: `${manifest.version}`,
        date: `${new Date().toLocaleDateString("en-GB", { day: "2-digit", month: "2-digit", year: "2-digit" })} ${new Date().toLocaleTimeString("en-GB", { hour12: false })}`,
        hash: execSync("git rev-parse HEAD").toString().trim(),
        symbol: "🔵",
        github: "https://github.com/mthierman/Airglow",
        download: `https://github.com/mthierman/Airglow/releases/download/${manifest.version}/Airglow.exe`,
    };
};

(() => {
    const outdir = resolve(dirname(import.meta.dirname));

    mkdir(outdir, { recursive: true })
        .then(async () => {
            await writeFile(resolve(outdir, `manifest.json`), JSON.stringify(airglow(), null, 4));
        })
        .catch(() => process.exit(1));
})();
