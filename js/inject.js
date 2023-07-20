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
};
