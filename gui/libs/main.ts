import * as url from "./url";

const trim = url.trimInput(url.tests.spaces);
console.log(trim);

const parse = url.parseUrl(trim);
console.log(parse);

// let response = await url.getResponse(trim);
// console.log(response);
