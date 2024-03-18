const tests = {
    error1: "https://asdasdasdasdasdfkjlwejjklsrghsjkghsjgnsegfnbsgk.com/",
    error2: "http://www.apple",
    http: "http://www.apple.com/",
    https: "https://www.apple.com/",
    www: "www.apple.com",
    com: "google.com",
    searchTerm: "apples",
    spaces: "   apples asdasd",
};

import url from "libs/url";

for (const u of Object.values(tests)) {
    console.log(u);
    console.log(url(u));
    console.log("\n");
}
