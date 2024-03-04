// import * as url from "../libs/url";
// import response from "#libs/response";
import url, { values } from "../libs/url";

for (const u of values) {
    console.log(u);
    console.log(url(u));
    console.log("\n");
}

// const test = url("http://asd");

// console.log(`${test}, ${test.length}`);
// console.log(await response(test));
