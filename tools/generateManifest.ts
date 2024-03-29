import { mkdir, writeFile } from "node:fs/promises";
import { dirname, resolve } from "node:path";
import manifest from "../package.json" with { type: "json" };

const airglow = () => {
    return {
        name: "Airglow",
        version: `v${manifest.version}`,
        date: `${new Date().toLocaleDateString()} ${new Date().toLocaleTimeString("en-US", { hour12: false })}`,
        symbol: "🔵",
        description: "Split screen browser for Windows",
        link: "/airglow",
        github: "https://github.com/mthierman/Airglow",
        download: `https://github.com/mthierman/Airglow/releases/download/${manifest.version}/Airglow.exe`,
        thumbnail: "",
        class: ["text-sky-500", "hover:text-sky-400"],
    };
};

(() => {
    const outdir = resolve(dirname(import.meta.dirname), "build");

    mkdir(outdir, { recursive: true })
        .then(async () => {
            await writeFile(
                resolve(outdir, `${manifest.name}.json`),
                JSON.stringify(airglow(), null, 4),
            );
        })
        .catch(() => process.exit(1));
})();
