import iconRaw from "../../data/icons/icon.svg?raw";

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
        <div className="max-w-screen pointer-events-none grid max-h-full select-none">
            <h1 className="place-self-center text-2xl">
                Press ⏸️ to set homepage
            </h1>
        </div>
    );
}
