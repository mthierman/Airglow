import { debounce } from "https://deno.land/std@0.178.0/async/debounce.ts";

const watch = debounce((_event: Deno.FsEvent) => {
    const cmd = ["pwsh.exe", "-nop", "tools/gen_debug.ps1"];
    Deno.run({ cmd });
}, 55);

for await (const event of Deno.watchFs("CMakeLists.txt")) {
    watch(event);
}
