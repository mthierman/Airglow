import "@css/home.css";
// import debug from "@assets/debug.svg?raw";
import release from "@assets/release.svg?raw";

document.onreadystatechange = () => {
    if (document.readyState === "complete") {
        const favicon = document.createElement("link");
        favicon.type = "image/svg+xml";
        favicon.rel = "icon";
        favicon.href = `data:image/svg+xml,${encodeURIComponent(release)}`;
        document.head.appendChild(favicon);
    }
};

export default () => {
    return (
        <h1 className="pointer-events-none select-none text-4xl font-thin">
            Press ⏸️ for settings
        </h1>
    );
};
