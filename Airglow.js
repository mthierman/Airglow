// BROWSERS
document.onreadystatechange = () => {
    if (document.readyState === "interactive") {
        let scheme = document.createElement("meta");
        scheme.setAttribute("name", "color-scheme");
        scheme.setAttribute("content", "light dark");
        document.getElementsByTagName("head")[0].appendChild(scheme);
        document.documentElement.style.setProperty(
            "color-scheme",
            "light dark"
        );
    }
    if (document.readyState === "complete") {
        onkeydown = (e) => {
            if (e.ctrlKey && e.key === "w") {
                window.chrome.webview.postMessage("close");
            } else {
                window.chrome.webview.postMessage(e.key);
            }
        };
    }
};

// SETTINGS
document.onreadystatechange = () => {
    if (document.readyState === "interactive") {
        let scheme = document.createElement("meta");
        scheme.setAttribute("name", "color-scheme");
        scheme.setAttribute("content", "light dark");
        document.getElementsByTagName("head")[0].appendChild(scheme);
        document.documentElement.style.setProperty(
            "color-scheme",
            "light dark"
        );
    }
    if (document.readyState === "complete") {
        onkeydown = (e) => {
            if (e.key === "F3") {
                e.preventDefault();
            }
            if (e.ctrlKey && e.key === "w") {
                window.chrome.webview.postMessage("close");
            } else {
                window.chrome.webview.postMessage(e.key);
            }
        };
    }
};
