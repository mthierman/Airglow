import { useEffect, useState, useRef } from "react";

export default function App() {
    let container = useRef(null);
    let [height, setHeight] = useState(null);

    useEffect(() => {
        setHeight(container.current.offsetHeight);
        if (height) console.log(height);
    });

    return (
        <div ref={container}>
            <button>Test</button>
        </div>
    );
}
