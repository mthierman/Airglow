import * as url from "./url";

let trim = url.trimInput(url.tests.spaces);
console.log(trim);

let parse = url.parseUrl(trim);
console.log(parse);

// let response = await url.getResponse(trim);
// console.log(response);
