import iconRaw from "../../../data/release.svg?raw";

document.onreadystatechange = () => {
    if (document.readyState === "complete") {
        const favicon = document.createElement("link");
        favicon.type = "image/svg+xml";
        favicon.rel = "icon";
        favicon.href = `data:image/svg+xml,${encodeURIComponent(iconRaw)}`;
        document.head.appendChild(favicon);
    }
};

export default function App() {
    return (
        <>
            <h1 className="pointer-events-none select-none font-display text-4xl font-bold">
                Press ⏸️ for settings
            </h1>
        </>
    );
}
