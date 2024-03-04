import url, { values } from "../libs/url";

for (const u of values) {
    console.log(u);
    console.log(url(u));
    console.log("\n");
}
