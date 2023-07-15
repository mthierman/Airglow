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
            if (e.key === "F1") {
                e.preventDefault();
                window.chrome.webview.postMessage("split");
            }
            if (e.key === "F2") {
                e.preventDefault();
                window.chrome.webview.postMessage("swapped");
            }
            if (e.key === "F3") {
                e.preventDefault();
            }
            if (e.key === "F4") {
                e.preventDefault();
                window.chrome.webview.postMessage("menu");
            }
            if (e.key === "F6") {
                e.preventDefault();
                window.chrome.webview.postMessage("maximize");
            }
            if (e.key === "F9") {
                e.preventDefault();
                window.chrome.webview.postMessage("topmost");
            }
            if (e.key === "F11") {
                e.preventDefault();
                window.chrome.webview.postMessage("fullscreen");
            }
            if (e.ctrlKey && e.key === "w") {
                e.preventDefault();
                window.chrome.webview.postMessage("close");
            } else {
                // window.chrome.webview.postMessage(e.key);
            }
        };
    }
};
