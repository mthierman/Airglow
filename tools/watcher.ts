import { debounce } from "https://deno.land/std@0.178.0/async/debounce.ts";

const log = debounce((_event: Deno.FsEvent) => {
    console.log("Modified!");
    const cmd = ["pwsh.exe", "-nop", "-f", "tools/debug.ps1"];
    Deno.run({ cmd });
}, 55);

for await (const event of Deno.watchFs("CMakeLists.txt")) {
    log(event);
}
